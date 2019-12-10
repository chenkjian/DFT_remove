////功能：测试利用傅里叶变换去除布料中的网纹

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
//Mat proceFre(Mat & src);
//
//int main()
//{
//	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
//
//	//Mat src = imread("C:/Users/chenkj/Desktop/测试印花/person.jpg");
//	Mat src = imread("C:/Users/chenkj/Desktop/测试印花/img_3534.tif"); 
//
//		//int nWidth, nHeight;
//		//nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
//		//nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
//		//printf("当前屏幕的分辨率为：%d×%d", nWidth, nHeight);
//
//		//int newHeight, newWidth;
//		//if ((src.rows / nHeight) >= (src.cols / nWidth))
//		//{
//		//	newHeight = nHeight;
//		//	newWidth = nHeight*src.cols / src.rows;
//		//}
//		//else {
//		//	newHeight = nWidth*src.rows / src.cols;
//		//	newWidth = nWidth;
//		//}
//	
//		//Size size = Size(newWidth, newHeight);
//		//resize(src, src, size, 0, 0);
//
//
//	Mat srcGray;
//	cvtColor(src, srcGray, CV_RGB2GRAY); //灰度图像做傅里叶变换
//	imwrite("灰度图.jpg", srcGray);
//
//	int m = getOptimalDFTSize(srcGray.rows); //2,3,5的倍数有更高效率的傅里叶变换
//	int n = getOptimalDFTSize(srcGray.cols);
//	cout << m << endl;
//	cout << n << endl;
//	Mat padded;
//	//把灰度图像放在左上角,在右边和下边扩展图像,扩展部分填充为0;
//	copyMakeBorder(srcGray, padded, 0, m - srcGray.rows, 0, n - srcGray.cols, BORDER_CONSTANT, Scalar::all(0));
//	cout << padded.size() << endl;
//	//这里是获取了两个Mat,一个用于存放dft变换的实部，一个用于存放虚部,初始的时候,实部就是图像本身,虚部全为零
//	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };//在这里是声明两个Mat矩阵，分别存放实部和虚部
//	Mat planes_true = Mat_<float>(padded);
//	Mat ph = Mat_<float>(padded);
//
//	Mat complexImg;
//	//将几个单通道的mat融合成一个多通道的mat,这里融合的complexImg既有实部又有虚部
//	merge(planes, 2, complexImg);
//
//	//对上边合成的mat进行傅里叶变换,***支持原地操作***,傅里叶变换结果为复数.通道1存的是实部,通道二存的是虚部
//	dft(complexImg, complexImg);
//	//把变换后的结果分割到两个mat,一个实部,一个虚部,方便后续操作
//	split(complexImg, planes);
//	//这一部分是为了计算dft变换后的幅值，傅立叶变换的幅度值范围大到不适合在屏幕上显示。高值在屏幕上显示为白点，而低值为黑点，高低值的变化无法有效分辨。为了在屏幕上凸显出高低变化的连续性，我们可以用对数尺度来替换线性尺度,以便于显示幅值,计算公式如下:
//	//=> log(1 + sqrt(Re(DFT(I))^2 +Im(DFT(I))^2))
//	//计算实数和虚数的幅值，并把幅值保存到palnes[0],允许在线覆盖
//	magnitude(planes[0], planes[1], planes_true);//幅度谱mag
//	phase(planes[0], planes[1], ph);//相位谱ph
//	Mat A = planes[0];
//	Mat B = planes[1];
//	Mat mag = planes_true;
//
//
//	//对幅值加1
//	mag += Scalar::all(1);
//	log(mag, mag);//计算出的幅值一般很大，达到10^4,通常没有办法在图像中显示出来，需要对其进行log求解。
//	
//	double maxVal;
//	minMaxLoc(mag, 0, &maxVal, 0, 0);
//
//	//processFre(mag, planes);
//	//--------------------------------------------------------------------------------
//	//mag = proceFre(mag);
//	//imshow("处理后频谱", mag);
//	//polarToCart(mag, ph, planes[0], planes[1]);//由幅度谱mag和相位谱ph恢复实部planes[0]和虚部planes[1]
//	//merge(planes, 2, complexImg);
//
//	//crop the spectrum, if it has an odd number of rows or columns
//	//修剪频谱,如果图像的行或者列是奇数的话,那其频谱是不对称的,因此要修剪
//	//这里为什么要用  &-2这个操作，我会在代码后面的 注2 说明
//	//我们知道x&-2代表x与 - 2按位相与，而 - 2的二进制形式是2的二进制取反加一的结果（这是补码的问题）。2 的二进制结果是（假设
//	//8位表示，实际整型是32位，但是描述方式是一样的，为便于描述，用8位表示）0000 0010，则 - 2的二进制形式为：1111 1110，
//	//x与 - 2按位相与后，不管x是奇数还是偶数，最后x都会变成一个偶数。 
//	//就是说dft这个函数虽然对于输入mat的尺寸不做要求，但是如果其行数和列数可以分解为2、3、5的乘积，那么对于dft运算的速度会加快很多。
//	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
//	Mat _magI = mag.clone();
//	//这一步的目的仍然是为了显示,但是幅度值仍然超过可显示范围[0,1],我们使用 normalize() 函数将幅度归一化到可显示范围。
//	normalize(_magI, _magI, 0, 1, CV_MINMAX);
//	
//	//namedWindow("before rearrange", 0);
//	imshow("before rearrange", _magI);
//
//	//_magI = proceFre(_magI);
//	//imshow("处理后频谱", _magI);
//	////planes[0] = sqrt(pow(mag, 2). / (1 + pow(tan(ph), 2)));
//	////planes[1] = planes[0].mul(tan(ph));
//	//merge(planes, 2, complexImg);
//
//	//rearrange the quadrants of Fourier image
//	//so that the origin is at the image center
//	//重新分配象限，使（0,0）移动到图像中心，  
//	//在《数字图像处理》中，傅里叶变换之前要对源图像乘以（-1）^(x+y)进行中心化。  
//	//这是是对傅里叶变换结果进行中心化
//	/*----------显示规则频谱图--------------------*/
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
//
//	normalize(mag, mag, 0, 1, CV_MINMAX);
//	//namedWindow("Input Image", 0);
//	//namedWindow("spectrum magnitude", 0);
//	imshow("原图灰度图", srcGray);
//	imshow("频谱幅度", mag);
//	mag = mag * 255;
//	imwrite("频谱幅度.jpg", mag);
//	/*--------------------------------------------------*/
//
//	mag = mag / 255;
//	mag = proceFre(mag);
//	//imshow("处理后频谱", mag);
//
//	//这里是以中心为标准，把mag图像分成四部分
//	
//    Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//    //swap quadrants(Top-Left with Bottom-Right)，交换象限
//	q00.copyTo(tmp);
//	q30.copyTo(q00);
//	tmp.copyTo(q30);
//	// swap quadrant (Top-Rightwith Bottom-Left)，交换象限
//	q10.copyTo(tmp);
//	q20.copyTo(q10);
//	tmp.copyTo(q20);
//
//	imshow("坐标变换", mag);
//
//	mag = mag * maxVal;
//	exp(mag, mag);
//	mag = mag - Scalar::all(1);
//	polarToCart(mag, ph, planes[0], planes[1]);//由幅度谱mag和相位谱ph恢复实部planes[0]和虚部planes[1]
//	merge(planes, 2, complexImg);
//
//
//	//Mat div;
//	//divide(mag.mul(mag), 1 + atan2(ph).mul(atan2(ph)), div);
//	//planes[0] = sqrt(div);
//	//planes[1] = planes[0].mul(tan(ph));
//
//	//-----------------------傅里叶的逆变换-----------------------------------
//	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
//	//傅里叶逆变换
//	idft(complexImg, ifft, DFT_REAL_OUTPUT);
//	normalize(ifft, ifft, 0, 1, CV_MINMAX);
//	
//	Rect rect(0, 0, src.cols, src.rows);
//	Mat ifft2 = ifft(rect);
//	//namedWindow("inverse fft", 0);
//	imshow("逆变换后的图", ifft2);
//	ifft2 = ifft2 * 255;
//	imwrite("rebuild.jpg", ifft2);
//	waitKey();
//	return 0;
//}
//
//
//
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
//
//
