// Copyright (C) 2022 Plumerai Ltd under the Apache 2.0 license.
//
// Implements a fake version of the Plumerai People Detection library such that
// we can test linking and running the application without needing access to the
// actual library. This file does not implement the regular Plumerai People
// Detection library header, do not use it outside of (automated) testing.
#include "plumerai/people_detection.h"

namespace plumerai {

PeopleDetection::PeopleDetection(int height, int width) {}
PeopleDetection::~PeopleDetection() {}

std::vector<BoxPrediction> PeopleDetection::process_frame(
    const std::uint8_t *image_data, float delta_t) {
  // This does nothing: it returns an empty vector
  return std::vector<BoxPrediction>();
}

}  // namespace plumerai
