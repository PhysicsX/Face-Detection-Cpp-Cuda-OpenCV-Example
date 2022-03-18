#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include "face_detector_cuda.hpp"
#include <thread>
#include <chrono>         // std::chrono::seconds
#include <mutex>
#include <thread>
#include <chrono>
#include <csignal>

bool FaceDetector::runFlag = true;

FaceDetector::FaceDetector()
{
	std::signal(SIGINT, interrupt);
	mStreamer = new Streamer(800, 480, 30);
	cascade = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
	fut = std::async(std::launch::async, [&](){
	while(flag)
	{	
		if(frame.empty() == false)
		{
			{
				std::lock_guard<std::mutex> guard(mMutex);	
				d_frame.upload(frame);
			}

			cv::cuda::cvtColor(d_frame, d_gray, cv::COLOR_BGR2GRAY);
			cascade->detectMultiScale(d_gray, d_found);
			
			{
				std::lock_guard<std::mutex> guard(mMutex);
				cascade->convert(d_found, h_found);    	
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	});
}

FaceDetector::~FaceDetector()
{
	flag = false;
	fut.wait();
	delete mStreamer;
}

void FaceDetector::interrupt(int sign)
{
	runFlag = false;
}

void FaceDetector::setFrame(cv::Mat matFrame)
{
	std::lock_guard<std::mutex> guard(mMutex);
	{
		frame=matFrame;
	}
}

std::vector<cv::Rect> FaceDetector::getRect()
{
	std::lock_guard<std::mutex> guard(mMutex);	
	return h_found;
}

void FaceDetector::loop()
{
	while(true){
	cv::Mat frame;
	
	mStreamer->returnFrame(frame);
	
	setFrame(frame);

	for(auto s : getRect())
	{
		cv::rectangle(frame, s, cv::Scalar(0,255,0), 3);	
		std::cout<<"height "<<s.height<<" width "<<s.width<<" x "<<s.x<<" y "<<s.y<<std::endl;
	}

	cvNamedWindow("FaceDetection", CV_WINDOW_NORMAL);
	cvSetWindowProperty("FaceDetection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	imshow("FaceDetection", frame);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (cv::waitKey(1) == 'q' || runFlag == false) {
            break;
        }
	}
}
