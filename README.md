# Face Detection Example with Cuda and OPenCV

Example of face detection using haarcascade model using cuda on Jetson Nano/NX development board.

## OpenCV installation
Just run the script on the ubuntu. (JetPack 4.6.1)
```bash
./buildOpencv.sh
```
This script will delete current opencv installation and pull the desired version from the repo and compile it using cuda extension.
Qt4 is disabled. You can build the qt5 yourself.


## How to test the application
Now you are ready to run the app.  Be sure that picamera is connected to correct port. There are two. Or you can adjust from the accroding to sensor id.
If you are working from ssh or remotely then do not forget to export display after or before building the binary.
Go to file directory.
```
$ mkdir build && cd build && cmake .. && cmake --build
$ export DISPLAY=:0
$ ./FaceDetectorCudaOpenCV
```
