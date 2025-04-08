
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

#ifndef LUMINFILE_HPP
#define LUMINFILE_HPP

#include <cstdint>
#include <variant>
#include <string>
#include <vector>

constexpr uint8_t LUMIN_VERSION_MAJOR = 25;
constexpr uint8_t LUMIN_VERSION_MINOR = 0;
constexpr uint8_t LUMIN_VERSION_PATCH = 0;

constexpr uint32_t LUMIN_MAGIC_NUMBER = 0xC0FFEE;

constexpr uint8_t FLAG_PUBLIC = 0x01;
constexpr uint8_t FLAG_PRIVATE = 0x02;
constexpr uint8_t FLAG_INTERNAL = 0x04;
constexpr uint8_t FLAG_STATIC = 0x08;
constexpr uint8_t FLAG_CONST = 0x10;
constexpr uint8_t FLAG_NATIVE = 0x20;
constexpr uint8_t FLAG_SYNC = 0x40;
constexpr uint8_t FLAG_ABSTRACT = 0x80;

struct FieldInfo {
    uint16_t flags; // Access Modifiers and other attributes
    uint16_t nameIndex; // Index in CP for field name
    uint16_t typeIndex; // Index in CP for field type
    uint32_t defaultValueIndex; // Index in CP of default value
};

struct MethodInfo {
    uint16_t flags; // Access Modifiers and other attributes
    uint16_t  nameIndex; // Index in CP for method name
    uint16_t signatureIndex; // Index fin CP for return/parameter types
    uint16_t maxStack; // Max operand stack size needed
    uint16_t maxLocals; // Number of local variables (incl parameters)
};

struct ClassInfo {
    uint16_t flags; // Class modifiers (public, abstract, etc)
    uint16_t nameIndex; // Index in CP for class name
    uint16_t superClassIndex; // Index in CP for parent class
    std::vector<FieldInfo> fields; // Fields in the class
    std::vector<MethodInfo> methods; // Methods in the class
};

enum class ConstantPoolTag : uint8_t {
    // Primatives (1-10 range)
    CONSTANT_INTEGER   = 1,  // 32-bit integer
    CONSTANT_FLOAT     = 2,  // 32-bit float
    CONSTANT_LONG      = 3,  // 64-bit integer
    CONSTANT_DOUBLE    = 4,  // 64-bit float

    // Text Data (11-20 range)
    CONST_UTF8         = 11, // UTF-8 encoded string
    CONST_STRING       = 12, // String reference (to another constant)

    // Type & Class References (21-30 range)
    CONST_CLASS        = 21, // Class reference
    CONST_INTERFACE    = 22, // Interface reference
    CONST_FIELD_REF    = 23, // Field reference
    CONST_METHOD_REF   = 24, // Method reference
    CONST_SIGNATURE    = 25, // Method signature info
};

struct ConstantPoolEntry {
    ConstantPoolTag tag;
    std::variant<
        std::monostate,
        int32_t,
        float,
        int64_t,
        double,
        std::string,
        uint16_t
    > data;

    void Serialize(std::ofstream &outFile) const;
    static ConstantPoolEntry Deserialize(std::ifstream &inFile);
};

struct LuminFile {
    uint32_t magicNumber;
    uint8_t versionMajor;
    uint8_t versionMinor;
    uint16_t flags;
    std::vector<ConstantPoolEntry> constantPool;
    std::vector<unsigned char> bytecode;
};

namespace Lumin::Utils {

bool WriteLuminFile(const std::string& outputPath, const LuminFile& luminFile);
LuminFile ReadLuminFile(const std::string& inputPath);

}

#endif //LUMINFILE_HPP
