/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#pragma once

#include "common/common.hh"
#include "vehicles/MavQuadCopter.hh"

#include <chrono>
#include <memory>
#include <vector>

class QuadCopterStopAvoidance
    : public CollisionAvoidanceStrategy<MavQuadCopter>
{
  public:
    QuadCopterStopAvoidance(std::shared_ptr<MavQuadCopter> quadcopter,
                            double trigger_distance = 4.0);

    void avoid(const std::vector<Obstacle> &detection) override;

  private:
    double trigger_dst;
};

