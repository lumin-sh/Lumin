
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

#ifndef EXPRESSIONSTATEMENT_HPP
#define EXPRESSIONSTATEMENT_HPP

#include <memory>
#include "Statement.hpp"
#include "expressions/Expression.hpp"

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    void accept( StatementVisitor<void> &visitor ) override  {
        visitor.visit( *this );
    }

    explicit ExpressionStatement( std::unique_ptr<Expression> expression )
        : expression( std::move( expression ) ) {}
};

#endif //EXPRESSIONSTATEMENT_HPP
