
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

#ifndef STATEMENTVISITOR_HPP
#define STATEMENTVISITOR_HPP

// Forward declarations
class IfStatement;
class ExpressionStatement;
class FunctionStatement;
class ReturnStatement;
class VariableStatement;
class BlockStatement;
class ClassStatement;

template<typename R>
class StatementVisitor {
public:
    virtual ~StatementVisitor() = default;

    virtual R visit(const IfStatement& statement) = 0;
    virtual R visit(const ExpressionStatement& statement) = 0;
    virtual R visit(const FunctionStatement& statement) = 0;
    virtual R visit(const ReturnStatement& statement) = 0;
    virtual R visit(const VariableStatement& statement) = 0;
    virtual R visit(const BlockStatement& statement) = 0;
    virtual R visit(const ClassStatement& statement) = 0;
};

#endif //STATEMENTVISITOR_HPP
