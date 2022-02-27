#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <vector>
#include <mutex>
#include <future>

class FaceDetector
{
	cv::Ptr<cv::cuda::CascadeClassifier> cascade;
	cv::cuda::GpuMat d_frame, d_gray, d_found;
	cv::Mat frame;
	std::vector<cv::Rect> h_found;
	std::mutex mMutex;
	bool flag = true;
	std::future<void> fut;

	public:

	FaceDetector();
	~FaceDetector();
	void setFrame(cv::Mat frame);
	std::vector<cv::Rect> getRect();
};
