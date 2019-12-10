//功能：高低频不同图像

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Windows.h>
#include <cmath>

using namespace cv;
using namespace std;

Mat tranform(Mat& src, int flag, Mat& dst);
Mat proceFre(Mat & src, int flag);
void resizeSyetem(Mat& src, Mat& dst);

int main()
{
	//Mat srcImage1 = imread("H:\\old\\Canada\\animal\\mmexport1514286161524.jpg");
	Mat srcImage1 = imread("H:\\old\\Canada\\animal\\IMG_20190706_112230.jpg"); 
	Mat srcImage2 = imread("H:\\old\\Canada\\animal\\IMG_20190709_091815.jpg");	

	resizeSyetem(srcImage1, srcImage1);
	resize(srcImage2, srcImage2, srcImage1.size(), 0);

	imshow("原图1", srcImage1);
	imshow("原图2", srcImage2);

	Mat dstImage1;
	Mat dstImage2;
	Mat srcChannels1[3];//存放原图1三个通道
	Mat srcChannels2[3];//存放原图2三个通道
	Mat dstChannels1[3];//存放结果图1三个通道
	Mat dstChannels2[3];//存放结果图2三个通道

	split(srcImage1, srcChannels1);
	split(srcImage2, srcChannels2);

	for (size_t i = 0; i < 3; i++)
	{
		tranform(srcChannels1[i], 0, dstChannels1[i]);
		tranform(srcChannels2[i], 1, dstChannels2[i]);
	}

	merge(dstChannels1, 3, dstImage1);
	imshow("高频", dstImage1/255);
	merge(dstChannels2, 3, dstImage2);
	imshow("低频", dstImage1 / 255);

	Mat dst = dstImage1/510+ dstImage2/510;
	imshow("合并", dst);
	//imwrite("彩色图处理结果.jpg", dstImage);

	waitKey(0);
	return 0;
}

