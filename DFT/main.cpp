////���ܣ����ø���Ҷ�任ȥ�������е�����

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
//	Mat srcImage = imread("C:/Users/chenkj/Desktop/����ӡ��/img_3534.tif");
//	//Mat srcImage = imread("C:/Users/chenkj/Desktop/����ӡ��/person.jpg");
//	imshow("ԭͼ", srcImage);
//	Mat dstImage;
//	Mat srcChannels[3];//���ԭͼ����ͨ��
//	Mat dstChannels[3];//���ԭͼ����ͨ��
//
//	split(srcImage, srcChannels);
//
//	for (size_t i = 0; i < 3; i++)
//	{
//		removeGridding(srcChannels[i], dstChannels[i]);
//	}
//
//	merge(dstChannels, 3, dstImage);
//	imshow("��ԭ���ͼ", dstImage/255);
//	imwrite("��ɫͼ������.jpg", dstImage);
//
//	waitKey(0);
//	return 0;
//}
//
///*//////////////////////////////////ȥ������///////////////////////////////////////////////////////////////////
//���ܣ�ȥ��ͼ���е�����
//���룺��ͨ���Ҷ�ͼ��һ��Ϊ��ɫͼ�ĸ���ͨ����
//�����������ɵ�ͼ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//Mat removeGridding(Mat & src, Mat & dst)
//{
//	int m = getOptimalDFTSize(src.rows); //2,3,5�ı����и���Ч�ʵĸ���Ҷ�任
//	int n = getOptimalDFTSize(src.cols);
//	Mat padded;
//	//�ѻҶ�ͼ��������Ͻ�,���ұߺ��±���չͼ��,��չ�������Ϊ0;
//	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
//	//planes[0]Ϊdft�任��ʵ����planes[1]Ϊ�鲿��phΪ��λ�� plane_true=magΪ��ֵ
//	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
//	Mat planes_true = Mat_<float>(padded);
//	Mat ph = Mat_<float>(padded);
//	Mat complexImg;
//	//��ͨ��complexImg����ʵ�������鲿
//	merge(planes, 2, complexImg);
//	//���ϱߺϳɵ�mat���и���Ҷ�任,***֧��ԭ�ز���***,����Ҷ�任���Ϊ����.ͨ��1�����ʵ��,ͨ����������鲿
//	dft(complexImg, complexImg);
//	//�ѱ任��Ľ���ָ����mat,һ��ʵ��,һ���鲿,�����������
//	split(complexImg, planes);
//	
//	//---------------�˲���Ŀ��Ϊ���õ���ʾ��ֵ---�����ָ�ԭͼʱ�����ٴ���һ��-------------------------
//	magnitude(planes[0], planes[1], planes_true);//������mag
//	phase(planes[0], planes[1], ph);//��λ��ph
//	Mat A = planes[0];
//	Mat B = planes[1];
//	Mat mag = planes_true;
//
//	mag += Scalar::all(1);//�Է�ֵ��1
//	log(mag, mag);//������ķ�ֵһ��ܴ󣬴ﵽ10^4,ͨ��û�а취��ͼ������ʾ��������Ҫ�������log��⡣
//
//	//ȡ�����е����ֵ�����ں�����ԭʱȥ��һ��
//	double maxVal;
//	minMaxLoc(mag, 0, &maxVal, 0, 0);
//
//	//�޼�Ƶ��,���ͼ����л������������Ļ�,����Ƶ���ǲ��ԳƵ�,���Ҫ�޼�
//	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
//	Mat _magI = mag.clone();
//	//�����ȹ�һ��������ʾ��Χ��
//	normalize(_magI, _magI, 0, 1, CV_MINMAX);
////	imshow("before rearrange", _magI);
//
//	//��ʾ����Ƶ��ͼ
//	int cx = mag.cols / 2;
//	int cy = mag.rows / 2;
//
//	//������������Ϊ��׼����magͼ��ֳ��Ĳ���
//	Mat tmp;
//	Mat q0(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q1(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q2(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q3(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//
//	//swap quadrants(Top-Left with Bottom-Right)����������
//	q0.copyTo(tmp);
//	q3.copyTo(q0);
//	tmp.copyTo(q3);
//
//	// swap quadrant (Top-Rightwith Bottom-Left)����������
//	q1.copyTo(tmp);
//	q2.copyTo(q1);
//	tmp.copyTo(q2);
//
//	normalize(mag, mag, 0, 1, CV_MINMAX);
////	imshow("ԭͼ�Ҷ�ͼ", src);
////	imshow("Ƶ�׷���", mag);
//	mag = mag * 255;
////	imwrite("Ƶ�׷���.jpg", mag);
//	/*--------------------------------------------------*/
//
//	mag = mag / 255;
//	mag = proceFre(mag); // ��������Ƶ��
//	//imshow("�����Ƶ��", mag);
//
//	//ǰ�����跴����һ�飬Ŀ����Ϊ����任��ԭͼ
//	Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//    //��������
//	q00.copyTo(tmp);
//	q30.copyTo(q00);
//	tmp.copyTo(q30);
//	q10.copyTo(tmp);
//	q20.copyTo(q10);
//	tmp.copyTo(q20);
//
//	mag = mag * maxVal;//����һ���ľ���ԭ 
//	exp(mag, mag);//��Ӧ��ǰ��ȥ����
//	mag = mag - Scalar::all(1);//��Ӧǰ��+1
//	polarToCart(mag, ph, planes[0], planes[1]);//�ɷ�����mag����λ��ph�ָ�ʵ��planes[0]���鲿planes[1]
//	merge(planes, 2, complexImg);//��ʵ���鲿�ϲ�
//
//
//	//-----------------------����Ҷ����任-----------------------------------
//	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
//	//����Ҷ��任
//	idft(complexImg, ifft, DFT_REAL_OUTPUT);
//	normalize(ifft, ifft, 0, 1, CV_MINMAX);
//
//	Rect rect(0, 0, src.cols, src.rows);
//	dst = ifft(rect);
////	imshow("��ԭ���ͼ", dst);
//	dst = dst * 255;
////	imwrite("��ԭ���ͼ.jpg", dst);
//	//waitKey();
//
//	return dst;
//}
//
//
///////////////////////////////////����Ƶ��//////////////////////////////////////////////
//Mat proceFre(Mat & src)
//{
//
//	//[0]���ж�ֵ��
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
//	//[1]�������Ͳ���
//	Mat erode_element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	Mat dilate_element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	dilate(image_threshold, image_dilate, dilate_element);
//	//[2]���и�ʴ����
//	erode(image_dilate, image_erode, erode_element);
//	//erode(image_threshold, image_erode, erode_element);
//	//dilate(image_erode, image_dilate, dilate_element);
//	//[3]���з�ɫ����
//	image_reverse = 1 - image_erode;
//	//[4]�������ĺ�
//	circle(image_reverse, Point(src.cols / 2, src.rows / 2), min(src.cols, src.rows) / 6, Scalar(1), -1);
//	//[5]��ֵ�˲�
//	medianBlur(image_reverse, image_median, 5);
//	//dilate(image_median, image_median, dilate_element);
//
//	//ԭƵ��ͼ���ϴ�����Ƶ��ͼ
//	dst = src.mul(image_median);
//
//	return dst;
//}
//
