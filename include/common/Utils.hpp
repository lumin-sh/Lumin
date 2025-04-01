
/*
 Copyright (C) 2025 Lumin Sh

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef LUMIN_UTILS_HPP
#define LUMIN_UTILS_HPP

#if defined(_WIN32)
#define LUMIN_BUILD_PLATFORM "Windows"
#elif defined(__APPLE__)
#define LUMIN_BUILD_PLATFORM "MacOS"
#elif defined(__linux__)
#define LUMIN_BUILD_PLATFORM "Linux"
#else
#define LUMIN_BUILD_PLATFORM "Unknown"
#endif

#if defined(_MSC_VER)
#define LUMIN_BUILD_COMPILER "MSVC"
#elif defined(__clang__)
#define LUMIN_BUILD_COMPILER "Clang"
#elif defined(__GNUC__)
#define LUMIN_BUILD_COMPILER "GCC"
#else
#define LUMIN_BUILD_COMPILER "Unknown"
#endif

#ifdef __x86_64__
#define LUMIN_ARCH "x86_64"
#elifdef __aarch64__
#define LUMIN_ARCH "aarch64"
#endif

constexpr char LUMIN_BUILD_DATE[11] = {
    '2', '0', __DATE__[9], __DATE__[10], '-',
    (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',
    (__DATE__[0] == 'J') ? ( (__DATE__[1] == 'a') ? '1' :
                             ((__DATE__[2] == 'n') ? '6' : '7') ) :
    (__DATE__[0] == 'F') ? '2' :
    (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' :
    (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' :
    (__DATE__[0] == 'S') ? '9' :
    (__DATE__[0] == 'O') ? '0' :
    (__DATE__[0] == 'N') ? '1' :
    (__DATE__[0] == 'D') ? '2' :
    0, '-',
    __DATE__[4] == ' ' ? '0' : __DATE__[4], __DATE__[5], '\0'
};

#include <Logging.hpp>
#include <utils/getopt.hpp>
#include <LuminFile.hpp>

#endif //LUMIN_UTILS_HPP
