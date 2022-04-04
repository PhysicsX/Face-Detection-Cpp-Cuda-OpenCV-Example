#include "face_detector_cuda.hpp"

bool FaceDetector::runFlag = true;

FaceDetector::FaceDetector():mStreamer(Streamer(0, 800, 480, 30))
{
	std::signal(SIGINT, interrupt);
	
	mFrameQueue.processQueue();
}

FaceDetector::~FaceDetector()
{
}

void FaceDetector::interrupt(int sign)
{
	runFlag = false;
}



void FaceDetector::loop()
{
	while(runFlag){
	cv::Mat frame;
	
	mStreamer.returnFrame(frame);
	
	cv::Mat tmp = frame;
	mFrameQueue.addQueue(tmp);
	
	auto landMarksTmp = mFrameQueue.returnLand();
	
	if(!landMarksTmp.empty())
		landMarks=landMarksTmp;

	for(int i{0}; i < landMarks.size(); i++)
	{
		cv::rectangle(frame, landMarks[i], cv::Scalar(0,255,0), 3);	
		std::cout<<i<<" height "<<landMarks[i].height<<" width "<<landMarks[i].width<<" x "<<landMarks[i].x<<" y "<<landMarks[i].y<<std::endl;
	}

	cvNamedWindow("FaceDetection", CV_WINDOW_NORMAL);
	cvSetWindowProperty("FaceDetection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	imshow("FaceDetection", frame);
       	if (cv::waitKey(1) == 'q') {
            	runFlag = false;
		break;
        }
	}
}
