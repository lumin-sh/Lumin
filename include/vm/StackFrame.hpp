
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

#ifndef STACKFRAME_HPP
#define STACKFRAME_HPP

#include <vector>
#include <NumericValue.hpp>

namespace Lumin::VM {

struct StackFrame {
    size_t return_address;
    size_t base_pointer;
    std::vector<NumericValue> local_variables;

    StackFrame( const size_t ret_addr, const size_t base_ptr, const size_t local_count )
        : return_address( ret_addr ), base_pointer( base_ptr ),
        local_variables( local_count ) {}
};

}

#endif //STACKFRAME_HPP
