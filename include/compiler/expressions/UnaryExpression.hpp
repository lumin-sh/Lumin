
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

#ifndef UNARYEXPRESSION_HPP
#define UNARYEXPRESSION_HPP

#include <memory>
#include "Expression.hpp"
#include <TokenType.hpp>

using namespace Lumin::Compiler;

class UnaryExpression : public Expression {
public:
    TokenType operator_;
    std::unique_ptr<Expression> right;

    void accept( ExpressionVisitor<void> &visitor ) override {
        visitor.visit( *this );
    }

    UnaryExpression( TokenType operator_, std::unique_ptr<Expression> right )
        : operator_( operator_ ), right( std::move( right ) ) {}
};

#endif //UNARYEXPRESSION_HPP
