#!/bin/bash

OPENCV_VERSION=4.4.0

ARCH_BIN=5.3
WHEREAMI=$PWD
OUTPUT_FILE=$WHEREAMI/output.txt

if [ -f "$OUTPUT_FILE" ]; then
    echo "$OUTPUT_FILE exists deleting it"
    rm -rf $OUTPUT_FILE
fi

OPENCV_SOURCE_DIR=$HOME

INSTALL_DIR=/usr/local

CMAKE_INSTALL_PREFIX=$INSTALL_DIR

echo " Path for installation $CMAKE_INSTALL_PREFIX"
echo " Path for source $OPENCV_SOURCE_DIR"

sudo apt-add-repository universe |& tee -a $OUTPUT_FILE
sudo apt-get update -y |& tee -a $OUTPUT_FILE 
sudo apt-get upgrade -y |& tee -a $OUTPUT_FILE

cd $WHEREAMI
sudo apt-get install -y \
    build-essential \
    cmake \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libeigen3-dev \
    libglew-dev \
    libgtk2.0-dev \
    libgtk-3-dev \
    libjpeg-dev \
    libpng-dev \
    libpostproc-dev \
    libswscale-dev \
    libtbb-dev \
    libtiff5-dev \
    libv4l-dev \
    libxvidcore-dev \
    libx264-dev \
    qt5-default \
    zlib1g-dev \
    python-dev \
    python-numpy \
    python-py \
    python-pytest \
    python3-dev \
    python3-numpy \
    python3-py \
    python3-pytest \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    pkg-config |& tee -a $OUTPUT_FILE 

# Remove old opencv if it is exist
sudo find / -name " *opencv* " -exec rm -i {} \; |& tee -a $OUTPUT_FILE 
sudo find / -name " *opencv4* " -exec rm -i {} \; |& tee -a $OUTPUT_FILE 
sudo apt purge -y libopencv-dev libopencv-python libopencv-samples libopencv* |& tee -a $OUTPUT_FILE 

cd $OPENCV_SOURCE_DIR
git clone --branch "$OPENCV_VERSION" https://github.com/opencv/opencv.git |& tee -a $OUTPUT_FILE 
git clone --branch "$OPENCV_VERSION" https://github.com/opencv/opencv_contrib.git |& tee -a $OUTPUT_FILE 

cd $OPENCV_SOURCE_DIR/opencv
mkdir build 
cd build

echo $PWD
time cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} \
      -D OPENCV_GENERATE_PKGCONFIG=ON \
      -D WITH_CUDA=ON \
      -D CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-10.2 \
      -D CUDA_ARCH_BIN=${ARCH_BIN} \
      -D CUDA_ARCH_PTX="" \
      -D ENABLE_FAST_MATH=ON \
      -D CUDA_FAST_MATH=ON \
      -D WITH_CUBLAS=ON \
      -D WITH_LIBV4L=ON \
      -D WITH_V4L=ON \
      -D WITH_GSTREAMER=ON \
      -D WITH_GSTREAMER_0_10=OFF \
      -D WITH_QT=OFF \
      -D WITH_OPENGL=ON \
      -D BUILD_opencv_python2=ON \
      -D BUILD_opencv_python3=ON \
      -D BUILD_TESTS=OFF \
      -D BUILD_PERF_TESTS=OFF \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D CPACK_BINARY_DEB=ON \
      ../ |& tee -a $OUTPUT_FILE 


if [ $? -eq 0 ] ; then
  echo "CMake configuration make successful" |& tee -a $OUTPUT_FILE  
else
  echo "CMake configuration is not successful" |& tee -a $OUTPUT_FILE 
  exit 1
fi

time make -j1 |& tee -a $OUTPUT_FILE 
if [ $? -eq 0 ] ; then
  echo "OpenCV make successful" |& tee -a $OUTPUT_FILE 
else
  echo "Issues during building" |& tee -a $OUTPUT_FILE 
  exit 1
fi

echo "Installing ... " |& tee -a $OUTPUT_FILE 
sudo make install |& tee -a $OUTPUT_FILE 
sudo ldconfig |& tee -a $OUTPUT_FILE 
if [ $? -eq 0 ] ; then
   echo "OpenCV installed in: $CMAKE_INSTALL_PREFIX" |& tee -a $OUTPUT_FILE 
else
   echo "There was an issue with the final installation" |& tee -a $OUTPUT_FILE 
   exit 1
fi

echo "Crate deb package for opencv" |& tee -a $OUTPUT_FILE 
sudo ldconfig |& tee -a $OUTPUT_FILE 
time sudo make package -j1 |& tee -a $OUTPUT_FILE
if [ $? -eq 0 ] ; then
  echo "OpenCV make package successful" |& tee -a $OUTPUT_FILE
else
  echo "Make package did not successfully build" |& tee -a $OUTPUT_FILE
  exit 1
fi

echo "OpenCV successfully built and installed. Enjoy !" |& tee -a $OUTPUT_FILE