/*//////////////////////////////////fft///////////////////////////////////////////////////////////////////
功能：保留图像高频信息或低频信息
输入：src:单通道灰度图（一般为彩色图的各个通道）
      flag:0-保留高频信息；1-保留低频信息
输出：处理完成的图
////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
Mat tranform(Mat & src, int flag, Mat & dst)
{
	int m = getOptimalDFTSize(src.rows); //2,3,5的倍数有更高效率的傅里叶变换
	int n = getOptimalDFTSize(src.cols);
	Mat padded;
	//把灰度图像放在左上角,在右边和下边扩展图像,扩展部分填充为0;
	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
	//planes[0]为dft变换的实部，planes[1]为虚部，ph为相位， plane_true=mag为幅值
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat planes_true = Mat_<float>(padded);
	Mat ph = Mat_<float>(padded);
	Mat complexImg;
	//多通道complexImg既有实部又有虚部
	merge(planes, 2, complexImg);
	//对上边合成的mat进行傅里叶变换,***支持原地操作***,傅里叶变换结果为复数.通道1存的是实部,通道二存的是虚部
	dft(complexImg, complexImg);
	//把变换后的结果分割到两个mat,一个实部,一个虚部,方便后续操作
	split(complexImg, planes);
	
	//---------------此部分目的为更好地显示幅值---后续恢复原图时反着再处理一遍-------------------------
	magnitude(planes[0], planes[1], planes_true);//幅度谱mag
	phase(planes[0], planes[1], ph);//相位谱ph
	Mat A = planes[0];
	Mat B = planes[1];
	Mat mag = planes_true;

	mag += Scalar::all(1);//对幅值加1
	log(mag, mag);//计算出的幅值一般很大，达到10^4,通常没有办法在图像中显示出来，需要对其进行log求解。

	//取矩阵中的最大值，便于后续还原时去归一化
	double maxVal;
	minMaxLoc(mag, 0, &maxVal, 0, 0);

	//修剪频谱,如果图像的行或者列是奇数的话,那其频谱是不对称的,因此要修剪
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
	Mat _magI = mag.clone();
	//将幅度归一化到可显示范围。
	normalize(_magI, _magI, 0, 1, CV_MINMAX);
//	imshow("before rearrange", _magI);

	//显示规则频谱图
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	//这里是以中心为标准，把mag图像分成四部分
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
	Mat q1(mag, Rect(cx, 0, cx, cy));  //Top-Right
	Mat q2(mag, Rect(0, cy, cx, cy));  //Bottom-Left
	Mat q3(mag, Rect(cx, cy, cx, cy)); //Bottom-Right

	//swap quadrants(Top-Left with Bottom-Right)，交换象限
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	// swap quadrant (Top-Rightwith Bottom-Left)，交换象限
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, CV_MINMAX);
//	imshow("原图灰度图", src);
//	imshow("频谱幅度", mag);
	mag = mag * 255;
//	imwrite("频谱幅度.jpg", mag);
	/*--------------------------------------------------*/

	mag = mag / 255;
	mag = proceFre(mag, flag); // 处理规则的频谱
	string name = "处理后频谱" + flag;
	imshow(name, mag);

	//前述步骤反着来一遍，目的是为了逆变换回原图
	Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
    //交换象限
	q00.copyTo(tmp);
	q30.copyTo(q00);
	tmp.copyTo(q30);
	q10.copyTo(tmp);
	q20.copyTo(q10);
	tmp.copyTo(q20);

	mag = mag * maxVal;//将归一化的矩阵还原 
	exp(mag, mag);//对应于前述去对数
	mag = mag - Scalar::all(1);//对应前述+1
	polarToCart(mag, ph, planes[0], planes[1]);//由幅度谱mag和相位谱ph恢复实部planes[0]和虚部planes[1]
	merge(planes, 2, complexImg);//将实部虚部合并


	//-----------------------傅里叶的逆变换-----------------------------------
	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
	//傅里叶逆变换
	idft(complexImg, ifft, DFT_REAL_OUTPUT);
	normalize(ifft, ifft, 0, 1, CV_MINMAX);

	Rect rect(0, 0, src.cols, src.rows);
	dst = ifft(rect);
//	imshow("复原后的图", dst);
	dst = dst * 255;
//	imwrite("复原后的图.jpg", dst);
	//waitKey();

	return dst;
}


/////////////////////////////////处理频谱//////////////////////////////////////////////
Mat proceFre(Mat & src, int flag)
{
	if (flag == 0)
	{
		////保留高频信息
		//circle(src, Point(0, src.rows), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(src.cols, 0), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(0, 0), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(src.cols, src.rows), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//保留高频信息
		//circle(src, Point(src.cols / 2, src.rows / 2), min(src.cols, src.rows) / 80, Scalar(0), -1);
		for (int i = 0; i < src.rows; i++) {
			float* data = src.ptr<float>(i);
			for (int j = 0; j < src.cols * src.channels(); j++) {
				if ((pow(src.cols / 2 - j, 2) + pow(src.rows / 2 - i, 2)) > (pow(min(src.cols, src.rows) / 50, 2))) {
					data[j] = 0;
				}
			}
		}
	}
	else
	{
		//保留低频信息
		circle(src, Point(src.cols / 2, src.rows / 2), min(src.cols, src.rows) / 50, Scalar(0), -1);
	}

	return src;
}

void resizeSyetem(Mat & src, Mat & dst)
{
	int nWidth, nHeight;
	nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	printf("当前屏幕的分辨率为：%d×%d", nWidth, nHeight);

	int newHeight, newWidth;
	if ((src.rows / nHeight) >= (src.cols / nWidth))
	{
		newHeight = nHeight;
		newWidth = nHeight * src.cols / src.rows;
	}
	else {
		newHeight = nWidth * src.rows / src.cols;
		newWidth = nWidth;
	}

	Size size = Size(newWidth, newHeight);
	resize(src, dst, size, 0, 0);
}

