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

#include <cstring>
#include <BytecodeWriter.hpp>

using namespace Lumin::Bytecode;

void BytecodeWriter::Emit( const uint64_t value ) {
    bytecode.push_back( value & 0xFF );
    bytecode.push_back( (value >> 8) & 0xFF );
    bytecode.push_back( (value >> 16) & 0xFF );
    bytecode.push_back( (value >> 24) & 0xFF );
    bytecode.push_back( (value >> 32) & 0xFF );
    bytecode.push_back( (value >> 40) & 0xFF );
    bytecode.push_back( (value >> 48) & 0xFF );
    bytecode.push_back( (value >> 56) & 0xFF );
}


void BytecodeWriter::Emit( const int64_t value ) {
    Emit( static_cast<uint64_t>( value ) );
}

void BytecodeWriter::Emit( const double value ) {
    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(double));

    Emit( bits );
}

void BytecodeWriter::Emit( const uint32_t value ) {
    bytecode.push_back( value & 0xFF );          // Least significant byte
    bytecode.push_back( (value >> 8) & 0xFF );   // Second byte
    bytecode.push_back( (value >> 16) & 0xFF );  // Third byte
    bytecode.push_back( (value >> 24) & 0xFF );  // Most significant byte
}

void BytecodeWriter::Emit( const int32_t value ) {
    Emit( static_cast<uint32_t>( value ) );
}

void BytecodeWriter::Emit( const float value ) {
    uint32_t bits;
    std::memcpy( &bits, &value, sizeof( float ) );

    Emit( bits );
}


void BytecodeWriter::Emit( const int16_t value ) {
    bytecode.push_back( value & 0xFF );
    bytecode.push_back( (value >> 8) & 0xFF );
}

void BytecodeWriter::Emit( const bool value ) {
    Emit( static_cast<int8_t>( value ) );
}

void BytecodeWriter::Emit( const uint8_t value ) {
    bytecode.push_back( value & 0xFF );
}

void BytecodeWriter::Emit( const int8_t value ) {
    Emit( static_cast<uint8_t>( value ) );
}

void BytecodeWriter::Emit( const OpCode opcode ) {
    bytecode.push_back( static_cast<uint8_t>( opcode ) );
}
