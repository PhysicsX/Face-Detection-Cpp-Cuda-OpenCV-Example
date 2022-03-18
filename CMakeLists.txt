cmake_minimum_required(VERSION 3.10)
project(FaceDetectorCudaOpenCV LANGUAGES CXX)

include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp src/face_detector_cuda.cpp src/streamer.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# OpenCV setup
find_package(OpenCV REQUIRED)
message("Opencv version ${OpenCV_VERSION}")
if(OpenCV_VERSION VERSION_LESS "4.4.0")
	message("Opencv version may be not suitable")
endif()
message("OpenCV libs ${OpenCV_LIBS}")
message("OpenCV directory ${OpenCV_DIR}")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Model file
set(HAARCASCADE_FRONTAL "${PROJECT_SOURCE_DIR}/model/haarcascade_frontalface_alt.xml")
#set(FACE_DETECTION_WEIGHTS "${PROJECT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")

target_compile_definitions(${PROJECT_NAME} PRIVATE HAARCASCADE_FRONTAL="${HAARCASCADE_FRONTAL}")
#target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")
