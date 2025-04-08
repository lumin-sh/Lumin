
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

#include <format>

#include "LuminVirtualMachine.hpp"
#include "Utils.hpp"

std::string GetLoggerName() {
    return "lumin";
}

int main( const int argc, char *argv[] ) {
    int opt;
    constexpr auto options = "f:|feature|:d:|disable:|h|help|V|verbose|v|version|g|debug|";

    while ( (opt = lumin::utils::getopt( argc, argv, options ) ) != -1 ) {
        switch ( opt ) {
            case 'v':
                if ( current_option == "version" || current_option == "v" ) {
                    LOG_INFO( "Lumin Virtual Machine Information:" )
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
            case 'd':
                LOG_INFO( "Feature disabled" )
                break;
            case 'h':
                LOG_INFO( "Help information displayed here" )
                break;
            case 'V':
                LOG_INFO( "Verbose mode enabled" )
                break;
            case 'g':
                LOG_INFO( "Debug mode enabled" )
                break;
            case 'f':
                LOG_INFO( "Feature enabled" )
                break;
            default:
                break;
        }
    }

    // Pi -> 0xDB, 0x0F, 0x49, 0x40

    const std::vector<Lumin::VM::byte> bytecode {
        static_cast<Lumin::VM::byte>( OpCode::ICONST ), 0x0A, 0x00, 0x00, 0x00,
        //static_cast<Lumin::VM::byte>( OpCode::ICONST ), 0x0A, 0x00, 0x00, 0x00,
        static_cast<Lumin::VM::byte>( OpCode::I2F ),
        static_cast<Lumin::VM::byte>( OpCode::FCONST ), 0xDB, 0x0F, 0x49, 0x40,
        static_cast<Lumin::VM::byte>( OpCode::FADD ),
        //static_cast<Lumin::VM::byte>( OpCode::INEG )
    };

    const auto VM = std::make_unique<Lumin::VM::LuminVirtualMachine>( bytecode );
    VM->Step();
    VM->Step();
    VM->Step();
    VM->Step();
    //
    VM->frames[0].local_variables.size();
    LOG_DEBUG ( std::format( "Value on the top of the stack: {}", std::get<float>( VM->stack.top() ) ) )
    //


    return 0;
}
