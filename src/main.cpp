#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "face_detector_cuda.hpp"
#include <thread>
#include <chrono>

std::string get_tegra_pipeline(int width, int height, int fps) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" +
           std::to_string(height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(fps) +
           "/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main() {


    std::cout << "OpenCV version : " << CV_VERSION << std::endl;
    std::cout << "Major version : " << CV_MAJOR_VERSION << std::endl;
    std::cout << "Minor version : " << CV_MINOR_VERSION << std::endl;
    std::cout << "Subminor version : " << CV_SUBMINOR_VERSION << std::endl;
    
	// Options
    int WIDTH = 800;
    int HEIGHT = 480;
    int FPS = 30;

    // Define the gstream pipeline
    std::string pipeline = get_tegra_pipeline(WIDTH, HEIGHT, FPS);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";

    // Create OpenCV capture object, ensure it works.
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
	std::cout << "Can not open video source";
        return -1;
    }

    std::vector<cv::Rect> h_found;
    cv::Ptr<cv::cuda::CascadeClassifier> cascade = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
    cv::cuda::GpuMat d_frame, d_gray, d_found;

    FaceDetector faceObj;
    while(1)
    {
	cv::Mat frame;
        if ( !cap.read(frame) ) {
	    std::cout << "Can not read frame from webcam";
            return -1;
        }
	faceObj.setFrame(frame);

	for(auto s : faceObj.getRect())
	{
		cv::rectangle(frame, s, cv::Scalar(0,255,0), 3);	
		std::cout<<"height "<<s.height<<" width "<<s.width<<" x "<<s.x<<" y "<<s.y<<std::endl;
	}

	cvNamedWindow("FaceDetection", CV_WINDOW_NORMAL);
	cvSetWindowProperty("FaceDetection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	imshow("FaceDetection", frame);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}
