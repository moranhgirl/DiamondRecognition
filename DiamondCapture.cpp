/*
 * DiamondCapture.cpp
 *
 *  Coded on: 2017 �� 11 �� 07 ��
 *      Author: Moran Huang
 */

//��ӿ⺯��
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stack>
#include "VideoProcessor.h"

//���� C++ �����ռ�
using namespace cv;
using namespace std;

//����ͼ�����������Ӻ���
void calc_weight(int P)
{
	char filename[1000];
	sprintf(filename, "%d.JPG", P);
	IplImage *img1 = cvLoadImage(filename, 0);
	cvNamedWindow("Orig");
	cvShowImage("Orig", img1);

	// ��������
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(img1, moments, 1);   // The actual moment values
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetSpatialMoment(moments, 0, 0);
	double posX = moment10 / area;
	double posY = moment01 / area;
	//��ʾ����������
	CvPoint center; center.x = int(posX); center.y = int(posY);
	cout << "x = " << center.x << "; y = " << center.y << endl;

	cvCircle(img1, center, 3, CV_RGB(120, 150, 100), 1, 8, 0);
	cvNamedWindow("result");
	cvShowImage("result", img1);
}


//�����ͼ���Ӻ���
void chao_fillHole(const cv::Mat srcimage, cv::Mat &dstimage)
{
    Size m_Size = srcimage.size();
    Mat temimage = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//��չͼ��
    srcimage.copyTo(temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
    floodFill(temimage, Point(0, 0), Scalar(255));
    Mat cutImg;       //�ü���չ��ͼ��
    temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
    dstimage = srcimage | (~cutImg);
}

// ����Ƶ��ÿ֡�����˲�����ȡ�����Ӻ���
void imgprocess(cv::Mat& img, cv::Mat& out, int P)
{
	Mat img1, img2, img3, img4;
	cv::cvtColor(img, img1, CV_BGR2GRAY);            	//��ͼ����лҶȴ���
	medianBlur(img1, img2, 7);          //��ͼ�������ֵ�˲�
	                                             //int Gavg = GlevelAvg(img2);
	                                                     //threshold(img2, out, Gavg, 255, CV_THRESH_BINARY);
	threshold(img2, img3, 210, 255, CV_THRESH_BINARY);   //210  Ϊ�Ҷ���ֵ���˴�Ϊ�ֶ��趨�����Ϊ�Զ�

	/* ��ͼ�ν���������㲢��� */
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img3, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		if(i == 1 && g_dConArea < 7280 && g_dConArea > 7000)
		{
			cout << "��ǰ֡��Ϊ" << P++ << endl;
			cout << "���������Ϊ����" << g_dConArea << endl;
			//calc_weight(P);
			string s = to_string(P) + ".jpg";
			imwrite(s, img3);
			calc_weight(P);
		}
	}

	chao_fillHole(img3, out);        //��ͼ����з��ͼ�����
}

//������
int main()
{
        // ����һ����Ƶ�����ദ����Ƶ֡
        // ���ȴ���ʵ��
        VideoProcessor processor;
        // ����Ƶ�ļ�
        processor.setInput("D:/testa.mp4");
        // ������ʾ����
        // �ֱ�Ϊ����������Ƶ
        processor.displayInput("Input Video");
        processor.displayOutput("Output Video");
        // ��ԭʼ֡�ʲ�����Ƶ
        processor.setDelay(1000./processor.getFrameRate());
        // ���ô���ص�����
        processor.setFrameProcessor(imgprocess);
        // ��ʼ֡�������
        processor.run();
        cv::waitKey();
}



