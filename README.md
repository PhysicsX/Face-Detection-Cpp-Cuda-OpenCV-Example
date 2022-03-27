# Face Detection Example with Cuda and OpenCV using C++

Example of face detection using haarcascade model using cuda on Jetson Nano/NX development board. 
What is Cuda ? Cuda is framework to use GPU for massive calculations like image processing or AI applications etc. If you want efficient, robust and industrial Object/Face Detection application especially for embedded devices, C++ and Cuda is a good option.

![](https://github.com/PhysicsX/Face-Detection-Cuda-OpenCV-Example/blob/master/misc/faceDetection.gif?raw=true)

## OpenCV installation
Latest version of the OpenCV is needed. Just run the script on the ubuntu. (tested with JetPack 4.6.0 and opencv version is 4.4.0)
```bash
./buildOpencv.sh
```
This script will delete current OpenCV installation and pull the desired version from the repo and compile it using cuda extension.
Qt4 is disabled. You can build the Qt5 yourself.
You can find the configuration summary like this: 
As it is seen, cuda should be "YES" ( NVIDIA CUDA: YES (ver 10.2, CUFFT CUBLAS FAST_MATH)). 

```bash
-- General configuration for OpenCV 4.4.0 =====================================
--   Version control:               4.4.0
--
--   Extra modules:
--     Location (extra):            /home/ulas/opencv_contrib/modules
--     Version control (extra):     4.4.0
--
--   Platform:
--     Timestamp:                   2022-03-25T23:02:02Z
--     Host:                        Linux 4.9.253-tegra aarch64
--     CMake:                       3.10.2
--     CMake generator:             Unix Makefiles
--     CMake build tool:            /usr/bin/make
--     Configuration:               RELEASE
--
--   CPU/HW features:
--     Baseline:                    NEON FP16
--       required:                  NEON
--       disabled:                  VFPV3
--
--   C/C++:
--     Built as dynamic libs?:      YES
--     C++ standard:                11
--     C++ Compiler:                /usr/bin/c++  (ver 7.5.0)
--     C++ flags (Release):         -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Winit-self -Wsuggest-override -Wno-delete-non-virtual-dtor -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections    -fvisibility=hidden -fvisibility-inlines-hidden -O3 -DNDEBUG  -DNDEBUG
--     C++ flags (Debug):           -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Winit-self -Wsuggest-override -Wno-delete-non-virtual-dtor -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections    -fvisibility=hidden -fvisibility-inlines-hidden -g  -O0 -DDEBUG -D_DEBUG
--     C Compiler:                  /usr/bin/cc
--     C flags (Release):           -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Winit-self -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections    -fvisibility=hidden -O3 -DNDEBUG  -DNDEBUG
--     C flags (Debug):             -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Winit-self -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections    -fvisibility=hidden -g  -O0 -DDEBUG -D_DEBUG
--     Linker flags (Release):      -Wl,--gc-sections -Wl,--as-needed
--     Linker flags (Debug):        -Wl,--gc-sections -Wl,--as-needed
--     ccache:                      NO
--     Precompiled headers:         NO
--     Extra dependencies:          m pthread cudart_static -lpthread dl rt nppc nppial nppicc nppicom nppidei nppif nppig nppim nppist nppisu nppitc npps cublas cudnn cufft -L/usr/local/cuda-10.2/lib64 -L/usr/lib/aarch64-linux-gnu
--     3rdparty dependencies:
--
--   OpenCV modules:
--     To be built:                 alphamat aruco bgsegm bioinspired calib3d ccalib core cudaarithm cudabgsegm cudacodec cudafeatures2d cudafilters cudaimgproc cudalegacy cudaobjdetect cudaoptflow cudastereo cudawarping cudev datasets dnn dnn_objdetect dnn_superres dpm face features2d flann freetype fuzzy gapi hfs highgui img_hash imgcodecs imgproc intensity_transform line_descriptor ml objdetect optflow phase_unwrapping photo plot python2 python3 quality rapid reg rgbd saliency shape stereo stitching structured_light superres surface_matching text tracking video videoio videostab xfeatures2d ximgproc xobjdetect xphoto
--     Disabled:                    world
--     Disabled by dependency:      -
--     Unavailable:                 cnn_3dobj cvv hdf java js julia matlab ovis sfm ts viz
--     Applications:                apps
--     Documentation:               NO
--     Non-free algorithms:         NO
--
--   GUI:
--     GTK+:                        YES (ver 3.22.30)
--       GThread :                  YES (ver 2.56.4)
--       GtkGlExt:                  NO
--     OpenGL support:              NO
--     VTK support:                 NO
--
--   Media I/O:
--     ZLib:                        /usr/lib/aarch64-linux-gnu/libz.so (ver 1.2.11)
--     JPEG:                        /usr/lib/aarch64-linux-gnu/libjpeg.so (ver 80)
--     WEBP:                        build (ver encoder: 0x020f)
--     PNG:                         /usr/lib/aarch64-linux-gnu/libpng.so (ver 1.6.34)
--     TIFF:                        /usr/lib/aarch64-linux-gnu/libtiff.so (ver 42 / 4.0.9)
--     JPEG 2000:                   build Jasper (ver 1.900.1)
--     OpenEXR:                     build (ver 2.3.0)
--     HDR:                         YES
--     SUNRASTER:                   YES
--     PXM:                         YES
--     PFM:                         YES
--
--   Video I/O:
--     DC1394:                      NO
--     FFMPEG:                      YES
--       avcodec:                   YES (57.107.100)
--       avformat:                  YES (57.83.100)
--       avutil:                    YES (55.78.100)
--       swscale:                   YES (4.8.100)
--       avresample:                NO
--     GStreamer:                   YES (1.14.5)
--     v4l/v4l2:                    YES (linux/videodev2.h)
--
--   Parallel framework:            pthreads
--
--   Trace:                         YES (with Intel ITT)
--
--   Other third-party libraries:
--     Lapack:                      NO
--     Eigen:                       YES (ver 3.3.4)
--     Custom HAL:                  YES (carotene (ver 0.0.1))
--     Protobuf:                    build (3.5.1)
--
--   NVIDIA CUDA:                   YES (ver 10.2, CUFFT CUBLAS FAST_MATH)
--     NVIDIA GPU arch:             53
--     NVIDIA PTX archs:
--
--   cuDNN:                         YES (ver 8.2.1)
--
--   OpenCL:                        YES (no extra features)
--     Include path:                /home/ulas/opencv/3rdparty/include/opencl/1.2
--     Link libraries:              Dynamic load
--
--   Python 2:
--     Interpreter:                 /usr/bin/python2.7 (ver 2.7.17)
--     Libraries:                   /usr/lib/aarch64-linux-gnu/libpython2.7.so (ver 2.7.17)
--     numpy:                       /usr/lib/python2.7/dist-packages/numpy/core/include (ver 1.13.3)
--     install path:                lib/python2.7/dist-packages/cv2/python-2.7
--
--   Python 3:
--     Interpreter:                 /usr/bin/python3 (ver 3.6.9)
--     Libraries:                   /usr/lib/aarch64-linux-gnu/libpython3.6m.so (ver 3.6.9)
--     numpy:                       /usr/lib/python3/dist-packages/numpy/core/include (ver 1.13.3)
--     install path:                lib/python3.6/dist-packages/cv2/python-3.6
--
--   Python (for build):            /usr/bin/python2.7
--
--   Java:
--     ant:                         NO
--     JNI:                         NO
--     Java wrappers:               NO
--     Java tests:                  NO
--
--   Install to:                    /usr/local
-- -----------------------------------------------------------------

```
All build log will be saved to a file. You can read it later for debugging purpose. It will be on the same directory.
Note: I do not run the script as "su" so it can ask password and because of timeout it can ask again during the installation. Yes installetion will take a few hours on the jetson Nano :(. And also there can be console prompt questions. That is why time to time check that what script needs help or not.

## How to test the application
Now you are ready to run the app.  Be sure that picamera is connected to correct port. There are two. Or you can adjust from the accroding to sensor id from code.

![](https://www.uctronics.com/media/catalog/product/cache/5d1f08909b337dbf64263212361f58e9/i/m/imx219_ff_ir_nano_4.jpg)

If you are working from ssh or remotely then do not forget to export display after or before building the binary.
If you are not go to base directory where CMakeList.txt is located and run:
```
$ mkdir build && cd build && cmake .. && cmake --build
$ export DISPLAY=:0
$ ./build/FaceDetectorCudaOpenCV
```
