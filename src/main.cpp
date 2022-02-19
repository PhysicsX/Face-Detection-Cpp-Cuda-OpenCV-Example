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
// g++ -I /usr/local/include/opencv4 example.cpp -o example `pkg-config opencv4 --cflags --libs`

using namespace cv;
using namespace std;

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
        cerr << "Can not open video source";
        return -1;
    }
    //cvNamedWindow("FaceDetection", WindowFlags::CV_WINDOW_NORMAL);
    //cvSetWindowProperty("FaceDetection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    std::vector<cv::Rect> h_found;
    cv::Ptr<cv::cuda::CascadeClassifier> cascade = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
    cv::cuda::GpuMat d_frame, d_gray, d_found;

    FaceDetector faceObj;
    while(1)
    {
	Mat frame;
        if ( !cap.read(frame) ) {
            cerr << "Can not read frame from webcam";
            return -1;
        }
        //d_frame.upload(frame);
        //cv::cuda::cvtColor(d_frame, d_gray, cv::COLOR_BGR2GRAY);
        //cascade->detectMultiScale(d_gray, d_found);
	//cascade->convert(d_found, h_found);
	faceObj.setFrame(frame);

	h_found = faceObj.getRect();
	for(int i = 0; i < h_found.size(); ++i)
	{
              rectangle(frame, h_found[i], Scalar(0,255,255), 5);
	}


	cvNamedWindow("Result", CV_WINDOW_NORMAL);
	cvSetWindowProperty("Result", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

        imshow("Result", frame);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}
