// Copyright 2023 Leidos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#include <string>
#include "sensor_data_sharing_msg/vehicle/attitude.hpp"
#include "sensor_data_sharing_msg/vehicle/attitude_confidence.hpp"
#include "sensor_data_sharing_msg/vehicle/angular_velocity_set.hpp"
#include "sensor_data_sharing_msg/vehicle/angular_velocity_confidence_set.hpp"
#include "sensor_data_sharing_msg/vehicle/vehicle_size.hpp"
#include "sensor_data_sharing_msg/vehicle/vehicle_size_confidence.hpp"

namespace streets_utils::messages{
    struct detected_vehicle_data{
        /**
         * @brief BIT String representing the state of each of the following vehicle 
         * exterior lights:
         *  lowBeamHeadlightsOn       (0), 
         *  highBeamHeadlightsOn      (1), 
         *  leftTurnSignalOn          (2), 
         *  rightTurnSignalOn         (3), 
         *  hazardSignalOn            (4), 
         *  automaticLightControlOn   (5),
         *  daytimeRunningLightsOn    (6), 
         *  fogLightOn                (7), 
         *  parkingLightsOn           (8) 
         * 
         */
        std::optional<std::string> exterior_lights;
        /**
         * @brief Vehicle Attitude.
         */
        std::optional<attitude> _veh_attitude;
        /**
         * @brief Confidence in reported vehicle attitude.
         */
        std::optional<attitude_confidence> _attitude_confidence;
        /**
         * @brief Angular velocity set in pitch and roll axis.
         */
        std::optional<angular_velocity_set> _angular_velocity;
        /**
         * @brief Confidence in reported angular velocity set.
         */
        std::optional<angular_velocity_confidence_set> _angular_velocity_confidence;
        /**
         * @brief Vehicle two dimensional size.
         */
        std::optional<vehicle_size> _size;
        /**
         * @brief Vehicle height in unit of 5 cm [0, 127]
         */
        std::optional<unsigned int>  _vehicle_height;
        /**
         * @brief Confidence in reported size.
         */
        std::optional<vehicle_size_confidence> _size_confidence; 
        /**
         * @brief See BasicVehicleClass in J2735
         */
        std::optional<unsigned int>  _vehicle_class;
        /**
         * @brief Confidence in vehicle classification [0,101]
         */
        std::optional<unsigned int>  _classification_confidence;

    };
}