#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include "streamer.hpp"
#include <thread>
#include <chrono>         // std::chrono::seconds
#include <mutex>


Streamer::Streamer(int _width, int _height, int _fps):width(_width),height(_height),fps(_fps)
{
                       pipeline = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" + std::to_string(height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(fps) + "/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

                cap.open(pipeline, cv::CAP_GSTREAMER);

}

void Streamer::returnFrame(cv::Mat& frame)
{
	if( !cap.read(frame))
		std::cout<<"Can not read the image"<<std::endl;

}

