////���ܣ���ͼ����С����Ļȫ�ɼ�

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
//	Mat src = imread("C:/Users/chenkj/Desktop/����ӡ��/14.png");
//	int nWidth, nHeight;
//	nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
//	nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
//	printf("��ǰ��Ļ�ķֱ���Ϊ��%d��%d", nWidth, nHeight);
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
//	cv::imshow("ͼ", src);
//	cv::waitKey(0);
//	system("pause");
//	return 0;
//}
