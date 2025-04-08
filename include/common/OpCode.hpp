
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

#ifndef LUMIN_OPCODE_HPP
#define LUMIN_OPCODE_HPP

#include <cstdint>

namespace Lumin::Bytecode {

enum class OpCode : unsigned char {
    // Integers
    ICONST = 0,   // Push integer constant
    ILOAD = 1,    // Load integer from local variable
    ISTORE = 2,   // Store integer to local variable
    IADD = 3,     // Integer addition
    ISUB = 4,     // Integer subtraction
    IMUL = 5,     // Integer multiplication
    IDIV = 6,     // Integer division
    IPRINT = 7,   // Print top of stack
    ICMP = 8,     // Compare top two stack values
    IFEQ = 9,     // Conditional jump if equal
    GOTO = 10,    // Unconditional jump
    HALT = 11,    // Stop execution
    IFNE = 12,    // Jump if not equal
    IFLT = 13,    // Jump if less than
    IFGT = 14,    // Jump if greater than
    IFLE = 15,    // Jump if less than or equal
    IFGE = 16,    // Jump if greater than or equal

    // Stack manipulation
    SWAP = 17,    // Swap top two stack elements
    DUP = 18,     // Duplicate top stack element
    POP = 19,     // Remove top stack element

    // Type conversion
    I2F = 20,     // Convert integer to float
    F2I = 21,     // Convert float to integer
    I2D = 22,     // Convert integer to double
    D2I = 23,     // Convert double to integer
    I2L = 24,     // Convert integer to long
    L2I = 25,     // Convert long to integer
    I2C = 26,     // Convert integer to char
    C2I = 27,     // Convert char to integer
    I2S = 28,     // Convert integer to short
    S2I = 29,     // Convert short to integer

    // Float operations
    FCONST = 30,  // Push float constant
    FADD = 31,    // Float addition
    FSUB = 32,    // Float subtraction
    FMUL = 33,    // Float multiplication
    FDIV = 34,    // Float division
    FNEG = 35,    // Float negation
    FPRINT = 36,  // Print float from top of stack

    // Double operations
    DCONST = 37,  // Push double constant
    DADD = 38,    // Double addition
    DSUB = 39,    // Double subtraction
    DMUL = 40,    // Double multiplication
    DDIV = 41,    // Double division
    DNEG = 42,    // Double negation
    DPRINT = 43,  // Print double from top of stack

    // Long operations
    LCONST = 44,  // Push long constant
    LADD = 45,    // Long addition
    LSUB = 46,    // Long subtraction
    LMUL = 47,    // Long multiplication
    LDIV = 48,    // Long division
    LNEG = 49,    // Long negation
    LPRINT = 50,  // Print long from top of stack

    // Char operations
    CCONST = 51,  // Push char constant
    CPRINT = 52,  // Print char from top of stack

    // Short operations
    SCONST = 53,  // Push short constant
    SPRINT = 54,  // Print short from top of stack

    // Advanced control flow
    CALL = 55,    // Call function
    RETURN = 56,  // Return from function

    // Logical and bitwise operations
    IAND = 57,    // Bitwise AND
    IOR = 58,     // Bitwise OR
    IXOR = 59,    // Bitwise XOR
    INEG = 60,    // Integer negation
    LAND = 61,    // Bitwise AND (long)
    LOR = 62,     // Bitwise OR (long)
    LXOR = 63,    // Bitwise XOR (long)

    // Memory and array operations
    LOAD_ARRAY = 64,   // Load array element
    STORE_ARRAY = 65,  // Store to array element
    ALLOC_ARRAY = 66   // Allocate new array
};

}

#endif //LUMIN_OPCODE_HPP
