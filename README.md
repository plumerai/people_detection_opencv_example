# Plumerai Video Intelligence OpenCV pipeline example

This is the source code for a simple version of using the Plumerai Video Intelligence library in an end-to-end demo setting. It runs on Linux, uses OpenCV and V4L, and retrieves input data from a webcam (or optionally an RTSP stream) and writes box coordinates to console and to an output video stream displayed on screen. It can be modified as needed for proto-typing with the Plumerai Video Intelligence library.

The Plumerai Video Intelligence library itself is not included: this repository can't be used without access to the library. If you do not have access to the library and would like to evaluate it, then contact us at [plumerai.com/contact_us](https://plumerai.com/contact_us). For more information, see [plumerai.com/people-detection](https://plumerai.com/people-detection).

## Linux system requirements

The example application uses [OpenCV](https://opencv.org/). Using the `CAP_V4L` OpenCV API, it connects to the webcam with [V4L (video 4 linux)](https://en.wikipedia.org/wiki/Video4Linux). Most (USB) webcams compatible with V4L should work with this example demo application, as long as they support the Motion-JPEG (MJPEG) input compression format. If you have V4L installed on your system (see below), this can be verified by running `v4l2-ctl --list-formats-ext`.

On a Debian-based system (e.g. Ubuntu) the pre-requisites (OpenCV with V4L) can be installed as follows:

```bash
apt install libopencv-dev libopencv-highgui-dev libopencv-videoio-dev libv4l-dev
```

In case of issues or for other Linux systems, see the [official OpenCV release page](https://opencv.org/releases/).

## Configuring the camera settings

First, make sure the camera settings (`camera_height`, `camera_width`, and `camera_id`) are set correctly near the top of the `src/opencv_example.cc` file.

The higher the input resolution, the better the results can become. However, note that this might slow down the entire example application, because the camera capture and video-displaying might take up more resources. The framerate reported is purely for the Plumerai Video Intelligence algorithm itself, and does not count camera capture or displaying of the results.

Note that the example application itself (the camera capture / video decoding and displaying) is not optimized for speed. Furthermore, the video intelligence algorithm in its current form is not optimized for speed on x86 systems, only for targets such as Arm Cortex-A.

## Optional: use an RTSP stream as input

This repository also contains example code to use an RTSP video stream instead of camera data as input. The code changes are minimal, and can be enabled by uncommenting the `#define USE_RTSP_INPUT` macro near the top of the `src/opencv_example.cc` file. Then, a little bit below the RTSP stream settings (`camera_height`, `camera_width`, and `rtsp_url`) can be changed as needed.

## Compiling the example application

The OpenCV example can be compiled with a C++11 compiler linking to both OpenCV4 and the Plumerai Video Intelligence library, e.g.:

```bash
g++ -std=c++11 -O3 src/opencv_example.cc -I/path/to/plumeraivideointelligence/include/ /path/to/plumeraivideointelligence/lib/<PLATFORM>/libplumeraivideointelligence.a `pkg-config --cflags --libs opencv4` -ldl -pthread -o opencv_example
```

Here, `<PLATFORM>` above refers to the platform you are on, e.g. `x86_64` or `aarch64`. In case of an older OpenCV version, remove the `4` in the `opencv4` part above.

The example app can then be executed without any command-line arguments, e.g.:

```
./opencv_example
```
