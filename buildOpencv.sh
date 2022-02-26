#!/bin/bash
# License: MIT. See license file in root directory
# Copyright(c) JetsonHacks (2017-2019)

OPENCV_VERSION=4.4.0
# Jetson Nano
ARCH_BIN=5.3

OUTPUT_FILE=output.txt

INSTALL_DIR=/usr/local
# Download the opencv_extras repository
# If you are installing the opencv testdata, ie
#  OPENCV_TEST_DATA_PATH=../opencv_extra/testdata
# Make sure that you set this to YES
# Value should be YES or NO
DOWNLOAD_OPENCV_EXTRAS=NO
# Source code directory
OPENCV_SOURCE_DIR=$HOME
WHEREAMI=$PWD
# NUM_JOBS is the number of jobs to run simultaneously when using make
# This will default to the number of CPU cores (on the Nano, that's 4)
# If you are using a SD card, you may want to change this
# to 1. Also, you may want to increase the size of your swap file
NUM_JOBS=1

CLEANUP=true

PACKAGE_OPENCV="-D CPACK_BINARY_DEB=ON"

function usage
{
    echo "usage: ./buildOpenCV.sh [[-s sourcedir ] | [-h]]"
    echo "-s | --sourcedir   Directory in which to place the opencv sources (default $HOME)"
    echo "-i | --installdir  Directory in which to install opencv libraries (default /usr/local)"
    echo "--no_package       Do not package OpenCV as .deb file (default is true)"
    echo "-h | --help  This message"
}

# Iterate through command line inputs
while [ "$1" != "" ]; do
    case $1 in
        -s | --sourcedir )      shift
                                OPENCV_SOURCE_DIR=$1
                                ;;
        -i | --installdir )     shift
                                INSTALL_DIR=$1
                                ;;
        --no_package )          PACKAGE_OPENCV=""
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

CMAKE_INSTALL_PREFIX=$INSTALL_DIR

# Print out the current configuration
echo "Build configuration: "
echo " NVIDIA Jetson Nano"
echo " OpenCV binaries will be installed in: $CMAKE_INSTALL_PREFIX"
echo " OpenCV Source will be installed in: $OPENCV_SOURCE_DIR"
if [ "$PACKAGE_OPENCV" = "" ] ; then
   echo " NOT Packaging OpenCV"
else
   echo " Packaging OpenCV"
fi

if [ $DOWNLOAD_OPENCV_EXTRAS == "YES" ] ; then
 echo "Also downloading opencv_extras"
fi

# Repository setup
sudo apt-add-repository universe |& tee -a $OUTPUT_FILE
sudo apt-get update |& tee -a $OUTPUT_FILE 

# Download dependencies for the desired configuration
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
find / -name " *opencv* " -exec rm -i {} \; |& tee -a $OUTPUT_FILE 
find / -name " *opencv4* " -exec rm -i {} \; |& tee -a $OUTPUT_FILE 
apt purge libopencv-dev libopencv-python libopencv-samples libopencv* |& tee -a $OUTPUT_FILE 

cd $OPENCV_SOURCE_DIR
git clone --branch "$OPENCV_VERSION" https://github.com/opencv/opencv.git |& tee -a $OUTPUT_FILE 
git clone --branch "$OPENCV_VERSION" https://github.com/opencv/opencv_contrib.git |& tee -a $OUTPUT_FILE 

if [ $DOWNLOAD_OPENCV_EXTRAS == "YES" ] ; then
 echo "Installing opencv_extras"
 # This is for the test data
 cd $OPENCV_SOURCE_DIR
 git clone https://github.com/opencv/opencv_extra.git |& tee -a $OUTPUT_FILE 
 cd opencv_extra
 git checkout -b v${OPENCV_VERSION} ${OPENCV_VERSION} |& tee -a $OUTPUT_FILE 
fi

cd $OPENCV_SOURCE_DIR/opencvess
mkdir build 
cd build

# Here are some options to install source examples and tests
#     -D INSTALL_TESTS=ON \
#     -D OPENCV_TEST_DATA_PATH=../opencv_extra/testdata \
#     -D INSTALL_C_EXAMPLES=ON \
#     -D INSTALL_PYTHON_EXAMPLES=ON \

# If you are compiling the opencv_contrib modules:
# curl -L https://github.com/opencv/opencv_contrib/archive/3.4.1.zip -o opencv_contrib-3.4.1.zip

# There are also switches which tell CMAKE to build the samples and tests
# Check OpenCV documentation for details
#       -D WITH_QT=ON \

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
      $"PACKAGE_OPENCV" \
      ../ |& tee -a $OUTPUT_FILE 


if [ $? -eq 0 ] ; then
  echo "CMake configuration make successful" |& tee -a $OUTPUT_FILE  
else
  # Try to make again
  echo "CMake issues " >&2 |& tee -a $OUTPUT_FILE  
  echo "Please check the configuration being used" |& tee -a $OUTPUT_FILE 
  exit 1
fi

# Consider the MAXN performance mode if using a barrel jack on the Nano
time make -j$NUM_JOBS |& tee -a $OUTPUT_FILE 
if [ $? -eq 0 ] ; then
  echo "OpenCV make successful" |& tee -a $OUTPUT_FILE 
else
  # Try to make again; Sometimes there are issues with the build
  # because of lack of resources or concurrency issues
  echo "Make did not build " >&2 |& tee -a $OUTPUT_FILE 
  echo "Retrying ... "
  # Single thread this time
  make |& tee -a $OUTPUT_FILE 
  if [ $? -eq 0 ] ; then
    echo "OpenCV make successful" |& tee -a $OUTPUT_FILE 
  else
    # Try to make again
    echo "Make did not successfully build" >&2 |& tee -a $OUTPUT_FILE 
    echo "Please fix issues and retry build" |& tee -a $OUTPUT_FILE 
    exit 1
  fi
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

# If PACKAGE_OPENCV is on, pack 'er up and get ready to go!
# We should still be in the build directory ...
if [ "$PACKAGE_OPENCV" != "" ] ; then
   echo "Starting Packaging" |& tee -a $OUTPUT_FILE 
   sudo ldconfig |& tee -a $OUTPUT_FILE 
   time sudo make package -j$NUM_JOBS |& tee -a $OUTPUT_FILE
   if [ $? -eq 0 ] ; then
     echo "OpenCV make package successful" |& tee -a $OUTPUT_FILE
   else
     # Try to make again; Sometimes there are issues with the build
     # because of lack of resources or concurrency issues
     echo "Make package did not build " >&2 |& tee -a $OUTPUT_FILE
     echo "Retrying ... " |& tee -a $OUTPUT_FILE
     # Single thread this time
     sudo make package |& tee -a $OUTPUT_FILE
     if [ $? -eq 0 ] ; then
       echo "OpenCV make package successful" |& tee -a $OUTPUT_FILE
     else
       # Try to make again
       echo "Make package did not successfully build" >&2 |& tee -a $OUTPUT_FILE
       echo "Please fix issues and retry build" |& tee -a $OUTPUT_FILE
       exit 1
     fi
   fi
fi


# check installation
IMPORT_CHECK="$(python -c "import cv2 ; print cv2.__version__")"
if [[ $IMPORT_CHECK != *$OPENCV_VERSION* ]]; then
  echo "There was an error loading OpenCV in the Python sanity test."
  echo "The loaded version does not match the version built here."
  echo "Please check the installation."
  echo "The first check should be the PYTHONPATH environment variable."
fi
