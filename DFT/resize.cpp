////功能：将图像缩小至屏幕全可见

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <Windows.h>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	Mat src = imread("C:/Users/chenkj/Desktop/测试印花/14.png");
//	int nWidth, nHeight;
//	nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
//	nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
//	printf("当前屏幕的分辨率为：%d×%d", nWidth, nHeight);
//
//	int newHeight, newWidth;
//	if ((src.rows / nHeight) >= (src.cols / nWidth))
//	{
//		newHeight = nHeight;
//		newWidth = nHeight*src.cols / src.rows;
//	}
//	else {
//		newHeight = nWidth*src.rows / src.cols;
//		newWidth = nWidth;
//	}
//
//	Size size = Size(newWidth, newHeight);
//	resize(src, src, size, 0, 0);
//
//	cv::imshow("图", src);
//	cv::waitKey(0);
//	system("pause");
//	return 0;
//}
