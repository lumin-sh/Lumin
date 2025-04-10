
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

#ifndef VARIABLESTATEMENT_HPP
#define VARIABLESTATEMENT_HPP

#include <string>
#include <memory>
#include <utility>
#include "Statement.hpp"
#include "expressions/Expression.hpp"
#include "AccessModifier.hpp"

class VariableStatement final : public Statement {
public:
    std::string name;
    AccessModifier access;
    std::unique_ptr<Expression> initializer;

    void accept( StatementVisitor<void> &visitor ) override  {
        visitor.visit( *this );
    }

    VariableStatement( std::string& name, const AccessModifier access, std::unique_ptr<Expression> initializer )
        : name( std::move( name ) ), access( access ), initializer( std::move( initializer ) ) {}
};

#endif //VARIABLESTATEMENT_HPP
