#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Set common compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

# Import the CMakeLists.txt for the glm library
add_subdirectory(glm)

# now build app's shared lib
file(GLOB_RECURSE BALL_SRCS ./Core/*.c*
                            ./GameObjects/*.c*
                            ./Rendering/*.c*
                            ./Scenes/*.c*
                            android_main.cpp)
add_library(game SHARED ${BALL_SRCS})

target_include_directories(game PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${ANDROID_NDK}/sources/android/native_app_glue)

# add lib dependencies
target_link_libraries(game
                      android
                      app-glue
                      atomic
                      EGL
                      GLESv2
                      glm
                      log
                      OpenSLES)
