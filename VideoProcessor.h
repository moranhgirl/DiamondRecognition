/*
 * VideoProssor.h
 *
 *  Coded on: 2017 �� 11 �� 07 ��
 *      Author: Moran Huang
 */

#ifndef VIDEOPROCESSOR_H_
#define VIDEOPROCESSOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "FrameProcessor.h"

class VideoProcessor
{
  private:
      // ������Ƶ�������
      cv::VideoCapture capture;
      // ÿ֡���õĻص�����
      void (*process)(cv::Mat&, cv::Mat&, int P);
      // FrameProcessor�ӿ�
      FrameProcessor *frameProcessor;
      // ȷ���Ƿ���ûص�������bool�ź�
      bool callIt;
      // ���봰�ڵ�����
      std::string windowNameInput;
      // ������ڵ�����
      std::string windowNameOutput;
      // �ӳ�
      int delay;
      // �Ѵ����֡��
      long fnumber;
      // �ڸ�ֹ֡ͣ
      long frameToStop;
      // �Ƿ�ֹͣ����
      bool stop;

      // ������ͼ�����д洢�ڲ�ͬ�ļ���ʱ����ʹ����������
      // ��ͼ���ļ�����������Ϊ����
      std::vector<std::string> images;
      // ͼ�������ĵ�����
      std::vector<std::string>::const_iterator itImg;

      // �õ���һ֡
      // �������ԣ���Ƶ�ļ�������ͷ
      bool readNextFrame(cv::Mat &frame)
      {
          if (images.size()==0)
              return capture.read(frame);
          else
          {
              if (itImg != images.end())
              {
                  frame= cv::imread(*itImg);
                  itImg++;
                  return frame.data != 0;
              }
          }
       }

public:

      // Ĭ������ digits(0), frameToStop(-1),
      VideoProcessor() : process(0), frameProcessor(0),
	  callIt(false), delay(-1), fnumber(0), frameToStop(0), stop(false) {}

      // �������봰��
      void displayInput(std::string wt);
      // �����������
      void displayOutput(std::string wn);
      // ������ʾ������֡
      void dontDisplay();

      // ���������������������ͼ������
      bool setInput(std::string filename);
      // ������Ϊ����ͷ������ID
      bool setInput(int id);
      // ������Ϊһ��ͼ������ʱ��Ӧ�øú���
      bool setInput(const std::vector<std::string>& imgs);

      // ����֮֡����ӳ�
      // 0��ζ����ÿһ֡���ȴ�������Ӧ
      // ������ζ��û���ӳ�
      void setDelay(int d);

      // ����ͼ���֡��
      double getFrameRate();

      // ��Ҫ���ûص�����
      void callProcess();

      // ����Ҫ���ûص�����
      void dontCallProcess();

      // ����FrameProcessorʵ��
      void setFrameProcessor(FrameProcessor* frameProcessorPtr);

      // ���ûص�����
      void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&, int));

      // ֹͣ����
      void stopIt();

      // �ж��Ƿ��Ѿ�ֹͣ
      bool isStopped();

      // �Ƿ�ʼ�˲����豸��
      bool isOpened();

      // ������һ֡��֡��
      long getFrameNumber();

      // �ú�����ȡ��������Ƶ֡
      void run();

};

#endif /* VIDEOPROCESSOR_H_ */
