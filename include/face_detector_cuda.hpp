#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <vector>
#include <mutex>
#include <future>
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>
#include "streamer.hpp"

class FaceDetector
{
	cv::Ptr<cv::cuda::CascadeClassifier> cascade;
	std::vector<cv::Rect> h_found;
	cv::Mat frame;
	mutable std::mutex mMutex;
	static bool runFlag;
	std::future<void> fut;
	Streamer mStreamer;

	void setFrame(cv::Mat frame);
	static void interrupt(int);
	
	public:

	FaceDetector();
	~FaceDetector();
	std::vector<cv::Rect> getRect() const;
	void loop();
};
