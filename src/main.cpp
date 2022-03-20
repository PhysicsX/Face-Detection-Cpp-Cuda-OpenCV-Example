#include <iostream>
#include "face_detector_cuda.hpp"

int main() {
  
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;
    std::cout << "Major version : " << CV_MAJOR_VERSION << std::endl;
    std::cout << "Minor version : " << CV_MINOR_VERSION << std::endl;
    std::cout << "Subminor version : " << CV_SUBMINOR_VERSION << std::endl;
    

    FaceDetector faceObj;
    // blocking loop
    // can be ended with q or ctrl+C
    faceObj.loop();
    
    return 0;
}
