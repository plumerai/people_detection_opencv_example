// Copyright (C) 2022 Plumerai Ltd under the Apache 2.0 license.
//
// This code implements a simple demo around the Plumerai People Detection
// library using OpenCV to capture webcam input and display to screen. You can
// modify the code as needed for your prototyping use-case, it might not work
// out-of-the-box depending on your system and camera used. See the README for
// more information.

// Standard C++ library includes
#include <chrono>
#include <string>
#include <vector>

// Regular OpenCV libraries, these should be installed on your system
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio.hpp"

// The Plumerai People Detection library. If you do not have access to this
// library and would like to evaluate it, then contact us via
// https://plumerai.com/contact_us. For more information, see our website
// https://plumerai.com/people-detection, and docs.plumerai.com for information
// how to use it.
#include "plumerai/people_detection.h"

// Enable this marco to switch the input of this example application from
// camera input to RTSP stream:
// #define USE_RTSP_INPUT

// Input RTSP stream settings (change as needed)
#ifdef USE_RTSP_INPUT
constexpr auto width = 240;
constexpr auto height = 161;
constexpr auto rtsp_url =
    "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mp4";
#else
// Input camera settings (change as needed)
constexpr auto width = 1280;
constexpr auto height = 720;
constexpr auto camera_id = 0;  // 0 == the first camera, 1 == the second, etc.
#endif  // USE_RTSP_INPUT

// The following line sets the capture API to Video4Linux. Change this as needed
// on your system. See the OpenCV website for a list of all options:
// https://docs.opencv.org/4.6.0/d4/d15/group__videoio__flags__base.html
constexpr auto camera_capture_api = cv::CAP_V4L;

// The following line sets the camera format to Motion-JPEG. If your camera does
// not support this, then it could be changed to another format.
const auto camera_capture_format = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

// These are settings to draw bounding boxes on screen. They do not need to be
// modified. A bounding-box will use any of the following 19 colours. If there
// are more than 19 boxes found, it will re-use the same colour.
constexpr int num_colours = 19;
const int colours[num_colours][3] = {
    {230, 25, 75},  {60, 180, 75},   {255, 225, 25}, {0, 130, 200},
    {245, 130, 48}, {145, 30, 180},  {70, 240, 240}, {240, 50, 230},
    {210, 245, 60}, {250, 190, 212}, {0, 128, 128},  {220, 190, 255},
    {170, 110, 40}, {255, 250, 200}, {128, 0, 0},    {170, 255, 195},
    {128, 128, 0},  {255, 215, 180}, {0, 0, 128}};

int main() {
  // Set-up the Plumerai People Detection algorithm
  auto ppd = plumerai::PeopleDetection(height, width);

  // Set-up the output window
  const auto window_text = "Plumerai People Detection";
  cv::namedWindow(window_text, cv::WINDOW_AUTOSIZE);

  // Set-up the camera, see the above constants for the settings
#ifdef USE_RTSP_INPUT
  cv::VideoCapture camera(rtsp_url);
#else
  cv::VideoCapture camera(camera_id, camera_capture_api);
  camera.set(cv::CAP_PROP_FRAME_WIDTH, width);
  camera.set(cv::CAP_PROP_FRAME_HEIGHT, height);
  camera.set(cv::CAP_PROP_FOURCC, camera_capture_format);
#endif  // USE_RTSP_INPUT
  if (!camera.isOpened()) {
    // If this is printed the camera might not be attached, the stream might not
    // be valid or they might not support the settings set above (width &
    // height, capture API or format).
    printf("Error: Could not open the input, verify the settings\n");
    return -1;
  }

  // Run the demo in an infinite loop
  cv::Mat cv_image;
  while (true) {
    // Retrieve an image from the camera
    if (camera.read(cv_image) == false) {
      printf("Error: could not read frame.\n");
      return -1;
    }

    // Process the frame using the Plumerai People Detection library. This
    // function is timed and framerate in FPS is reported. Note that the
    // Plumerai People Detection library is not optimized for all systems,
    // see the README for more details.
    auto start_time = std::chrono::steady_clock::now();
    auto predictions = ppd.process_frame(cv_image.data);
    auto elapsed_time = std::chrono::steady_clock::now() - start_time;

    // Report the time and framerate the above function took
    auto time_ms =
        std::chrono::duration<double, std::milli>(elapsed_time).count();
    auto fps = static_cast<int>(1000 / time_ms);
    printf("Processing took %.1f ms (%d fps)\n", time_ms, fps);

    // Process the resulting bounding-boxes if there are any
    for (auto &p : predictions) {
      auto x_min =
          std::max(0, std::min(static_cast<int>(p.x_min * width), width - 1));
      auto y_min =
          std::max(0, std::min(static_cast<int>(p.y_min * height), height - 1));
      auto x_max =
          std::max(0, std::min(static_cast<int>(p.x_max * width), width - 1));
      auto y_max =
          std::max(0, std::min(static_cast<int>(p.y_max * height), height - 1));

      // Write the resulting coordinates to stdout
      printf(
          "Found box #%d with confidence %.2f at (x=%d, y=%d) - (x=%d, y=%d)\n",
          p.id, p.confidence, x_min, y_min, x_max, y_max);

      // Also draw the bounding-boxes on the video
      auto colour = colours[p.id % num_colours];
      for (int c = 0; c < 3; ++c) {
        for (int x = x_min; x < x_max; x++) {
          cv_image.data[(y_min * width + x) * 3 + c] = colour[c];
          cv_image.data[(y_max * width + x) * 3 + c] = colour[c];
        }
        for (int y = y_min; y < y_max; y++) {
          cv_image.data[(y * width + x_min) * 3 + c] = colour[c];
          cv_image.data[(y * width + x_max) * 3 + c] = colour[c];
        }
      }

      // Any user-processing code can be added here, e.g. writing the box
      // coordinates into a file, sending them over the network, or modifying
      // the image (e.g. person blurring for privacy).
    }

    // Finally display the webcam image with the resulting bounding-boxes
    cv::imshow(window_text, cv_image);
    const auto wait_ms = 30;  // can be changed to force a certain framerate
    if ((cv::waitKey(wait_ms) & 0xFF) == 27) {
      // Exit when ESC is pressed
      break;
    }
  }
  return 0;
}
