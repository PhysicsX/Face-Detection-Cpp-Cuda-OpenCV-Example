#ifndef STREAMER_H
#define STREAMER_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>


class Streamer
{

		std::string pipeline;
		int width, height, fps;
		cv::VideoCapture cap;

	public:
		
		Streamer(int, int, int, int);
		void returnFrame(cv::Mat&);
};

#endif
