#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include "face_detector_cuda.hpp"
#include <thread>
#include <chrono>         // std::chrono::seconds
#include <mutex>

FaceDetector::FaceDetector()
{
	cascade = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
	
	std::thread t([this](){
	while(1)
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
   	t.detach();

}


void FaceDetector::setFrame(Mat matFrame)
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
