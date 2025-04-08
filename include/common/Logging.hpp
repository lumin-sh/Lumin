
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

#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <string>
#include <filesystem>

#define LEVEL_DEBUG "\u001b[38;5;63m"
#define LEVEL_INFO "\u001b[38;5;157m"
#define LEVEL_WARN "\u001b[38;5;220m"
#define LEVEL_ERRORC "\u001b[38;5;196m"
#define LEVEL_RESET "\u001b[0m"

std::string GetLoggerName();

inline void log(const std::string& level_color, const std::string& message) {
    std::cout << level_color << message << LEVEL_RESET << std::endl;
}

inline void log(const std::string& level_color, const std::string& name, const std::string& message) {
    std::cout << level_color << "[" << name << "] " << message << LEVEL_RESET << std::endl;
}

inline void log_debug(
    const std::string& file,
    const int line,
    const std::string& message
) {
    const std::string filename = std::filesystem::path(file).filename().string();
    std::cout << LEVEL_DEBUG << "[" << filename << ":" << line << "] " << message << LEVEL_RESET << std::endl;
}


#ifdef NDEBUG
#define LOG_DEBUG(...)
#else
#define LOG_DEBUG(...) log_debug(__FILE__, __LINE__, __VA_ARGS__);
#endif
#define LOG_INFO(...) log(LEVEL_INFO, __VA_ARGS__);
#define LOG_WARN(...) log(LEVEL_WARN, __VA_ARGS__);
#define LOG_ERROR(...) log(LEVEL_ERRORC, __VA_ARGS__);

#endif // LOGGING_HPP

