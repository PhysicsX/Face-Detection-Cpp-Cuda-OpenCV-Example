#include "face_detector_cuda.hpp"

bool FaceDetector::runFlag = true;

FaceDetector::FaceDetector():mStreamer(Streamer(800, 480, 30))
{
	std::signal(SIGINT, interrupt);
	cascade = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
	
	fut = std::async(std::launch::async, [&](){
		while(runFlag)
		{	
			if(frame.empty() == false)
			{
			cv::cuda::GpuMat d_frame, d_gray, d_found;
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
	fut.wait();
}

void FaceDetector::interrupt(int sign)
{
	runFlag = false;
}

void FaceDetector::setFrame(cv::Mat matFrame)
{
	std::lock_guard<std::mutex> guard(mMutex);
	frame=matFrame;
}

std::vector<cv::Rect> FaceDetector::getRect() const
{
	std::lock_guard<std::mutex> guard(mMutex);	
	return h_found;
}

void FaceDetector::loop()
{
	while(runFlag){
	cv::Mat frame;
	
	mStreamer.returnFrame(frame);
	
	setFrame(frame);

	//for(auto s : getRect())
	
	for(int i{0}; i < getRect().size(); i++)
	{
		cv::rectangle(frame, getRect()[i], cv::Scalar(0,255,0), 3);	
		//std::cout<<"height "<<s.height<<" width "<<s.width<<" x "<<s.x<<" y "<<s.y<<std::endl;
		std::cout<<i<<" height "<<getRect()[i].height<<" width "<<getRect()[i].width<<" x "<<getRect()[i].x<<" y "<<getRect()[i].y<<std::endl;
	}

	cvNamedWindow("FaceDetection", CV_WINDOW_NORMAL);
	cvSetWindowProperty("FaceDetection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	imshow("FaceDetection", frame);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (cv::waitKey(1) == 'q') {
            	runFlag = false;
		break;
        }
	}
}
