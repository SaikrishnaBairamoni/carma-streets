#!/bin/bash
#  Copyright (C) 2018-2020 LEIDOS.
# 
#  Licensed under the Apache License, Version 2.0 (the "License"); you may not
#  use this file except in compliance with the License. You may obtain a copy of
#  the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#  License for the specific language governing permissions and limitations under
#  the License.

# script executes all kafka_clients and scheduling service build and coverage steps so that they can be singularly
# wrapped by the sonarcloud build-wrapper
set -e

COVERAGE_FLAGS="-g --coverage -fprofile-arcs -ftest-coverage"
ls -la
pwd
cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_service_base
mkdir build
cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_service_base/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install

cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_vehicle_list
mkdir build
cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_vehicle_list/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install

cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_signal_phase_and_timing
mkdir build
cd /streets_utils/streets_signal_phase_and_timing/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install

cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_api/intersection_client_api
mkdir build
cd /home/carma-streets/streets_utils/streets_api/intersection_client_api/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install

cd /home/runner/work/carma-streets/carma-streets/streets_utils/streets_vehicle_scheduler
mkdir build
cd /home/carma-streets/streets_utils/streets_vehicle_scheduler/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install

cd /home/runner/work/carma-streets/carma-streets/streets_api/intersection_server_api
mkdir build
cd /home/carma-streets/streets_utils/streets_api/intersection_server_api/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make
make install
cd /home/runner/work/carma-streets/carma-streets/kafka_clients
mkdir build
cd /home/carma-streets/kafka_clients/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs
make install


cd /home/runner/work/carma-streets/carma-streets/scheduling_service
mkdir build
cd /home/carma-streets/scheduling_service/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs

cd /home/runner/work/carma-streets/carma-streets/intersection_model
mkdir build
cd /home/carma-streets/intersection_model/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs

cd /home/runner/work/carma-streets/carma-streets/message_services
mkdir build
cd /home/carma-streets/message_services/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs


cd /home/runner/work/carma-streets/carma-streets/signal_opt_service
mkdir build
cd /home/carma-streets/signal_opt_service/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs

cd /home/runner/work/carma-streets/carma-streets/tsc_client_service
mkdir build
cd /home/carma-streets/tsc_client_service/build
cmake -DOPTION_BUILD_DOCS=ON -DCMAKE_CXX_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_C_FLAGS="${COVERAGE_FLAGS}" -DCMAKE_BUILD_TYPE="Debug" ..
make api-docs

