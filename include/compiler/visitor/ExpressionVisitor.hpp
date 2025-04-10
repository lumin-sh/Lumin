
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

#ifndef EXPRESSIONVISITOR_HPP
#define EXPRESSIONVISITOR_HPP

// Forward declarations
class AssignmentExpression;
class BinaryExpression;
class CallExpression;
class LiteralExpression;
class UnaryExpression;
class GetVariableExpression;

template<typename R>
class ExpressionVisitor {
public:
    virtual ~ExpressionVisitor() = default;

    virtual R visit(const LiteralExpression& expression) = 0;
    virtual R visit(const AssignmentExpression& expression) = 0;
    virtual R visit(const BinaryExpression& expression) = 0;
    virtual R visit(const UnaryExpression& expression) = 0;
    virtual R visit(const GetVariableExpression& expression) = 0;
    virtual R visit(const CallExpression& expression) = 0;
};
#endif //EXPRESSIONVISITOR_HPP
