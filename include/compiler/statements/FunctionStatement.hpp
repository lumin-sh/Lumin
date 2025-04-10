
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

#ifndef FUNCTIONSTATEMENT_HPP
#define FUNCTIONSTATEMENT_HPP

#include <string>
#include <utility>
#include <vector>
#include "AccessModifier.hpp"
#include "InlineSpecifier.hpp"
#include "BlockStatement.hpp"
#include "TokenType.hpp"

using namespace Lumin::Compiler;

class FunctionStatement final : public Statement {
public:
    std::string name;
    AccessModifier access;
    InlineSpecifier inlineSpec;
    std::vector<std::pair<std::string, TokenType>> parameters;
    std::vector<std::unique_ptr<Statement>> body;

    void accept( StatementVisitor<void> &visitor ) override {
        visitor.visit( *this );
    }

    FunctionStatement(
        std::string& name,
        const AccessModifier access,
        const InlineSpecifier inlineSpec,
        std::vector<std::pair<std::string, TokenType>> params,
        std::vector<std::unique_ptr<Statement>> body
    )
        : name( std::move( name ) )
        ,access( access )
        , inlineSpec( inlineSpec )
        , parameters( std::move( params ) )
        , body( std::move( body ) ) {}
};

#endif //FUNCTIONSTATEMENT_HPP
