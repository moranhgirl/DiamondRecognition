/*
 * FrameProssor.cpp
 *
 *  Coded on: 2017 �� 11 �� 07 ��
 *      Author: Moran Huang
 */

#include "VideoProcessor.h"

using namespace std;

// �������봰��
void VideoProcessor::displayInput(std::string wt)
{
    windowNameInput = wt;
    cv::namedWindow(windowNameInput);
}

// �����������
void VideoProcessor::displayOutput(std::string wn)
{
    windowNameOutput = wn;
    cv::namedWindow(windowNameOutput);
}

// ������ʾ������֡
void VideoProcessor::dontDisplay()
{
    cv::destroyWindow(windowNameInput);
    cv::destroyWindow(windowNameOutput);
    windowNameInput.clear();
    windowNameOutput.clear();
}

// ���������ͼ������
bool VideoProcessor::setInput(std::string filename)
{
  fnumber = 0;
  // �ͷ�֮ǰ�򿪹�����Ƶ��Դ
  capture.release();
  images.clear();

  // ����Ƶ
  return capture.open(filename);
}

// ������Ϊ����ͷ������ID
bool VideoProcessor::setInput(int id)
{
  fnumber= 0;
  // �ͷ�֮ǰ�򿪹�����Ƶ��Դ
  capture.release();
  images.clear();

  // ����Ƶ�ļ�
  return capture.open(id);
}

// ������Ϊһ��ͼ������ʱ��Ӧ�øú���
bool VideoProcessor::setInput(const std::vector<std::string>& imgs)
{
  fnumber= 0;
  // �ͷ�֮ǰ�򿪹�����Ƶ��Դ
  capture.release();

  // ���뽫�Ǹ�ͼ�������
  images = imgs;
  itImg = images.begin();

  return true;
}

// ����֮֡����ӳ�
// 0��ζ����ÿһ֡���ȴ�������Ӧ
// ������ζ��û���ӳ�
void VideoProcessor::setDelay(int d)
{
    delay = d;
}

// ����ͼ���֡��
double VideoProcessor::getFrameRate()
{
    if (images.size() != 0) return 0;
    double r = capture.get(CV_CAP_PROP_FPS);
    return r;
}

// ��Ҫ���ûص�����
void VideoProcessor::callProcess()
{
    callIt = true;
}

// ����Ҫ���ûص�����
void VideoProcessor::dontCallProcess()
{
    callIt = false;
}

// ����FrameProcessorʵ��
void VideoProcessor::setFrameProcessor(FrameProcessor* frameProcessorPtr)
{
    // ʹ�ص�������Ч��
    process = 0;
    // ��������FrameProcessorʵ��
    frameProcessor = frameProcessorPtr;
    callProcess();
}

// ���ûص�����
void VideoProcessor::setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&, int))
{
    // ʹFrameProcessorʵ����Ч��
    frameProcessor = 0;
    // �������ûص�����
    process = frameProcessingCallback;
    callProcess();
}

// ���º�����ʾ��Ƶ�Ķ�ȡ״̬
// ֹͣ����
void VideoProcessor::stopIt()
{
    stop = true;
}

// �ж��Ƿ��Ѿ�ֹͣ
bool VideoProcessor::isStopped()
{
    return stop;
}

// �Ƿ�ʼ�˲����豸��
bool VideoProcessor::isOpened()
{
    return capture.isOpened() || !images.empty();
}

// ������һ֡��֡��
long VideoProcessor::getFrameNumber()
{
  if (images.size() == 0)
  {
      // �õ������豸����Ϣ
      long f = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
      return f;

  }
  else // ����������һ��ͼ������ʱ�����
  {
      return static_cast<long>(itImg-images.begin());
  }
}

// �ú�����ȡ��������Ƶ֡
void VideoProcessor::run()
{
    // ��ǰ֡
    cv::Mat frame;
    // ���֡
    cv::Mat output;

    // ��ʧ��ʱ
    if (!isOpened())
    {
        cout << "Error!" << endl;
        return;
    }
    stop= false;
    while (!isStopped())
    {
        // ��ȡ��һ֡
        if (!readNextFrame(frame))
            break;
        // ��ʾ���֡
        if (windowNameInput.length() != 0)
            cv::imshow(windowNameInput, frame);
        // ���ô�����
        if (callIt)
        {
          // ����ǰ֡
          if (process)
          {
        	  long int FrameNum = getFrameNumber();
        	  process(frame, output, FrameNum);
          }
          else if (frameProcessor)
              frameProcessor -> process(frame, output);
          // ����֡��
          fnumber++;
        }
        else
        {
          output = frame;
        }
        // ��ʾ���֡
        if (windowNameOutput.length()!=0)
            cv::imshow(windowNameOutput,output);
        // �����ӳ�
        if (delay >= 0 && cv::waitKey(delay) >= 0)
          stopIt();
        // ����Ƿ���Ҫֹͣ����
        if (frameToStop >= 0 && getFrameNumber() == frameToStop)
            stopIt();
    }
}



