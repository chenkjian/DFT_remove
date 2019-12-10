////功能：利用傅里叶变换去除布料中的网纹

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <Windows.h>
//#include <cmath>
//
//using namespace cv;
//using namespace std;
//
//Mat removeGridding(Mat& src, Mat& dst);
//Mat proceFre(Mat & src);
//
//int main()
//{
//	Mat srcImage = imread("C:/Users/chenkj/Desktop/测试印花/img_3534.tif");
//	//Mat srcImage = imread("C:/Users/chenkj/Desktop/测试印花/person.jpg");
//	imshow("原图", srcImage);
//	Mat dstImage;
//	Mat srcChannels[3];//存放原图三个通道
//	Mat dstChannels[3];//存放原图三个通道
//
//	split(srcImage, srcChannels);
//
//	for (size_t i = 0; i < 3; i++)
//	{
//		removeGridding(srcChannels[i], dstChannels[i]);
//	}
//
//	merge(dstChannels, 3, dstImage);
//	imshow("复原后的图", dstImage/255);
//	imwrite("彩色图处理结果.jpg", dstImage);
//
//	waitKey(0);
//	return 0;
//}
//
///*//////////////////////////////////去除网格///////////////////////////////////////////////////////////////////
//功能：去除图像中的网格
//输入：单通道灰度图（一般为彩色图的各个通道）
//输出：处理完成的图
//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//Mat removeGridding(Mat & src, Mat & dst)
//{
//	int m = getOptimalDFTSize(src.rows); //2,3,5的倍数有更高效率的傅里叶变换
//	int n = getOptimalDFTSize(src.cols);
//	Mat padded;
//	//把灰度图像放在左上角,在右边和下边扩展图像,扩展部分填充为0;
//	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
//	//planes[0]为dft变换的实部，planes[1]为虚部，ph为相位， plane_true=mag为幅值
//	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
//	Mat planes_true = Mat_<float>(padded);
//	Mat ph = Mat_<float>(padded);
//	Mat complexImg;
//	//多通道complexImg既有实部又有虚部
//	merge(planes, 2, complexImg);
//	//对上边合成的mat进行傅里叶变换,***支持原地操作***,傅里叶变换结果为复数.通道1存的是实部,通道二存的是虚部
//	dft(complexImg, complexImg);
//	//把变换后的结果分割到两个mat,一个实部,一个虚部,方便后续操作
//	split(complexImg, planes);
//	
//	//---------------此部分目的为更好地显示幅值---后续恢复原图时反着再处理一遍-------------------------
//	magnitude(planes[0], planes[1], planes_true);//幅度谱mag
//	phase(planes[0], planes[1], ph);//相位谱ph
//	Mat A = planes[0];
//	Mat B = planes[1];
//	Mat mag = planes_true;
//
//	mag += Scalar::all(1);//对幅值加1
//	log(mag, mag);//计算出的幅值一般很大，达到10^4,通常没有办法在图像中显示出来，需要对其进行log求解。
//
//	//取矩阵中的最大值，便于后续还原时去归一化
//	double maxVal;
//	minMaxLoc(mag, 0, &maxVal, 0, 0);
//
//	//修剪频谱,如果图像的行或者列是奇数的话,那其频谱是不对称的,因此要修剪
//	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
//	Mat _magI = mag.clone();
//	//将幅度归一化到可显示范围。
//	normalize(_magI, _magI, 0, 1, CV_MINMAX);
////	imshow("before rearrange", _magI);
//
//	//显示规则频谱图
//	int cx = mag.cols / 2;
//	int cy = mag.rows / 2;
//
//	//这里是以中心为标准，把mag图像分成四部分
//	Mat tmp;
//	Mat q0(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q1(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q2(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q3(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//
//	//swap quadrants(Top-Left with Bottom-Right)，交换象限
//	q0.copyTo(tmp);
//	q3.copyTo(q0);
//	tmp.copyTo(q3);
//
//	// swap quadrant (Top-Rightwith Bottom-Left)，交换象限
//	q1.copyTo(tmp);
//	q2.copyTo(q1);
//	tmp.copyTo(q2);
//
//	normalize(mag, mag, 0, 1, CV_MINMAX);
////	imshow("原图灰度图", src);
////	imshow("频谱幅度", mag);
//	mag = mag * 255;
////	imwrite("频谱幅度.jpg", mag);
//	/*--------------------------------------------------*/
//
//	mag = mag / 255;
//	mag = proceFre(mag); // 处理规则的频谱
//	//imshow("处理后频谱", mag);
//
//	//前述步骤反着来一遍，目的是为了逆变换回原图
//	Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//    //交换象限
//	q00.copyTo(tmp);
//	q30.copyTo(q00);
//	tmp.copyTo(q30);
//	q10.copyTo(tmp);
//	q20.copyTo(q10);
//	tmp.copyTo(q20);
//
//	mag = mag * maxVal;//将归一化的矩阵还原 
//	exp(mag, mag);//对应于前述去对数
//	mag = mag - Scalar::all(1);//对应前述+1
//	polarToCart(mag, ph, planes[0], planes[1]);//由幅度谱mag和相位谱ph恢复实部planes[0]和虚部planes[1]
//	merge(planes, 2, complexImg);//将实部虚部合并
//
//
//	//-----------------------傅里叶的逆变换-----------------------------------
//	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
//	//傅里叶逆变换
//	idft(complexImg, ifft, DFT_REAL_OUTPUT);
//	normalize(ifft, ifft, 0, 1, CV_MINMAX);
//
//	Rect rect(0, 0, src.cols, src.rows);
//	dst = ifft(rect);
////	imshow("复原后的图", dst);
//	dst = dst * 255;
////	imwrite("复原后的图.jpg", dst);
//	//waitKey();
//
//	return dst;
//}
//
//
///////////////////////////////////处理频谱//////////////////////////////////////////////
//Mat proceFre(Mat & src)
//{
//
//	//[0]进行二值化
//	Mat image_threshold;
//	Mat image_dilate;
//	Mat image_erode;
//	Mat image_reverse;
//	Mat image_median;
//	//Mat dst(src.rows,src.cols,CV_8UC1,Scalar(255));
//	Mat dst;
//	dst = src.clone();
//	//adaptiveThreshold(src, image_threshold, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 25, 2);
//	//threshold(src, image_threshold, 0, 255, CV_THRESH_OTSU);
//	threshold(src, image_threshold, 0.5, 1, CV_THRESH_BINARY);
//	//[1]进行膨胀操作
//	Mat erode_element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	Mat dilate_element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	dilate(image_threshold, image_dilate, dilate_element);
//	//[2]进行腐蚀操作
//	erode(image_dilate, image_erode, erode_element);
//	//erode(image_threshold, image_erode, erode_element);
//	//dilate(image_erode, image_dilate, dilate_element);
//	//[3]进行反色操作
//	image_reverse = 1 - image_erode;
//	//[4]保留中心核
//	circle(image_reverse, Point(src.cols / 2, src.rows / 2), min(src.cols, src.rows) / 6, Scalar(1), -1);
//	//[5]中值滤波
//	medianBlur(image_reverse, image_median, 5);
//	//dilate(image_median, image_median, dilate_element);
//
//	//原频谱图乘上处理后的频谱图
//	dst = src.mul(image_median);
//
//	return dst;
//}
//
