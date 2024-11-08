// Copyright (C) 2024 Plumerai Ltd under the Apache 2.0 license.
//
// Implements a fake version of the Plumerai Video Intelligence header such that
// we can test linking and running the application without needing access to the
// actual library. This file is not the regular Plumerai Video Intelligence
// header, do not use it outside of (automated) testing.
#pragma once

#include <cstdint>
#include <vector>

typedef struct BoxPrediction {
  float y_min;
  float x_min;
  float y_max;
  float x_max;
  float confidence;
  unsigned int id;
  unsigned int class_id;
} BoxPrediction;

namespace plumerai {

enum class ErrorCode { SUCCESS = 0 };

enum class ImageFormat { PACKED_RGB888 };

template <ImageFormat format>
struct ImagePointer {
  ImagePointer(const std::uint8_t* _data) {}
};

class ObjectDetection {
 public:
  ErrorCode get_detections(std::vector<BoxPrediction>& results);
};

class VideoIntelligence {
 public:
  VideoIntelligence(int height, int width);
  ~VideoIntelligence();

  template <ImageFormat image_format>
  ErrorCode process_frame(const ImagePointer<image_format> image_data,
                          float delta_t = 0.f) {
    return ErrorCode::SUCCESS;
  }

  ObjectDetection object_detection() const { return ObjectDetection(); }
};

}  // namespace plumerai
