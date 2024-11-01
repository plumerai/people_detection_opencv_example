// Copyright (C) 2024 Plumerai Ltd under the Apache 2.0 license.
//
// Implements a fake version of the Plumerai Video Intelligence library such
// that we can test linking and running the application without needing access
// to the actual library. This file does not implement the regular Plumerai
// Video Intelligence library header, do not use it outside of (automated)
// testing.
#include "plumerai/video_intelligence.h"

namespace plumerai {

VideoIntelligence::VideoIntelligence(int height, int width) {}
VideoIntelligence::~VideoIntelligence() {}

ErrorCode ObjectDetection::get_detections(std::vector<BoxPrediction>& results) {
  results.clear();
  return ErrorCode::SUCCESS;
}

}  // namespace plumerai
