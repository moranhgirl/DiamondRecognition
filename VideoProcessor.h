/*
 * VideoProssor.h
 *
 *  Coded on: 2017 年 11 月 07 日
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
      // 创建视频捕获对象
      cv::VideoCapture capture;
      // 每帧调用的回调函数
      void (*process)(cv::Mat&, cv::Mat&, int P);
      // FrameProcessor接口
      FrameProcessor *frameProcessor;
      // 确定是否调用回调函数的bool信号
      bool callIt;
      // 输入窗口的名称
      std::string windowNameInput;
      // 输出窗口的名称
      std::string windowNameOutput;
      // 延迟
      int delay;
      // 已处理的帧数
      long fnumber;
      // 在该帧停止
      long frameToStop;
      // 是否停止处理
      bool stop;

      // 当输入图像序列存储在不同文件中时，可使用以下设置
      // 把图像文件名的数组作为输入
      std::vector<std::string> images;
      // 图像向量的迭加器
      std::vector<std::string>::const_iterator itImg;

      // 得到下一帧
      // 可能来自：视频文件或摄像头
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

      // 默认设置 digits(0), frameToStop(-1),
      VideoProcessor() : process(0), frameProcessor(0),
	  callIt(false), delay(-1), fnumber(0), frameToStop(0), stop(false) {}

      // 创建输入窗口
      void displayInput(std::string wt);
      // 创建输出窗口
      void displayOutput(std::string wn);
      // 不再显示处理后的帧
      void dontDisplay();

      // 以下三个函数设置输入的图像向量
      bool setInput(std::string filename);
      // 若输入为摄像头，设置ID
      bool setInput(int id);
      // 若输入为一组图像序列时，应用该函数
      bool setInput(const std::vector<std::string>& imgs);

      // 设置帧之间的延迟
      // 0意味着在每一帧都等待按键响应
      // 负数意味着没有延迟
      void setDelay(int d);

      // 返回图像的帧率
      double getFrameRate();

      // 需要调用回调函数
      void callProcess();

      // 不需要调用回调函数
      void dontCallProcess();

      // 设置FrameProcessor实例
      void setFrameProcessor(FrameProcessor* frameProcessorPtr);

      // 设置回调函数
      void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&, int));

      // 停止运行
      void stopIt();

      // 判断是否已经停止
      bool isStopped();

      // 是否开始了捕获设备？
      bool isOpened();

      // 返回下一帧的帧数
      long getFrameNumber();

      // 该函数获取并处理视频帧
      void run();

};

#endif /* VIDEOPROCESSOR_H_ */
