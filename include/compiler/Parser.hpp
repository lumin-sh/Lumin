
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

#ifndef LUMIN_PARSER_HPP
#define LUMIN_PARSER_HPP

#include <memory>
#include <vector>
#include <TokenType.hpp>
#include <statements/VariableStatement.hpp>
#include <statements/FunctionStatement.hpp>
#include <statements/ReturnStatement.hpp>
#include <statements/ExpressionStatement.hpp>
#include <expressions/GetVariableExpression.hpp>
#include <expressions/AssignmentExpression.hpp>
#include <expressions/BinaryExpression.hpp>
#include <expressions/UnaryExpression.hpp>
#include <expressions/LiteralExpression.hpp>
#include <expressions/CallExpression.hpp>

namespace Lumin::Compiler {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Statement>> Parse();
private:
    // Declaration parsing methods
    std::unique_ptr<Statement> ParseDeclaration();
    std::unique_ptr<Statement> ParseFunctionDeclaration(AccessModifier access, InlineSpecifier inlineSpec);
    std::unique_ptr<Statement> ParseVariableDeclaration(AccessModifier access);

    // Statement parsing methods
    std::unique_ptr<Statement> ParseStatement();
    std::unique_ptr<Statement> ParseReturnStatement();
    std::unique_ptr<Statement> ParseExpressionStatement();
    std::vector<std::unique_ptr<Statement>> ParseBlock();
    // Expression parsing methods
    std::unique_ptr<Expression> ParseExpression();
    std::unique_ptr<Expression> ParseAssignment();
    std::unique_ptr<Expression> ParseEquality();
    std::unique_ptr<Expression> ParseComparison();
    std::unique_ptr<Expression> ParseAdditive();
    std::unique_ptr<Expression> ParseMultiplicative();
    std::unique_ptr<Expression> ParseUnary();
    std::unique_ptr<Expression> ParsePrimary();

    // Modifier/Specifiers parsing methods
    AccessModifier ParseAccessModifier();
    InlineSpecifier ParseInlineSpecifier();

    // Helper
    bool Match(std::initializer_list<TokenType> types);
    bool Check(TokenType type) const;
    Token Consume(TokenType type, const std::string& message);
    Token Advance();
    bool IsAtEnd() const;
    Token& Previous();
    void Synchronize();


    std::vector<Token> tokens;
    size_t current;
};

}

#endif //LUMIN_PARSER_HPP
