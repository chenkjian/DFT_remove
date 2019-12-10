//���ܣ��ߵ�Ƶ��ͬͼ��

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

	imshow("ԭͼ1", srcImage1);
	imshow("ԭͼ2", srcImage2);

	Mat dstImage1;
	Mat dstImage2;
	Mat srcChannels1[3];//���ԭͼ1����ͨ��
	Mat srcChannels2[3];//���ԭͼ2����ͨ��
	Mat dstChannels1[3];//��Ž��ͼ1����ͨ��
	Mat dstChannels2[3];//��Ž��ͼ2����ͨ��

	split(srcImage1, srcChannels1);
	split(srcImage2, srcChannels2);

	for (size_t i = 0; i < 3; i++)
	{
		tranform(srcChannels1[i], 0, dstChannels1[i]);
		tranform(srcChannels2[i], 1, dstChannels2[i]);
	}

	merge(dstChannels1, 3, dstImage1);
	imshow("��Ƶ", dstImage1/255);
	merge(dstChannels2, 3, dstImage2);
	imshow("��Ƶ", dstImage1 / 255);

	Mat dst = dstImage1/510+ dstImage2/510;
	imshow("�ϲ�", dst);
	//imwrite("��ɫͼ������.jpg", dstImage);

	waitKey(0);
	return 0;
}

/*//////////////////////////////////fft///////////////////////////////////////////////////////////////////
���ܣ�����ͼ���Ƶ��Ϣ���Ƶ��Ϣ
���룺src:��ͨ���Ҷ�ͼ��һ��Ϊ��ɫͼ�ĸ���ͨ����
      flag:0-������Ƶ��Ϣ��1-������Ƶ��Ϣ
�����������ɵ�ͼ
////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
Mat tranform(Mat & src, int flag, Mat & dst)
{
	int m = getOptimalDFTSize(src.rows); //2,3,5�ı����и���Ч�ʵĸ���Ҷ�任
	int n = getOptimalDFTSize(src.cols);
	Mat padded;
	//�ѻҶ�ͼ��������Ͻ�,���ұߺ��±���չͼ��,��չ�������Ϊ0;
	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
	//planes[0]Ϊdft�任��ʵ����planes[1]Ϊ�鲿��phΪ��λ�� plane_true=magΪ��ֵ
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat planes_true = Mat_<float>(padded);
	Mat ph = Mat_<float>(padded);
	Mat complexImg;
	//��ͨ��complexImg����ʵ�������鲿
	merge(planes, 2, complexImg);
	//���ϱߺϳɵ�mat���и���Ҷ�任,***֧��ԭ�ز���***,����Ҷ�任���Ϊ����.ͨ��1�����ʵ��,ͨ����������鲿
	dft(complexImg, complexImg);
	//�ѱ任��Ľ���ָ����mat,һ��ʵ��,һ���鲿,�����������
	split(complexImg, planes);
	
	//---------------�˲���Ŀ��Ϊ���õ���ʾ��ֵ---�����ָ�ԭͼʱ�����ٴ���һ��-------------------------
	magnitude(planes[0], planes[1], planes_true);//������mag
	phase(planes[0], planes[1], ph);//��λ��ph
	Mat A = planes[0];
	Mat B = planes[1];
	Mat mag = planes_true;

	mag += Scalar::all(1);//�Է�ֵ��1
	log(mag, mag);//������ķ�ֵһ��ܴ󣬴ﵽ10^4,ͨ��û�а취��ͼ������ʾ��������Ҫ�������log��⡣

	//ȡ�����е����ֵ�����ں�����ԭʱȥ��һ��
	double maxVal;
	minMaxLoc(mag, 0, &maxVal, 0, 0);

	//�޼�Ƶ��,���ͼ����л������������Ļ�,����Ƶ���ǲ��ԳƵ�,���Ҫ�޼�
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
	Mat _magI = mag.clone();
	//�����ȹ�һ��������ʾ��Χ��
	normalize(_magI, _magI, 0, 1, CV_MINMAX);
//	imshow("before rearrange", _magI);

	//��ʾ����Ƶ��ͼ
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	//������������Ϊ��׼����magͼ��ֳ��Ĳ���
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
	Mat q1(mag, Rect(cx, 0, cx, cy));  //Top-Right
	Mat q2(mag, Rect(0, cy, cx, cy));  //Bottom-Left
	Mat q3(mag, Rect(cx, cy, cx, cy)); //Bottom-Right

	//swap quadrants(Top-Left with Bottom-Right)����������
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	// swap quadrant (Top-Rightwith Bottom-Left)����������
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, CV_MINMAX);
//	imshow("ԭͼ�Ҷ�ͼ", src);
//	imshow("Ƶ�׷���", mag);
	mag = mag * 255;
//	imwrite("Ƶ�׷���.jpg", mag);
	/*--------------------------------------------------*/

	mag = mag / 255;
	mag = proceFre(mag, flag); // ��������Ƶ��
	string name = "�����Ƶ��" + flag;
	imshow(name, mag);

	//ǰ�����跴����һ�飬Ŀ����Ϊ����任��ԭͼ
	Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
    //��������
	q00.copyTo(tmp);
	q30.copyTo(q00);
	tmp.copyTo(q30);
	q10.copyTo(tmp);
	q20.copyTo(q10);
	tmp.copyTo(q20);

	mag = mag * maxVal;//����һ���ľ���ԭ 
	exp(mag, mag);//��Ӧ��ǰ��ȥ����
	mag = mag - Scalar::all(1);//��Ӧǰ��+1
	polarToCart(mag, ph, planes[0], planes[1]);//�ɷ�����mag����λ��ph�ָ�ʵ��planes[0]���鲿planes[1]
	merge(planes, 2, complexImg);//��ʵ���鲿�ϲ�


	//-----------------------����Ҷ����任-----------------------------------
	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
	//����Ҷ��任
	idft(complexImg, ifft, DFT_REAL_OUTPUT);
	normalize(ifft, ifft, 0, 1, CV_MINMAX);

	Rect rect(0, 0, src.cols, src.rows);
	dst = ifft(rect);
//	imshow("��ԭ���ͼ", dst);
	dst = dst * 255;
//	imwrite("��ԭ���ͼ.jpg", dst);
	//waitKey();

	return dst;
}


/////////////////////////////////����Ƶ��//////////////////////////////////////////////
Mat proceFre(Mat & src, int flag)
{
	if (flag == 0)
	{
		////������Ƶ��Ϣ
		//circle(src, Point(0, src.rows), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(src.cols, 0), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(0, 0), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//circle(src, Point(src.cols, src.rows), min(src.cols, src.rows) / 1.5, Scalar(0), -1);
		//������Ƶ��Ϣ
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
		//������Ƶ��Ϣ
		circle(src, Point(src.cols / 2, src.rows / 2), min(src.cols, src.rows) / 50, Scalar(0), -1);
	}

	return src;
}

void resizeSyetem(Mat & src, Mat & dst)
{
	int nWidth, nHeight;
	nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	printf("��ǰ��Ļ�ķֱ���Ϊ��%d��%d", nWidth, nHeight);

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

