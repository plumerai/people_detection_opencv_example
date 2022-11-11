// Copyright (C) 2022 Plumerai Ltd under the Apache 2.0 license.
//
// Implements a fake version of the Plumerai People Detection header such that
// we can test linking and running the application without needing access to the
// actual library. This file is not the regular Plumerai People Detection
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

class PeopleDetection {
 public:
  PeopleDetection(int height, int width);
  ~PeopleDetection();

  std::vector<BoxPrediction> process_frame(const std::uint8_t *image_data,
                                           float delta_t = 0.f);
};

}  // namespace plumerai
