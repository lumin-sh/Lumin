
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

#ifndef LUMINVIRTUALMACHINE_HPP
#define LUMINVIRTUALMACHINE_HPP


#include <vector>
#include <unordered_map>
#include <Opcode.hpp>
#include <StackFrame.hpp>
#include <VMStack.hpp>

using namespace Lumin::Bytecode;

namespace Lumin::VM {

typedef unsigned char byte;

struct LuminVirtualMachineConfig {
    bool DebugMode = false;
};

class LuminVirtualMachine {
public:
    explicit LuminVirtualMachine(const std::vector<byte>& bytecode);
    // TODO: remove
    bool freezeExecution = false;
    void Step();
    void Run();
    void Reset();
    //
    VMStack<NumericValue> stack;
    std::vector<NumericValue> locals;
    std::vector<StackFrame> frames;
private:
    using OpcodeHandler = void (LuminVirtualMachine::*)();
    std::unordered_map<OpCode, OpcodeHandler> opcode_handlers;
    std::vector<byte> bytecode;
    size_t ip;
    size_t base_pointer;

    void Init();
    void Process(OpCode opcode);

    template < typename T >
    T Read();
    template< typename T >
    T PopCheckedValue();
    template < typename Op >
    NumericValue PerformNumericOperation(
        const NumericValue &a,
        const NumericValue &b,
        Op operation
    );

    // Integer
    void HandleICONST();
    void HandleILOAD();
    void HandleISTORE();
    void HandleIDIV();
    void HandleIMUL();
    void HandleIADD();
    void HandleISUB();
    void HandleINEG();
    void HandleI2F();
    // Floats
    void HandleFCONST();
    void HandleFDIV();
    void HandleFMUL();
    void HandleFADD();
    void HandleFSUB();
    void HandleFNEG();
    void HandleFLOAD();
    void HandleFSTORE();
    //
};

}

#endif //LUMINVIRTUALMACHINE_HPP
