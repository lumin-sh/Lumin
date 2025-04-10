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

#include <algorithm>
#include <iostream>
#include <Parser.hpp>
#include <Logging.hpp>

using namespace Lumin::Compiler;

Parser::Parser( std::vector<Token> tokens) :
    tokens( std::move( tokens ) ),
    current( 0 ) {}

std::vector<std::unique_ptr<Statement>> Parser::Parse() {

    std::vector<std::unique_ptr<Statement>> statements;
    while ( !IsAtEnd() ) {
        if ( auto stmt = ParseDeclaration() ) {
            statements.push_back( std::move( stmt ) );
        } else {
            LOG_DEBUG( "Parsed a null statement!" )
        }
    }

    return statements;
}

std::unique_ptr<Statement> Parser::ParseDeclaration() {
    try {
        const AccessModifier accessModifier = ParseAccessModifier();
        const InlineSpecifier inlineSpec = ParseInlineSpecifier();

        if ( Match( { TokenType::KEYWORD_FUN } ) ) {
            return ParseFunctionDeclaration( accessModifier, inlineSpec );
        }

        if ( Match( { TokenType::KEYWORD_VAR } ) ) {
            return ParseVariableDeclaration( accessModifier );
        }

        if( Match( { TokenType::KEYWORD_VAL } ) ) {
            return ParseVariableDeclaration( accessModifier ); // todo: make this pass a mutability parameter?
        }

        if ( accessModifier != AccessModifier::NONE ) {
            throw std::runtime_error( "Expected declaration after access modifier" );
        }

        return ParseStatement();

    } catch ( const std::exception& e ) {
        std::cerr << "ParseDecl error: " << e.what() << " @ " << current << std::endl;

        Synchronize();
        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<Statement> Parser::ParseStatement() {
    //if ( Match( { TokenType::KEYWORD_IF } ) ) return parseIfStatement();
    if ( Match( { TokenType::KEYWORD_RETURN} ) ) return ParseReturnStatement();
    return ParseExpressionStatement();
}

std::unique_ptr<Statement> Parser::ParseExpressionStatement() {
    auto expr = ParseExpression();
    Consume( TokenType::PUNCTUATION_SEMICOLON, "Expect ';' after expression" );
    return std::make_unique<ExpressionStatement>( std::move( expr ) );
}


std::unique_ptr<Statement> Parser::ParseReturnStatement() {
    std::unique_ptr<Expression> value;
    if ( !Check( TokenType::PUNCTUATION_SEMICOLON ) ) {
        value = ParseExpression();
    }

    Consume( TokenType::PUNCTUATION_SEMICOLON, "Expect ';' after return value" );
    return std::make_unique<ReturnStatement>( std::move( value ) );
}


std::unique_ptr<Statement> Parser::ParseVariableDeclaration( AccessModifier access ) {
    std::string name = Consume(
        TokenType::LITERAL_IDENTIFIER,
        "Expected variable name"
    ).lexeme;

    std::unique_ptr<Expression> initializer;
    if ( Match( { TokenType::OPERATOR_ASSIGN } ) ) {
        initializer = ParseExpression();
    }

    Consume( TokenType::PUNCTUATION_SEMICOLON, "Expect ';' after variable declaration" );

    return std::make_unique<VariableStatement>( name, access, std::move(initializer) );

}

std::unique_ptr<Statement> Parser::ParseFunctionDeclaration( AccessModifier access, InlineSpecifier inlineSpec ) {
    std::string name = Consume( TokenType::LITERAL_IDENTIFIER, "Expect function name" ).lexeme;

    Consume( TokenType::PUNCTUATION_LPAREN, "Expect '(' after function name" );
    std::vector<std::pair<std::string, TokenType>> parameters;

    if ( !Check( TokenType::PUNCTUATION_RPAREN ) ) {
        do {
            std::string paramName = Consume( TokenType::LITERAL_IDENTIFIER, "Expect parameter name" ).lexeme;
            Consume( TokenType::PUNCTUATION_COLON, "Expect ':' after parameter name" );

            auto paramType = TokenType::SPECIAL_ERROR;
            if (Match({
                TokenType::KEYWORD_BOOL,
                TokenType::KEYWORD_INT,
                TokenType::KEYWORD_LONG,
                TokenType::KEYWORD_FLOAT,
                TokenType::KEYWORD_DOUBLE,
                TokenType::KEYWORD_STRING,
                TokenType::KEYWORD_CHAR,
                TokenType::KEYWORD_VOID
            })) {
                paramType = Previous().type;
            } else {
                throw std::runtime_error( "Expected type after ':'" );
            }

            parameters.emplace_back( paramName, paramType );
        } while ( Match( { TokenType::PUNCTUATION_COMMA } ) );
    }

    Consume(TokenType::PUNCTUATION_RPAREN, "Expect ')' after parameters");
    Consume(TokenType::PUNCTUATION_LBRACE, "Expect '{' before function body");

    auto body = ParseBlock();

    return std::make_unique<FunctionStatement>(name, access, inlineSpec, std::move(parameters), std::move(body));
}

std::vector<std::unique_ptr<Statement>> Parser::ParseBlock() {
    std::vector<std::unique_ptr<Statement>> statements;

    while ( !Check( TokenType::PUNCTUATION_RBRACE ) && !IsAtEnd() ) {
        statements.push_back( ParseDeclaration() );
    }

    Consume( TokenType::PUNCTUATION_RBRACE, "Expect '}' after block" );
    return statements;
}

std::unique_ptr<Expression> Parser::ParseExpression() {
    return ParseAssignment();
}

std::unique_ptr<Expression> Parser::ParseAssignment() {
    auto expr = ParseEquality();

    if ( Match( { TokenType::OPERATOR_EQUALS } ) ) {
        auto value = ParseAssignment();
        if ( auto* variable = dynamic_cast<GetVariableExpression*>( expr.get() ) ) {
            return std::make_unique<AssignmentExpression>( variable->name, std::move( value ) );
        }
        throw std::runtime_error("Invalid assignment target");
    }

    return expr;
}

std::unique_ptr<Expression> Parser::ParseEquality() {
    auto expr = ParseComparison();

    while ( Match( { TokenType::OPERATOR_EQUALS, TokenType::OPERATOR_BANG_EQUALS } ) ) {
        TokenType op = Previous().type;
        auto right = ParseComparison();
        expr = std::make_unique<BinaryExpression>( std::move( expr ), op, std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expression> Parser::ParseComparison() {
    auto expr = ParseAdditive();

    while ( Match( { TokenType::OPERATOR_GREATER_THAN, TokenType::OPERATOR_GREATER_EQUALS,
                  TokenType::OPERATOR_LESS_THAN, TokenType::OPERATOR_LESS_EQUALS } ) ) {
        TokenType op = Previous().type;
        auto right = ParseAdditive();
        expr = std::make_unique<BinaryExpression>( std::move( expr ), op, std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expression> Parser::ParseAdditive() {
    auto expr = ParseMultiplicative();

    while ( Match( { TokenType::OPERATOR_PLUS, TokenType::OPERATOR_MINUS } ) ) {
        TokenType op = Previous().type;
        auto right = ParseMultiplicative();
        expr = std::make_unique<BinaryExpression>( std::move( expr ), op, std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expression> Parser::ParseMultiplicative() {
    auto expr = ParseUnary();

    while ( Match( { TokenType::OPERATOR_MULTIPLY, TokenType::OPERATOR_DIVIDE } ) ) {
        TokenType op = Previous().type;
        auto right = ParseUnary();
        expr = std::make_unique<BinaryExpression>( std::move( expr ), op, std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expression> Parser::ParseUnary() {
    if ( Match( { TokenType::OPERATOR_BANG, TokenType::OPERATOR_MINUS } ) ) {
        TokenType op = Previous().type;
        auto right = ParseUnary();
        return std::make_unique<UnaryExpression>( op, std::move( right ) );
    }

    return ParsePrimary();
}

std::unique_ptr<Expression> Parser::ParsePrimary() {

    if ( Match( { TokenType::LITERAL_DOUBLE } ) ) {
        return std::make_unique<LiteralExpression>( std::stod( Previous().lexeme ) );
    }

    if ( Match( { TokenType::LITERAL_FLOAT } ) ) {
        return std::make_unique<LiteralExpression>( std::stof( Previous().lexeme ) );
    }

    if ( Match( { TokenType::LITERAL_INT } ) ) {
        return std::make_unique<LiteralExpression>( std::stoi( Previous().lexeme ) );
    }

    if ( Match( { TokenType::LITERAL_LONG } ) ) {
        return std::make_unique<LiteralExpression>( std::stoll( Previous().lexeme ) );
    }

    if ( Match ( { TokenType::LITERAL_BOOL } ) ) {
        const std::string value = Previous().lexeme;
        if ( value == "true" ) {
            return std::make_unique<LiteralExpression>( true );
        }

        return std::make_unique<LiteralExpression>( false );
    }


    if ( Match( { TokenType::LITERAL_IDENTIFIER } ) ) {
        // Check if this is a function call
        if ( Check( TokenType::PUNCTUATION_LPAREN ) ) {
            std::string functionName = Previous().lexeme;

            Consume( TokenType::PUNCTUATION_LPAREN, "Expect '(' after function name" );

            std::vector<std::unique_ptr<Expression>> arguments;
            if ( !Check( TokenType::PUNCTUATION_RPAREN ) ) {
                do {
                    arguments.push_back( ParseExpression() );
                } while ( Match( { TokenType::PUNCTUATION_COMMA } ) );
            }

            Consume( TokenType::PUNCTUATION_RPAREN, "Expect ')' after arguments" );

            return std::make_unique<CallExpression>( functionName, std::move( arguments ) );
        }

        return std::make_unique<GetVariableExpression>( Previous().lexeme );
    }

    if ( Match( { TokenType::PUNCTUATION_LPAREN } ) ) {
        auto expr = ParseExpression();
        Consume( TokenType::PUNCTUATION_RPAREN, "Expect ')' after expression" );
        return expr;
    }

    throw std::runtime_error("Expect expression");
}

AccessModifier Parser::ParseAccessModifier() {
    if( Match( { TokenType::MODIFIER_PRIVATE } ) ) return AccessModifier::PRIVATE;
    if( Match( { TokenType::MODIFIER_INTERNAL } ) ) return AccessModifier::INTERNAL;
    if( Match( { TokenType::MODIFIER_PUBLIC } ) ) return AccessModifier::PUBLIC;
    return AccessModifier::NONE;
}

InlineSpecifier Parser::ParseInlineSpecifier() {
    if ( Match( { TokenType::MODIFIER_INLINE } ) ) return InlineSpecifier::INLINE;
    if ( Match( { TokenType::MODIFIER_NOINLINE } ) ) return InlineSpecifier::NOINLINE;
    return InlineSpecifier::NONE;
}

bool Parser::Check( const TokenType type ) const {
    if ( IsAtEnd() ) return false;
    return tokens[current].type == type;
}

Token Parser::Consume( const TokenType type, const std::string& message ) {
    if ( Check( type ) ) return Advance();
    throw std::runtime_error( message );
}

Token Parser::Advance() {
    if ( !IsAtEnd() ) current++;
    return Previous();
}

bool Parser::Match(std::initializer_list<TokenType> types) {
    if ( std::ranges::any_of( types, [this]( const TokenType type ) { return Check(type); } ) ) {
        Advance();
        return true;
    }
    return false;
}

Token& Parser::Previous() {
    return tokens[current - 1];
}

bool Parser::IsAtEnd() const {
    return tokens[current].type == TokenType::SPECIAL_END;
}

void Parser::Synchronize() {
    Advance();

    while ( !IsAtEnd() ) {
        if ( Previous().type == TokenType::PUNCTUATION_SEMICOLON ) return;

        switch ( tokens[current].type ) {
            case TokenType::KEYWORD_CLASS:
            case TokenType::KEYWORD_FUN:
            case TokenType::KEYWORD_VAR:
            case TokenType::KEYWORD_FOR:
            case TokenType::KEYWORD_IF:
            case TokenType::KEYWORD_RETURN:
                return;
            default:
                break;
        }

        Advance();
    }
}
