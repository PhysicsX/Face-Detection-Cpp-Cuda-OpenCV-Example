#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace cv;
using namespace std;

class FaceDetector
{
	cv::Ptr<cv::cuda::CascadeClassifier> cascade;
	cv::cuda::GpuMat d_frame, d_gray, d_found;
	Mat frame;
	std::vector<cv::Rect> h_found;
	std::mutex mMutex, mMutex2;
	bool mReady = false;
	std::condition_variable mCv;
	public:

	FaceDetector();
	void setFrame(Mat frame);
	std::vector<cv::Rect> getRect();
};
