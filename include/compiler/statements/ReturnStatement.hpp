
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

#ifndef RETURNSTATEMENT_HPP
#define RETURNSTATEMENT_HPP

#include <memory>
#include "Statement.hpp"
#include "expressions/Expression.hpp"

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value;

    explicit ReturnStatement( std::unique_ptr<Expression> value = nullptr )
        : value( std::move( value ) ) {}

    void accept( StatementVisitor<void>& visitor ) override {
        visitor.visit( *this );
    }
};

#endif //RETURNSTATEMENT_HPP
