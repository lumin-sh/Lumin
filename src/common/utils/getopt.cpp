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

#include <cstdio>
#include <utils/getopt.hpp>

int lumin::utils::getopt( const int nargc, char* const nargv[], std::string_view ostr ) {
    static const char* place = EMSG;
    if ( optreset || !*place ) {
        optreset = false;
        if ( optind >= nargc || *( place = nargv[optind] ) != '-' ) {
            place = EMSG;
            return -1;
        }
        if ( place[1] && *++place == '-' ) {
            ++optind;
            place = EMSG;
            return -1;
        }
    }

    optopt = static_cast<int>( *place++ );

    const size_t oli = ostr.find( static_cast<char>(optopt) );
    const bool valid_option = ( optopt != static_cast<int>(':') && oli != std::string_view::npos );

    if ( !valid_option ) {
        if ( optopt == static_cast<int>('-') ) {
            return -1;
        }

        if ( !*place ) {
            ++optind;
        }

        if ( opterr && ostr[0] != ':' ) {
            printf("Illegal option -- %c\n", optopt);
        }

        return BADCH;
    }

    const bool needs_arg = ( oli + 1 < ostr.length() && ostr[oli + 1] == ':' );

    if ( !needs_arg ) {
        optarg = nullptr;
        if ( !*place ) {
            ++optind;
        }
    } else {
        if ( *place ) {
            optarg = place;
        } else if ( nargc <= ++optind ) {
            place = EMSG;
            if ( ostr[0] == ':' ) {
                return BADARG;
            }

            if ( opterr ) {
                printf("Option requires an argument -- %c\n", optopt);
            }

            return BADCH;
        } else {
            optarg = nargv[optind];
        }

        place = EMSG;
        ++optind;
    }

    return optopt;
}