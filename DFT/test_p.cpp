////���ܣ��������ø���Ҷ�任ȥ�������е�����

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
//	//Mat src = imread("C:/Users/chenkj/Desktop/����ӡ��/person.jpg");
//	Mat src = imread("C:/Users/chenkj/Desktop/����ӡ��/img_3534.tif"); 
//
//		//int nWidth, nHeight;
//		//nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
//		//nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
//		//printf("��ǰ��Ļ�ķֱ���Ϊ��%d��%d", nWidth, nHeight);
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
//	cvtColor(src, srcGray, CV_RGB2GRAY); //�Ҷ�ͼ��������Ҷ�任
//	imwrite("�Ҷ�ͼ.jpg", srcGray);
//
//	int m = getOptimalDFTSize(srcGray.rows); //2,3,5�ı����и���Ч�ʵĸ���Ҷ�任
//	int n = getOptimalDFTSize(srcGray.cols);
//	cout << m << endl;
//	cout << n << endl;
//	Mat padded;
//	//�ѻҶ�ͼ��������Ͻ�,���ұߺ��±���չͼ��,��չ�������Ϊ0;
//	copyMakeBorder(srcGray, padded, 0, m - srcGray.rows, 0, n - srcGray.cols, BORDER_CONSTANT, Scalar::all(0));
//	cout << padded.size() << endl;
//	//�����ǻ�ȡ������Mat,һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿,��ʼ��ʱ��,ʵ������ͼ����,�鲿ȫΪ��
//	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };//����������������Mat���󣬷ֱ���ʵ�����鲿
//	Mat planes_true = Mat_<float>(padded);
//	Mat ph = Mat_<float>(padded);
//
//	Mat complexImg;
//	//��������ͨ����mat�ںϳ�һ����ͨ����mat,�����ںϵ�complexImg����ʵ�������鲿
//	merge(planes, 2, complexImg);
//
//	//���ϱߺϳɵ�mat���и���Ҷ�任,***֧��ԭ�ز���***,����Ҷ�任���Ϊ����.ͨ��1�����ʵ��,ͨ����������鲿
//	dft(complexImg, complexImg);
//	//�ѱ任��Ľ���ָ����mat,һ��ʵ��,һ���鲿,�����������
//	split(complexImg, planes);
//	//��һ������Ϊ�˼���dft�任��ķ�ֵ������Ҷ�任�ķ���ֵ��Χ�󵽲��ʺ�����Ļ����ʾ����ֵ����Ļ����ʾΪ�׵㣬����ֵΪ�ڵ㣬�ߵ�ֵ�ı仯�޷���Ч�ֱ档Ϊ������Ļ��͹�Գ��ߵͱ仯�������ԣ����ǿ����ö����߶����滻���Գ߶�,�Ա�����ʾ��ֵ,���㹫ʽ����:
//	//=> log(1 + sqrt(Re(DFT(I))^2 +Im(DFT(I))^2))
//	//����ʵ���������ķ�ֵ�����ѷ�ֵ���浽palnes[0],�������߸���
//	magnitude(planes[0], planes[1], planes_true);//������mag
//	phase(planes[0], planes[1], ph);//��λ��ph
//	Mat A = planes[0];
//	Mat B = planes[1];
//	Mat mag = planes_true;
//
//
//	//�Է�ֵ��1
//	mag += Scalar::all(1);
//	log(mag, mag);//������ķ�ֵһ��ܴ󣬴ﵽ10^4,ͨ��û�а취��ͼ������ʾ��������Ҫ�������log��⡣
//	
//	double maxVal;
//	minMaxLoc(mag, 0, &maxVal, 0, 0);
//
//	//processFre(mag, planes);
//	//--------------------------------------------------------------------------------
//	//mag = proceFre(mag);
//	//imshow("�����Ƶ��", mag);
//	//polarToCart(mag, ph, planes[0], planes[1]);//�ɷ�����mag����λ��ph�ָ�ʵ��planes[0]���鲿planes[1]
//	//merge(planes, 2, complexImg);
//
//	//crop the spectrum, if it has an odd number of rows or columns
//	//�޼�Ƶ��,���ͼ����л������������Ļ�,����Ƶ���ǲ��ԳƵ�,���Ҫ�޼�
//	//����ΪʲôҪ��  &-2����������һ��ڴ������� ע2 ˵��
//	//����֪��x&-2����x�� - 2��λ���룬�� - 2�Ķ�������ʽ��2�Ķ�����ȡ����һ�Ľ�������ǲ�������⣩��2 �Ķ����ƽ���ǣ�����
//	//8λ��ʾ��ʵ��������32λ������������ʽ��һ���ģ�Ϊ������������8λ��ʾ��0000 0010���� - 2�Ķ�������ʽΪ��1111 1110��
//	//x�� - 2��λ����󣬲���x����������ż�������x������һ��ż���� 
//	//����˵dft���������Ȼ��������mat�ĳߴ粻��Ҫ�󣬵���������������������Էֽ�Ϊ2��3��5�ĳ˻�����ô����dft������ٶȻ�ӿ�ܶࡣ
//	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
//	Mat _magI = mag.clone();
//	//��һ����Ŀ����Ȼ��Ϊ����ʾ,���Ƿ���ֵ��Ȼ��������ʾ��Χ[0,1],����ʹ�� normalize() ���������ȹ�һ��������ʾ��Χ��
//	normalize(_magI, _magI, 0, 1, CV_MINMAX);
//	
//	//namedWindow("before rearrange", 0);
//	imshow("before rearrange", _magI);
//
//	//_magI = proceFre(_magI);
//	//imshow("�����Ƶ��", _magI);
//	////planes[0] = sqrt(pow(mag, 2). / (1 + pow(tan(ph), 2)));
//	////planes[1] = planes[0].mul(tan(ph));
//	//merge(planes, 2, complexImg);
//
//	//rearrange the quadrants of Fourier image
//	//so that the origin is at the image center
//	//���·������ޣ�ʹ��0,0���ƶ���ͼ�����ģ�  
//	//�ڡ�����ͼ�����У�����Ҷ�任֮ǰҪ��Դͼ����ԣ�-1��^(x+y)�������Ļ���  
//	//�����ǶԸ���Ҷ�任����������Ļ�
//	/*----------��ʾ����Ƶ��ͼ--------------------*/
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
//
//	normalize(mag, mag, 0, 1, CV_MINMAX);
//	//namedWindow("Input Image", 0);
//	//namedWindow("spectrum magnitude", 0);
//	imshow("ԭͼ�Ҷ�ͼ", srcGray);
//	imshow("Ƶ�׷���", mag);
//	mag = mag * 255;
//	imwrite("Ƶ�׷���.jpg", mag);
//	/*--------------------------------------------------*/
//
//	mag = mag / 255;
//	mag = proceFre(mag);
//	//imshow("�����Ƶ��", mag);
//
//	//������������Ϊ��׼����magͼ��ֳ��Ĳ���
//	
//    Mat q00(mag, Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
//	Mat q10(mag, Rect(cx, 0, cx, cy));  //Top-Right
//	Mat q20(mag, Rect(0, cy, cx, cy));  //Bottom-Left
//	Mat q30(mag, Rect(cx, cy, cx, cy)); //Bottom-Right
//    //swap quadrants(Top-Left with Bottom-Right)����������
//	q00.copyTo(tmp);
//	q30.copyTo(q00);
//	tmp.copyTo(q30);
//	// swap quadrant (Top-Rightwith Bottom-Left)����������
//	q10.copyTo(tmp);
//	q20.copyTo(q10);
//	tmp.copyTo(q20);
//
//	imshow("����任", mag);
//
//	mag = mag * maxVal;
//	exp(mag, mag);
//	mag = mag - Scalar::all(1);
//	polarToCart(mag, ph, planes[0], planes[1]);//�ɷ�����mag����λ��ph�ָ�ʵ��planes[0]���鲿planes[1]
//	merge(planes, 2, complexImg);
//
//
//	//Mat div;
//	//divide(mag.mul(mag), 1 + atan2(ph).mul(atan2(ph)), div);
//	//planes[0] = sqrt(div);
//	//planes[1] = planes[0].mul(tan(ph));
//
//	//-----------------------����Ҷ����任-----------------------------------
//	Mat ifft(Size(src.cols, src.rows), CV_8UC1);
//	//����Ҷ��任
//	idft(complexImg, ifft, DFT_REAL_OUTPUT);
//	normalize(ifft, ifft, 0, 1, CV_MINMAX);
//	
//	Rect rect(0, 0, src.cols, src.rows);
//	Mat ifft2 = ifft(rect);
//	//namedWindow("inverse fft", 0);
//	imshow("��任���ͼ", ifft2);
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
//
//
