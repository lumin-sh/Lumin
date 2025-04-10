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
#include <LuminVirtualMachine.hpp>
#include <string>

#include "Logging.hpp"

using namespace Lumin::VM;

LuminVirtualMachine::LuminVirtualMachine( const std::vector<byte>& bytecode ) {
    this->bytecode = bytecode;
    this->ip = 0;
    this->base_pointer = 0;

    Init();
}

void LuminVirtualMachine::Run() {
    ip = 0;

    while ( ip < bytecode.size() && !freezeExecution) {
        const auto opcode = static_cast<OpCode>( bytecode[ip++] );

        try {
            Process( opcode );
        } catch ( const std::exception& exception ) {
            LOG_ERROR( std::format( "LuminVM Error {} ( IP: {} )", exception.what(), ip - 1 ) )

        }
    }
}

void LuminVirtualMachine::Step() {
    LOG_DEBUG( std::format( "Stepping at IP: {}", ip ) )
    if ( ip < bytecode.size() ) {
        const auto opcode = static_cast<OpCode>( bytecode[ip++] );

        try {
            Process( opcode );
        } catch ( const std::exception& exception ) {
            LOG_DEBUG( std::format( "LuminVM Error {} ( IP: {} )", exception.what(), ip - 1 ) )
        }
    } else {
        LOG_DEBUG ( std::format( "Cannot step any furter! ( IP: {} )", ip ) )
    }
}


void LuminVirtualMachine::Reset() {
    ip = 0;
    stack.Clear();
    frames.clear();
    base_pointer = 0;
}

void LuminVirtualMachine::Init() {
    opcode_handlers = {
        { OpCode::ICONST, &LuminVirtualMachine::HandleICONST },
        { OpCode::ILOAD, &LuminVirtualMachine::HandleILOAD },
        { OpCode::ISTORE, &LuminVirtualMachine::HandleISTORE },
        { OpCode::IDIV, &LuminVirtualMachine::HandleIDIV },
        { OpCode::IMUL, &LuminVirtualMachine::HandleIMUL },
        { OpCode::IADD, &LuminVirtualMachine::HandleIADD },
        { OpCode::ISUB, &LuminVirtualMachine::HandleISUB },
        { OpCode::INEG, &LuminVirtualMachine::HandleINEG },
        { OpCode::I2F, &LuminVirtualMachine::HandleI2F },
        //
        { OpCode::FCONST, &LuminVirtualMachine::HandleFCONST },
        { OpCode::FADD, &LuminVirtualMachine::HandleFADD },
    };
}

void LuminVirtualMachine::Process( OpCode opcode ) {
    const auto it = opcode_handlers.find( opcode );
    if ( it != opcode_handlers.end() ) {
        (this->*(it->second))();
    } else {
        throw std::runtime_error( "Unimplemented opcode: " + static_cast<int>( opcode ) );
    }
}

template < typename T >
T LuminVirtualMachine::Read() {
    if ( ip + sizeof( T ) > bytecode.size() ) {
        throw std::runtime_error( "Bytecode read out of bounds" );
    }

    T value = *reinterpret_cast<const T*>( &bytecode[ip] );
    ip += sizeof( T );

    return value;
}

template < typename T >
T LuminVirtualMachine::PopCheckedValue() {
    if ( stack.Empty() ) {
        throw std::runtime_error( "Stack underflow" );
    }

    T value = stack.Pop();

    return value;
}

template < typename Op >
NumericValue LuminVirtualMachine::PerformNumericOperation(
    const NumericValue& a,
    const NumericValue& b,
    Op operation
) {
    return std::visit([&operation]( auto x, auto y ) -> NumericValue {
         using X = std::decay_t<decltype( x )>;
         using Y = std::decay_t<decltype( y )>;
         if constexpr ( std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> ) {
             return operation( x, y );
         } else {
             throw std::runtime_error( "Incompatible types for operation" );
         }
    }, a, b);
}

void LuminVirtualMachine::HandleIMUL() {
    const auto a = PopCheckedValue<NumericValue>();
    const auto b = PopCheckedValue<NumericValue>();

    stack.Push( PerformNumericOperation( a, b, std::multiplies() ) );
}

void LuminVirtualMachine::HandleICONST() {
    stack.Push( Read<int32_t>() );
}

void LuminVirtualMachine::HandleIDIV() {
    const auto a = PopCheckedValue<NumericValue>();
    const auto b = PopCheckedValue<NumericValue>();

    stack.Push( PerformNumericOperation( a, b, std::divides() ) );
}

void LuminVirtualMachine::HandleIADD() {
    const auto a = PopCheckedValue<NumericValue>();
    const auto b = PopCheckedValue<NumericValue>();

    stack.Push( PerformNumericOperation( a, b, std::plus() ) );
}

void LuminVirtualMachine::HandleISUB() {
    const auto a = PopCheckedValue<NumericValue>();
    const auto b = PopCheckedValue<NumericValue>();

    stack.Push( PerformNumericOperation( a, b, std::minus() ) );
}

void LuminVirtualMachine::HandleINEG() {
    const auto a = PopCheckedValue<NumericValue>();

    auto negate = []( auto value ) -> NumericValue {
        using T = std::decay_t<decltype( value )>;
        if constexpr ( std::is_arithmetic_v<T> ) {
            return -value;  // Negate numeric types
        } else {
            throw std::runtime_error( "Unsupported type for negation" );
        }
    };

    stack.Push( std::visit( negate, a ) );
}

void LuminVirtualMachine::HandleISTORE() {
    const auto index = Read<uint16_t>();

    if ( index >= locals.size() ) {
        throw std::runtime_error( "Local variable index out of bounds" );
    }

    if ( stack.Empty() ) {
        throw std::runtime_error( "Stack underflow" );
    }

    locals[index] = PopCheckedValue<NumericValue>();
}

void LuminVirtualMachine::HandleILOAD() {
    const auto index = Read<uint16_t>();

    if ( index >= locals.size() ) {
        throw std::runtime_error( "Local variable index out of bounds" );
    }

    stack.Push( locals[index] );
}

void LuminVirtualMachine::HandleI2F() {
    const auto integer = std::get<int32_t>( PopCheckedValue<NumericValue>() );
    stack.Push( NumericValue( static_cast<float>( integer ) ) );
}

void LuminVirtualMachine::HandleFCONST() {
    stack.Push( Read<float>() );
}

void LuminVirtualMachine::HandleFADD() {
    const auto a = PopCheckedValue<NumericValue>();
    const auto b = PopCheckedValue<NumericValue>();

    stack.Push( PerformNumericOperation( a, b, std::plus() ) );
}
