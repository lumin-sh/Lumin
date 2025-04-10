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

#include <fstream>
#include <LuminFile.hpp>
#include <Logging.hpp>

using namespace Lumin::Utils;

void ConstantPoolEntry::Serialize( std::ofstream& outFile ) const {
    outFile.write(reinterpret_cast<const char*>(&tag), sizeof(tag));

    switch (tag) {
        case ConstantPoolTag::CONSTANT_INTEGER:
            outFile.write(reinterpret_cast<const char*>(&std::get<int32_t>(data)), sizeof(int32_t));
        break;
        case ConstantPoolTag::CONSTANT_FLOAT:
            outFile.write(reinterpret_cast<const char*>(&std::get<float>(data)), sizeof(float));
        break;
        case ConstantPoolTag::CONSTANT_LONG:
            outFile.write(reinterpret_cast<const char*>(&std::get<int64_t>(data)), sizeof(int64_t));
        break;
        case ConstantPoolTag::CONSTANT_DOUBLE:
            outFile.write(reinterpret_cast<const char*>(&std::get<double>(data)), sizeof(double));
        break;
        case ConstantPoolTag::CONST_UTF8:
        case ConstantPoolTag::CONST_STRING: {
            const std::string& str = std::get<std::string>(data);
            const size_t len = str.size();
            outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
            outFile.write(str.c_str(), len);
            break;
        }
        case ConstantPoolTag::CONST_CLASS:
        case ConstantPoolTag::CONST_INTERFACE:
        case ConstantPoolTag::CONST_FIELD_REF:
        case ConstantPoolTag::CONST_METHOD_REF:
        case ConstantPoolTag::CONST_SIGNATURE:
            outFile.write(reinterpret_cast<const char*>(&std::get<uint16_t>(data)), sizeof(uint16_t));
        break;
        default:
            break;
    }
}

ConstantPoolEntry ConstantPoolEntry::Deserialize( std::ifstream& inFile ) {
    ConstantPoolEntry entry;
    inFile.read( reinterpret_cast<char*>( &entry.tag ), sizeof( entry.tag ) );

    switch (entry.tag) {
        case ConstantPoolTag::CONSTANT_INTEGER: {
            int32_t value;
            inFile.read( reinterpret_cast<char*>( &value ), sizeof( value ) );
            entry.data = value;
            break;
        }
        case ConstantPoolTag::CONSTANT_FLOAT: {
            float value;
            inFile.read( reinterpret_cast<char*>( &value ), sizeof( value ) );
            entry.data = value;
            break;
        }
        case ConstantPoolTag::CONSTANT_LONG: {
            int64_t value;
            inFile.read( reinterpret_cast<char*>( &value ), sizeof( value ) );
            entry.data = value;
            break;
        }
        case ConstantPoolTag::CONSTANT_DOUBLE: {
            double value;
            inFile.read( reinterpret_cast<char*>( &value ), sizeof( value ) );
            entry.data = value;
            break;
        }
        case ConstantPoolTag::CONST_UTF8:
        case ConstantPoolTag::CONST_STRING: {
            size_t len;
            inFile.read( reinterpret_cast<char*>( &len ), sizeof( len ) );
            std::string str( len, '\0' );
            inFile.read( &str[0], len );
            entry.data = str;
            break;
        }
        case ConstantPoolTag::CONST_CLASS:
        case ConstantPoolTag::CONST_INTERFACE:
        case ConstantPoolTag::CONST_FIELD_REF:
        case ConstantPoolTag::CONST_METHOD_REF:
        case ConstantPoolTag::CONST_SIGNATURE: {
            uint16_t ref;
            inFile.read( reinterpret_cast<char*>( &ref ), sizeof( ref ) );
            entry.data = ref;
            break;
        }
        default:
            break;
    }

    return entry;
}


bool Lumin::Utils::WriteLuminFile( const std::string& outputPath, const LuminFile& luminFile ) {
    std::ofstream file( outputPath, std::ios::binary );
    if ( !file ) {
        LOG_ERROR( "Failed to open file for writing: " + outputPath )
        return false;
    }

    const auto& [
        magicNumber,
        versionMajor,
        versionMinor,
        flags,
        constantPool,
        bytecode
    ] = luminFile;

    file.write( reinterpret_cast<const char*>( &magicNumber ), sizeof( magicNumber ) );
    file.write( reinterpret_cast<const char*>( &versionMajor ), sizeof( versionMajor ) );
    file.write( reinterpret_cast<const char*>( &versionMinor ), sizeof( versionMinor ) );
    file.write( reinterpret_cast<const char*>( &flags ), sizeof( flags ));

    const size_t poolSize = constantPool.size();
    file.write( reinterpret_cast<const char*>( &poolSize ), sizeof( poolSize ) );
    for ( const auto& entry : constantPool ) {
        entry.Serialize( file );
    }

    const size_t bytecodeSize = bytecode.size();
    file.write( reinterpret_cast<const char*>( bytecode.data() ), bytecodeSize * sizeof( unsigned char ) );

    file.close();

    return true;
}

LuminFile Lumin::Utils::ReadLuminFile( const std::string& inputPath ) {
    LuminFile luminFile;
    std::ifstream file( inputPath, std::ios::binary );
    if ( !file ) {
        LOG_ERROR( "Failed to open file for reading: " + inputPath )
        luminFile.magicNumber = -1;
        return luminFile;
    }

    file.read( reinterpret_cast<char*>( &luminFile.magicNumber ), sizeof( luminFile.magicNumber ) );
    file.read( reinterpret_cast<char*>( &luminFile.versionMajor ), sizeof( luminFile.versionMajor ) );
    file.read( reinterpret_cast<char*>( &luminFile.versionMinor ), sizeof( luminFile.versionMinor ) );
    file.read( reinterpret_cast<char*>( &luminFile.flags ), sizeof( luminFile.flags ) );

    size_t poolSize;
    file.read( reinterpret_cast<char*>( &poolSize ), sizeof( poolSize ) );
    luminFile.constantPool.resize( poolSize );
    for ( auto& entry : luminFile.constantPool ) {
        entry = ConstantPoolEntry::Deserialize( file );
    }

    size_t bytecodeSize;
    file.read( reinterpret_cast<char*>( &bytecodeSize ), sizeof( bytecodeSize ) );
    luminFile.bytecode.resize( bytecodeSize);
    file.read( reinterpret_cast<char*>( luminFile.bytecode.data() ), bytecodeSize );

    file.close();
    return luminFile;
}
