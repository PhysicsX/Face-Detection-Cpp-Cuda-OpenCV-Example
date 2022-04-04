#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

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
#include <queue>

class FaceDetector
{
	public:
	std::vector<cv::Rect> h_found;
	cv::Mat frame;
	mutable std::mutex mMutex;
	static bool runFlag;
	std::future<void> fut;
	Streamer mStreamer;
	std::vector<cv::Rect> landMarks;
	void setFrame(cv::Mat& frame);
	static void interrupt(int);


	class frameQueue {
		public:
		frameQueue():runFlag(false){
		cascadeU = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);
		}
		~frameQueue()
		{
			{
        			std::lock_guard<std::mutex> lock{mMutex};
        			runFlag = true;
        			mCondVar.notify_one();
        		}
        	
		if(fut.valid())
        	{
            		fut.wait();
        	}
		
		}
		

		void addQueue(const cv::Mat frame) {
        		
			std::lock_guard<std::mutex> lock{mMutex};
        		if(mQueue.size() < 5)
			{
				mQueue.push(frame);
        			mCondVar.notify_one();
			}
  		}	

  		void processQueue() {

    		if(!fut.valid())
    		{
    			fut = std::async(std::launch::async, [&](){

      			while (!runFlag) {
           			std::unique_lock<std::mutex> lock{mMutex};

           			mCondVar.wait(lock, [this]() { return !mQueue.empty() || runFlag; });

          			if(runFlag)
          			{
                			return;
          			}

           			const auto frame = mQueue.front();
           			mQueue.pop();
           			lock.unlock();
				
				//std::thread([this, frame](){
				if(frame.empty() == false)
                                {
				//	auto ptr = cv::cuda::CascadeClassifier::create(HAARCASCADE_FRONTAL);	
				
					cv::cuda::GpuMat d_frame, d_gray, d_found;
                                        d_frame.upload(frame);

                                        cv::cuda::cvtColor(d_frame, d_gray, cv::COLOR_BGR2GRAY);
                                        cascadeU->detectMultiScale(d_gray, d_found);
                                        std::vector<cv::Rect> landmarks;
                                        cascadeU->convert(d_found, landmarks);
					if(!landmarks.empty())
                                        	pushLandMark(landmarks);
					//for(const auto& l : landmarks)
                                        //        std::cout<<l.height<<" "<<l.width<<" "<<l.x<<" "<<l.y<<std::endl;
                                }
				//}).detach();

			}



      			});
    		}

  		}


		void pushLandMark(std::vector<cv::Rect> land)
		{
			std::lock_guard<std::mutex> lock{mMutex2};	
			landQueue.push(land);	
		}

		std::vector<cv::Rect> returnLand()
		{
			std::vector<cv::Rect> vec;
			std::lock_guard<std::mutex> lock{mMutex2};
			if(landQueue.empty())
			{
				return vec;
			}	
			else
			{
				vec = landQueue.front();
				landQueue.pop();
				return vec;
			}
		}	

		private:
		cv::Ptr<cv::cuda::CascadeClassifier> cascadeU;
		std::future<void> fut;
		std::queue<cv::Mat> mQueue;
		std::queue<std::vector<cv::Rect>> landQueue;
		
		std::mutex mMutex;
		std::mutex mMutex2;	
		bool runFlag;
		std::condition_variable mCondVar;
	} mFrameQueue;

	public:

	FaceDetector();
	~FaceDetector();
	std::vector<cv::Rect> getRect() const;
	void loop();
};

#endif // FACE_DETECTOR_H
