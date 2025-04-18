
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

#ifndef LITERALEXPRESSION_HPP
#define LITERALEXPRESSION_HPP

#include "Expression.hpp"
#include <NumericValue.hpp>

class LiteralExpression : public Expression {
public:
    NumericValue value;

    void accept( ExpressionVisitor<void> &visitor ) override {
        visitor.visit( *this );
    }

    explicit LiteralExpression( const NumericValue value ) : value( value ) {}
};

#endif //LITERALEXPRESSION_HPP
