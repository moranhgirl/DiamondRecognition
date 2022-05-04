/*
 * FrameProcessor.h
 *
 *  Coded on: 2017 �� 11 �� 07 ��
 *      Author: Moran Huang
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class FrameProcessor
{
public:
    virtual void process(cv:: Mat &input, cv:: Mat &output) = 0;
};


#endif /* FRAMEPROCESSOR_H_ */
