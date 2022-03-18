#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <vector>
#include <mutex>
#include <future>
#include "streamer.hpp"

class FaceDetector
{
	cv::Ptr<cv::cuda::CascadeClassifier> cascade;
	cv::cuda::GpuMat d_frame, d_gray, d_found;
	cv::Mat frame;
	std::vector<cv::Rect> h_found;
	std::mutex mMutex;
	bool flag = true;
	static bool runFlag;
	std::future<void> fut;
        //Streamer mStreamer(int width = 800, int height = 480, int fps = 30);
	Streamer* mStreamer;
	public:

	FaceDetector();
	~FaceDetector();
	void setFrame(cv::Mat frame);
	std::vector<cv::Rect> getRect();
	static void interrupt(int);
	void loop();
};
