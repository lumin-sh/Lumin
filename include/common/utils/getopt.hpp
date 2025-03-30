
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

#ifndef LUMIN_GETOPT_HPP
#define LUMIN_GETOPT_HPP


#include <string>
#include <vector>

constexpr char EMSG[] = "";
constexpr int BADCH = '?';
constexpr int BADARG = ':';

inline int optind = 1;
inline int optopt = 0;
inline int opterr = 1;
inline int optreset = 0;
inline const char* optarg = nullptr;
inline std::string current_option = "";

namespace lumin::utils {

struct Option {
    std::string name;
    bool requires_arg;
    bool is_long;
};

std::vector<Option> parse_option_string( std::string_view ostr );
int getopt( const int nargc, char* const nargv[], std::string_view ostr );

}

#endif //LUMIN_GETOPT_HPP
