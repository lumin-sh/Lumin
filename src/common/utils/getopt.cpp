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
#include <Logging.hpp>
#include <cstring>
#include <format>

std::vector<lumin::utils::Option> lumin::utils::parse_option_string( std::string_view ostr ) {
    std::vector<Option> options;

    for ( size_t i = 0; i < ostr.length(); ++i ) {
        if ( ostr[i] == '|' ) {
            const size_t end = ostr.find( '|', i + 1 );
            if ( end == std::string_view::npos ) {
                // unmatched delimiter
                if ( opterr ) {
                    LOG_WARN( loggerName(), "Option string format error: unmatched '|'" );
                }
                break;
            }

            const std::string name( ostr.substr( i + 1, end - i - 1 ) );
            bool requires_arg = false;

            if ( end + 1 < ostr.length() && ostr[end + 1] == ':' ) {
                requires_arg = true;
                i = end + 1; // skip ':'
            } else {
                i = end; // skip to closing '|'
            }

            // long options wrapped in pipes
            options.push_back( { name, requires_arg, true } );
        } else {
            // normal single character options
            const char opt_char = ostr[i];
            if ( opt_char == ':' ) continue; // standalone colons

            const bool requires_arg = ( i + 1 < ostr.length() && ostr[i + 1] == ':' );

            const std::string name( 1, opt_char );
            options.push_back( { name, requires_arg, false } );

            // skip colon (if present)
            if (requires_arg) ++i;
        }
    }

    return options;
}

int lumin::utils::getopt( const int nargc, char* const nargv[], const std::string_view ostr ) {
    static const char* place = EMSG;
    static std::vector<Option> options;
    static bool options_parsed = false;

    // parse option string once
    if ( !options_parsed ) {
        options = parse_option_string( ostr );
        options_parsed = true;
    }

    // reset if needed
    if (optreset) {
        optreset = false;
        options_parsed = false;
        options = parse_option_string( ostr );
        place = EMSG;
    }

    // check if we need to get a new arg
    if ( !*place ) {
        if ( optind >= nargc || *(place = nargv[optind]) != '-' ) {
            place = EMSG;
            return -1;
        }

        // handle '--' as end of options
        if ( place[1] && place[1] == '-' && place[2] == '\0' ) {
            ++optind;
            place = EMSG;
            return -1;
        }
    }

    // process long options (--option)
    if ( place[0] == '-' && place[1] == '-' && place[2] != '\0' ) {
        const char* option_start = place + 2; // skip '--'
        std::string current_opt = option_start;

        // check '=' for arg
        const char* equals_pos = strchr( option_start, '=' );
        if ( equals_pos ) {
            current_opt = std::string( option_start, equals_pos - option_start );
        }

        // find the option in our parsed options
        bool found = false;
        bool needs_arg = false;
        char matched_short = '\0';

        for ( const auto& [name, requires_arg, is_long] : options ) {
            if ( is_long && name == current_opt ) {
                found = true;
                needs_arg = requires_arg;
                current_option = name;

                // find short option for the return value
                for ( const auto& short_opt : options ) {
                    if ( !short_opt.is_long &&
                        short_opt.name.length() == 1 &&
                        short_opt.name[0] == name[0] ) {
                        matched_short = short_opt.name[0];
                        break;
                    }
                }

                // if no matching short, use first character
                if ( matched_short == '\0' ) {
                    matched_short = name[0];
                }

                optopt = static_cast<int>( matched_short );
                break;
            }
        }

        if ( !found ) {
            if ( opterr && ostr[0] != ':' ) {
                LOG_WARN( loggerName(), std::format( "Illegal option - {}", current_opt ) )
            }
            place = EMSG;
            ++optind;
            return BADCH;
        }

        // handle args
        if ( !needs_arg ) {
            optarg = nullptr;
        } else {
            if ( equals_pos ) {
                // arg is after '=' in the same parameter
                optarg = equals_pos + 1;
            } else if ( optind + 1 < nargc ) {
                // arg is the next parameter
                optarg = nargv[++optind];
            } else {
                // missing required argument
                place = EMSG;
                if ( ostr[0] == ':' ) {
                    ++optind;
                    return BADARG;
                }

                if ( opterr ) {
                    LOG_WARN( loggerName(), std::format( "Option requires an argument - {}", current_opt ) );
                }
                ++optind;
                return BADCH;
            }
        }

        place = EMSG;
        ++optind;
        return optopt;
    }

    // short options (-o)
    if ( place[0] == '-' && place[1] != '\0' && place[1] != '-' ) {
        optopt = static_cast<int>( *++place ); // skip '-'
        current_option = std::string( 1, static_cast<char>( optopt ) );

        // find the option in our parsed options
        bool valid_option = false;
        bool needs_arg = false;

        for ( const auto& [name, requires_arg, is_long] : options ) {
            if ( !is_long && name.length() == 1 && name[0] == static_cast<char>( optopt ) ) {
                valid_option = true;
                needs_arg = requires_arg;
                break;
            }
        }

        if ( !valid_option ) {
            if ( opterr && ostr[0] != ':' ) {
                LOG_WARN( loggerName(), std::format( "Illegal option - {}", static_cast<char>( optopt ) ) );
            }

            if ( !place[1] ) {
                ++optind;
                place = EMSG;
            } else {
                // move to next character in current argument
                ++place;
            }

            return BADCH;
        }

        if ( !needs_arg ) {
            // if there are more characters, treat them as the next option
            if ( place[1] ) {
                ++place;
            } else {
                place = EMSG;
                ++optind;
            }
        } else {
            if ( place[1] ) {
                // arg is the rest of this parameter
                optarg = place + 1;
                place = EMSG;
                ++optind;
            } else if ( nargc <= ++optind ) {
                // Nn more parameters, but argument required
                place = EMSG;
                if ( ostr[0] == ':' ) {
                    return BADARG;
                }
                if ( opterr ) {
                    LOG_WARN( loggerName(), std::format( "Option requires an argument - {}", static_cast<char>( optopt ) ) );
                }
                return BADCH;
            } else {
                // arg is the next parameter
                optarg = nargv[optind];
                place = EMSG;
                ++optind;
            }
        }

        return optopt;
    }

    // if we get here it's a dash followed by nothing
    ++optind;
    place = EMSG;
    return BADCH;
}