
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

#ifndef LUMIN_BYTECODEWRITER_HPP
#define LUMIN_BYTECODEWRITER_HPP

#include <vector>
#include <Opcode.hpp>

namespace Lumin::Bytecode {

class BytecodeWriter {
public:
    std::vector<uint8_t> bytecode;

    void Emit(OpCode opcode);

    void Emit(uint64_t value);
    void Emit(int64_t value);
    void Emit(uint32_t value);
    void Emit(int32_t value);
    void Emit(int16_t value);
    void Emit(uint8_t value);
    void Emit(int8_t value);
    void Emit(float value);
    void Emit(double value);
    void Emit(bool value);

};

}

#endif //LUMIN_BYTECODEWRITER_HPP
