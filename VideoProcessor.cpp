/*
 * FrameProssor.cpp
 *
 *  Coded on: 2017 年 11 月 07 日
 *      Author: Moran Huang
 */

#include "VideoProcessor.h"

using namespace std;

// 创建输入窗口
void VideoProcessor::displayInput(std::string wt)
{
    windowNameInput = wt;
    cv::namedWindow(windowNameInput);
}

// 创建输出窗口
void VideoProcessor::displayOutput(std::string wn)
{
    windowNameOutput = wn;
    cv::namedWindow(windowNameOutput);
}

// 不再显示处理后的帧
void VideoProcessor::dontDisplay()
{
    cv::destroyWindow(windowNameInput);
    cv::destroyWindow(windowNameOutput);
    windowNameInput.clear();
    windowNameOutput.clear();
}

// 设置输入的图像向量
bool VideoProcessor::setInput(std::string filename)
{
  fnumber = 0;
  // 释放之前打开过的视频资源
  capture.release();
  images.clear();

  // 打开视频
  return capture.open(filename);
}

// 若输入为摄像头，设置ID
bool VideoProcessor::setInput(int id)
{
  fnumber= 0;
  // 释放之前打开过的视频资源
  capture.release();
  images.clear();

  // 打开视频文件
  return capture.open(id);
}

// 若输入为一组图像序列时，应用该函数
bool VideoProcessor::setInput(const std::vector<std::string>& imgs)
{
  fnumber= 0;
  // 释放之前打开过的视频资源
  capture.release();

  // 输入将是该图像的向量
  images = imgs;
  itImg = images.begin();

  return true;
}

// 设置帧之间的延迟
// 0意味着在每一帧都等待按键响应
// 负数意味着没有延迟
void VideoProcessor::setDelay(int d)
{
    delay = d;
}

// 返回图像的帧率
double VideoProcessor::getFrameRate()
{
    if (images.size() != 0) return 0;
    double r = capture.get(CV_CAP_PROP_FPS);
    return r;
}

// 需要调用回调函数
void VideoProcessor::callProcess()
{
    callIt = true;
}

// 不需要调用回调函数
void VideoProcessor::dontCallProcess()
{
    callIt = false;
}

// 设置FrameProcessor实例
void VideoProcessor::setFrameProcessor(FrameProcessor* frameProcessorPtr)
{
    // 使回调函数无效化
    process = 0;
    // 重新设置FrameProcessor实例
    frameProcessor = frameProcessorPtr;
    callProcess();
}

// 设置回调函数
void VideoProcessor::setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&, int))
{
    // 使FrameProcessor实例无效化
    frameProcessor = 0;
    // 重新设置回调函数
    process = frameProcessingCallback;
    callProcess();
}

// 以下函数表示视频的读取状态
// 停止运行
void VideoProcessor::stopIt()
{
    stop = true;
}

// 判断是否已经停止
bool VideoProcessor::isStopped()
{
    return stop;
}

// 是否开始了捕获设备？
bool VideoProcessor::isOpened()
{
    return capture.isOpened() || !images.empty();
}

// 返回下一帧的帧数
long VideoProcessor::getFrameNumber()
{
  if (images.size() == 0)
  {
      // 得到捕获设备的信息
      long f = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
      return f;

  }
  else // 当输入来自一组图像序列时的情况
  {
      return static_cast<long>(itImg-images.begin());
  }
}

// 该函数获取并处理视频帧
void VideoProcessor::run()
{
    // 当前帧
    cv::Mat frame;
    // 输出帧
    cv::Mat output;

    // 打开失败时
    if (!isOpened())
    {
        cout << "Error!" << endl;
        return;
    }
    stop= false;
    while (!isStopped())
    {
        // 读取下一帧
        if (!readNextFrame(frame))
            break;
        // 显示输出帧
        if (windowNameInput.length() != 0)
            cv::imshow(windowNameInput, frame);
        // 调用处理函数
        if (callIt)
        {
          // 处理当前帧
          if (process)
          {
        	  long int FrameNum = getFrameNumber();
        	  process(frame, output, FrameNum);
          }
          else if (frameProcessor)
              frameProcessor -> process(frame, output);
          // 增加帧数
          fnumber++;
        }
        else
        {
          output = frame;
        }
        // 显示输出帧
        if (windowNameOutput.length()!=0)
            cv::imshow(windowNameOutput,output);
        // 引入延迟
        if (delay >= 0 && cv::waitKey(delay) >= 0)
          stopIt();
        // 检查是否需要停止运行
        if (frameToStop >= 0 && getFrameNumber() == frameToStop)
            stopIt();
    }
}



