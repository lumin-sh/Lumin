
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

#include <typeinfo>
#include <Lexer.hpp>
#include <Parser.hpp>
#include "Utils.hpp"

std::string GetLoggerName() {
    return "luminc";
}

int main( const int argc, char** argv ) {
    int opt;
    /*
     o/output - output file
     d/disable - disable feature
     h/help - help
     V/verbose - verbose
     v/version - version
     g/debug - debug
     w/warning - warning level
     n/nowarn - disable warnings
     f/feature - enable feature
     */
    constexpr auto options = "o:|output|:d:|disable:|h|help|V|verbose|v|version|g|debug|w:|warning|:n:|nowarn|:f:|feature|:";
    while ( (opt = lumin::utils::getopt( argc, argv, options ) ) != -1 ) {
        switch ( opt ) {
            case 'v':
                if ( current_option == "version" || current_option == "v" ) {
                    LOG_INFO( "Lumin Compiler Information:" )
                    LOG_INFO( "  Target Lumin Version: " + std::to_string( LUMIN_VERSION_MAJOR ) +
                         "." + std::to_string ( LUMIN_VERSION_MINOR ) +  "(Patch " + std::to_string ( LUMIN_VERSION_PATCH ) + ")" );
                    LOG_INFO( "  Platform: "
                        + std::string( LUMIN_ARCH ) + "-"
                        + std::string( LUMIN_BUILD_PLATFORM ) + "-"
                        + std::string( LUMIN_BUILD_COMPILER )
                        )
                    LOG_INFO( "  Build Date: " + std::string( LUMIN_BUILD_DATE ) )
                    return 0;
                }
                break;
            case 'o':
                LOG_INFO( "Output file: " + std::string( optarg ) )
                break;
            case 'd':
                LOG_INFO("Feature disabled")
                break;
            case 'h':
                LOG_INFO("Help information displayed here")
                break;
            case 'V':
                LOG_INFO("Verbose mode enabled")
                break;
            case 'g':
                LOG_INFO("Debug mode enabled")
                break;
            case 'w':
                LOG_INFO("Warning level set: " + std::string(optarg))
                break;
            case 'n':
                LOG_INFO("Warnings disabled: " + std::string(optarg))
                break;
            case 'f':
                LOG_INFO("Feature enabled")
                break;
            default:
                break;
        }
    }

    for (int i = optind; i < argc; i++) {
        LOG_INFO("Non-option argument: " + std::string(argv[i]))
    }

    std::stringstream ss;
    ss << "private inline fun mul(a: int, b: int) {";
    ss << "     return a * b;";
    ss << "}";
    ss << "";
    ss << "fun main() {";
    ss << "     var x = 10 + 10;";
    ss << "     mul(10, 10);";
    ss << "}";

    Lexer lexer(ss.view());
    std::cout << "Tokenizing" << std::endl;
    Parser parser(lexer.Tokenize());
    std::cout << "Parsing" << std::endl;
    auto statements = parser.Parse();
    std::cout << "Printing statements" << std::endl;
    int i = 0;
    for ( const auto& statement : statements ) {
        std::cout << i << ": " << typeid(*statement).name() << "\n";
        i++;
    }



    return 0;
}