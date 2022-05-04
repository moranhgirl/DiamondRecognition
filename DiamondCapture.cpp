/*
 * DiamondCapture.cpp
 *
 *  Coded on: 2017 年 11 月 07 日
 *      Author: Moran Huang
 */

//添加库函数
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stack>
#include "VideoProcessor.h"

//调用 C++ 工作空间
using namespace cv;
using namespace std;

//计算图形重心坐标子函数
void calc_weight(int P)
{
	char filename[1000];
	sprintf(filename, "%d.JPG", P);
	IplImage *img1 = cvLoadImage(filename, 0);
	cvNamedWindow("Orig");
	cvShowImage("Orig", img1);

	// 计算重心
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(img1, moments, 1);   // The actual moment values
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetSpatialMoment(moments, 0, 0);
	double posX = moment10 / area;
	double posY = moment01 / area;
	//显示并绘制重心
	CvPoint center; center.x = int(posX); center.y = int(posY);
	cout << "x = " << center.x << "; y = " << center.y << endl;

	cvCircle(img1, center, 3, CV_RGB(120, 150, 100), 1, 8, 0);
	cvNamedWindow("result");
	cvShowImage("result", img1);
}


//填充封闭图形子函数
void chao_fillHole(const cv::Mat srcimage, cv::Mat &dstimage)
{
    Size m_Size = srcimage.size();
    Mat temimage = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//延展图像
    srcimage.copyTo(temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
    floodFill(temimage, Point(0, 0), Scalar(255));
    Mat cutImg;       //裁剪延展的图像
    temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
    dstimage = srcimage | (~cutImg);
}

// 对视频的每帧进行滤波、提取处理子函数
void imgprocess(cv::Mat& img, cv::Mat& out, int P)
{
	Mat img1, img2, img3, img4;
	cv::cvtColor(img, img1, CV_BGR2GRAY);            	//对图像进行灰度处理
	medianBlur(img1, img2, 7);          //对图像进行中值滤波
	                                             //int Gavg = GlevelAvg(img2);
	                                                     //threshold(img2, out, Gavg, 255, CV_THRESH_BINARY);
	threshold(img2, img3, 210, 255, CV_THRESH_BINARY);   //210  为灰度阈值，此处为手动设定，请改为自动

	/* 对图形进行面积计算并输出 */
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img3, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		if(i == 1 && g_dConArea < 7280 && g_dConArea > 7000)
		{
			cout << "当前帧数为" << P++ << endl;
			cout << "【菱形面积为：】" << g_dConArea << endl;
			//calc_weight(P);
			string s = to_string(P) + ".jpg";
			imwrite(s, img3);
			calc_weight(P);
		}
	}

	chao_fillHole(img3, out);        //对图像进行封闭图形填充
}

//主函数
int main()
{
        // 定义一个视频处理类处理视频帧
        // 首先创建实例
        VideoProcessor processor;
        // 打开视频文件
        processor.setInput("D:/testa.mp4");
        // 声明显示窗口
        // 分别为输入和输出视频
        processor.displayInput("Input Video");
        processor.displayOutput("Output Video");
        // 以原始帧率播放视频
        processor.setDelay(1000./processor.getFrameRate());
        // 设置处理回调函数
        processor.setFrameProcessor(imgprocess);
        // 开始帧处理过程
        processor.run();
        cv::waitKey();
}



