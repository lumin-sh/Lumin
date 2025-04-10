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

#include <format>
#include <Lexer.hpp>
#include <stdexcept>
#include <unordered_map>

using namespace Lumin::Compiler;

Lexer::Lexer( const std::string_view source ) : source( source ) {}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;

    while ( !IsAtEnd() ) {
        start = current;
        tokens.push_back( ScanToken() );
    }

    tokens.push_back( { TokenType::SPECIAL_END, "" } );

    return tokens;
}

Token Lexer::ScanToken() {
    switch (char c = Advance()) {
        case '+':
            if ( Match( '=' ) ) return MakeToken( TokenType::OPERATOR_PLUS_EQ );
            if ( Match( '+' ) ) return MakeToken( TokenType::OPERATOR_INCREMENT );
            return MakeToken( TokenType::OPERATOR_PLUS );
        case '-':
            if ( Match( '>' ) ) return MakeToken( TokenType::OPERATOR_ARROW );
            if ( Match( '=' ) ) return MakeToken( TokenType::OPERATOR_MINUS_EQ );
            if ( Match( '-' ) ) return MakeToken( TokenType::OPERATOR_DECREMENT );
            return MakeToken( TokenType::OPERATOR_MINUS );
        case '*': return MakeToken( Match( '=' ) ? TokenType::OPERATOR_MULTIPLY_EQ : TokenType::OPERATOR_MULTIPLY );
        case '/':
            if ( Match( '/' ) ) {
                while ( !IsAtEnd() && Peek() != '\n' ) Advance();
                return ScanToken();
            }
            if ( Match( '*' ) ) {
                while ( !IsAtEnd() && !( Peek() == '*' && PeekNext() == '/' ) ) {
                    Advance();
                }

                if ( IsAtEnd() ) {
                    throw std::runtime_error( "Unterminated multi-line comment" );
                }

                Advance();
                Advance();

                return ScanToken();
            }
            return MakeToken(Match('=') ? TokenType::OPERATOR_DIVIDE_EQ : TokenType::OPERATOR_DIVIDE);
        case '%': return MakeToken( Match( '=' ) ? TokenType::OPERATOR_MODULO_EQ : TokenType::OPERATOR_MODULO );
        case '(': return MakeToken( TokenType::PUNCTUATION_LPAREN );
        case ')': return MakeToken( TokenType::PUNCTUATION_RPAREN );
        case '{': return MakeToken( TokenType::PUNCTUATION_LBRACE );
        case '}': return MakeToken( TokenType::PUNCTUATION_RBRACE );
        case ':':
                return MakeToken( Match( ':' ) ? TokenType::OPERATOR_ACCESS : TokenType::PUNCTUATION_COLON );
        case ';': return MakeToken( TokenType::PUNCTUATION_SEMICOLON );
        case ',': return MakeToken( TokenType::PUNCTUATION_COMMA );
        case '.': return MakeToken( Match( '.' ) ? TokenType::OPERATOR_RANGE : TokenType::PUNCTUATION_DOT );
        case '=':
            return MakeToken( Match( '=' ) ? TokenType::OPERATOR_EQUALS : TokenType::OPERATOR_ASSIGN );
        case '>':
            if ( Match( '=' ) ) return MakeToken( TokenType::OPERATOR_GREATER_EQUALS );
            if ( Match( '>' ) ) {
                if ( Match( '>' ) ) return MakeToken( TokenType::OPERATOR_UNSIGNED_RIGHT_SHIFT );
                return MakeToken( TokenType::OPERATOR_RIGHT_SHIFT );
            }
            return MakeToken( TokenType::OPERATOR_GREATER_THAN );
        case '<':
            if ( Match( '=' ) ) return MakeToken( TokenType::OPERATOR_LESS_EQUALS );
            if ( Match( '<' ) ) return MakeToken( TokenType::OPERATOR_LEFT_SHIFT );
            return MakeToken( TokenType::OPERATOR_LESS_THAN );
        case '!':
            if ( Match( '=' ) ) return MakeToken( TokenType::OPERATOR_BANG_EQUALS );
            return MakeToken( TokenType::OPERATOR_BANG );
        case '&':
            if ( Match( '&' ) ) return MakeToken( TokenType::OPERATOR_LOGICAL_AND );
            return MakeToken( TokenType::OPERATOR_BITWISE_AND );
        case '|':
            if ( Match( '|' ) ) return MakeToken( TokenType::OPERATOR_LOGICAL_OR );
            return MakeToken( TokenType::OPERATOR_BITWISE_INC_OR );
        case '^': return MakeToken( TokenType::OPERATOR_BITWISE_EXC_OR );
        case '~': return MakeToken( TokenType::OPERATOR_BITWISE_NEGATE );
        case '$': return MakeToken( TokenType::PUNCTUATION_DOLLAR );
        case '_': return MakeToken( TokenType::PUNCTUATION_UNDERSCORE );
        case ' ':
        case '\r':
        case '\t':
        case '\n':
            return ScanToken();
        default:
            if ( std::isalpha( c ) ) return IdentifierToken();
        if ( std::isdigit( c ) ) return NumberToken();
        throw std::runtime_error( std::format( "Unexpected character: {}", c ) );
    }
}

bool Lexer::Match( const char expected ) {
    if ( IsAtEnd() || source[current] != expected ) return false;
    current++;
    return true;
}

Token Lexer::MakeToken( const TokenType type ) const {
    const auto lexeme = std::string( source.substr( start, current - start ) );
    Token token = { type, lexeme };
    return token;
}

Token Lexer::IdentifierToken() {
    while ( !IsAtEnd() && ( std::isalnum( Peek() ) || Peek() == '_' ) ) {
        Advance();
    }

    const auto text = source.substr(start, current - start);
    static const std::unordered_map<std::string_view, TokenType> keywords = {
        { "inline", TokenType::MODIFIER_INLINE },
        { "noinline", TokenType::MODIFIER_NOINLINE },
        { "private", TokenType::MODIFIER_PRIVATE },
        { "fun", TokenType::KEYWORD_FUN },
        { "native", TokenType::KEYWORD_NATIVE },
        { "async", TokenType::KEYWORD_ASYNC },
        { "var", TokenType::KEYWORD_VAR },
        { "val", TokenType::KEYWORD_VAL },
        { "if", TokenType::KEYWORD_IF },
        { "then", TokenType::KEYWORD_THEN },
        { "else", TokenType::KEYWORD_ELSE },
        { "for", TokenType::KEYWORD_FOR },
        { "in", TokenType::KEYWORD_IN },
        { "return", TokenType::KEYWORD_RETURN },
        { "try", TokenType::KEYWORD_TRY },
        { "catch", TokenType::KEYWORD_CATCH },
        { "int", TokenType::KEYWORD_INT },
        { "double", TokenType::KEYWORD_DOUBLE },
        { "float", TokenType::KEYWORD_FLOAT },
        { "long", TokenType::KEYWORD_LONG },
        { "bool", TokenType::KEYWORD_BOOL },
        { "void", TokenType::KEYWORD_VOID },
        { "char", TokenType::KEYWORD_CHAR },
        { "string", TokenType::KEYWORD_STRING },
        { "class", TokenType::KEYWORD_CLASS },
        { "interface", TokenType::KEYWORD_INTERFACE },
        { "enum", TokenType::KEYWORD_ENUM },
        { "extends", TokenType::KEYWORD_EXTENDS },
        { "implements", TokenType::KEYWORD_IMPLEMENTS },
        { "typealias", TokenType::KEYWORD_TYPEALIAS },
        { "import", TokenType::KEYWORD_IMPORT },
        { "namespace", TokenType::KEYWORD_NAMESPACE },
        { "this", TokenType::KEYWORD_THIS },
        { "is", TokenType::KEYWORD_IS },
        { "as", TokenType::KEYWORD_AS },
        { "match", TokenType::KEYWORD_MATCH },
        { "break", TokenType::KEYWORD_BREAK },
        { "continue", TokenType::KEYWORD_CONTINUE },
        { "throw", TokenType::KEYWORD_THROW },
        { "true", TokenType::LITERAL_BOOL },
        { "false", TokenType::LITERAL_BOOL }
    };

    const auto it = keywords.find( Trim( text ) );

    return { it != keywords.end() ? it->second : TokenType::LITERAL_IDENTIFIER, std::string( text ) };
}

Token Lexer::StringToken() {
    while ( !IsAtEnd() && Peek() != '"' ) {
        if ( Peek() == '\\' && !IsAtEnd() ) {
            Advance();
            Advance();
        } else {
            Advance();
        }
    }

    if ( IsAtEnd() ) {
        throw std::runtime_error( "Unterminated string literal" );
    }

    Advance();

    return MakeToken( TokenType::LITERAL_STRING );
}

Token Lexer::CharacterToken() {
    if ( IsAtEnd() ) {
        throw std::runtime_error( "Unterminated character literal" );
    }

    if ( Peek() == '\\' ) {
        Advance();
        if (IsAtEnd()) {
            throw std::runtime_error( "Incomplete escape sequence in character literal" );
        }
        Advance();
    } else {
        Advance();
    }

    if ( IsAtEnd() || Peek() != '\'' ) {
        throw std::runtime_error( "Unterminated character literal" );
    }

    Advance();

    return MakeToken( TokenType::LITERAL_CHAR );
}

Token Lexer::NumberToken() {
    bool isFloat = false;

    // handle int
    while ( !IsAtEnd() && std::isdigit( Peek() ) ) {
        Advance();
    }

    // handle decimal
    if ( !IsAtEnd() && Peek() == '.' && std::isdigit( Peek() ) ) {
        isFloat = true;
        Advance(); // consume '.'

        while ( !IsAtEnd() && std::isdigit( Peek() ) ) {
            Advance();
        }
    }

    // exponential
    if ( !IsAtEnd() && ( Peek() == 'e' || Peek() == 'E' ) ) {
        isFloat = true;
        Advance(); // consume 'e' or 'E'

        if ( !IsAtEnd() && ( Peek() == '+' || Peek() == '-' ) ) {
            Advance(); // consume the '+' or '-'
        }

        if ( !IsAtEnd() && std::isdigit( Peek() ) ) {
            while ( !IsAtEnd() && std::isdigit( Peek() ) ) {
                Advance();
            }
        } else {
            throw std::runtime_error( "Invalid number format: expected digit after exponent" );
        }
    }

    // type suffixes
    if (!IsAtEnd()) {
        const char c = Peek();

        if ( c == 'f' || c == 'F' ) {
            Advance();
            return MakeToken( TokenType::LITERAL_FLOAT );
        }

        if ( c == 'd' || c == 'D' ) {
            Advance();
            return MakeToken( TokenType::LITERAL_DOUBLE );
        }

        if ( c == 'l' || c == 'L' ) {
            Advance();
            return MakeToken( TokenType::LITERAL_LONG );
        }

    }

    return MakeToken( isFloat ? TokenType::LITERAL_FLOAT : TokenType::LITERAL_INT );
}

char Lexer::Peek() const {
    return IsAtEnd() ? '\0' : source[current];
}

char Lexer::PeekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}


char Lexer::Advance() {
    return source[current++];
}

bool Lexer::IsAtEnd() const {
    return current >= source.length();
}

std::string_view Lexer::Trim(const std::string_view &source) {
    const auto start = source.find_first_not_of(" \t\n\r");
    const auto end = source.find_last_not_of(" \t\n\r");
    return ( start == std::string_view::npos || end == std::string_view::npos )
        ? std::string_view{}
    : source.substr( start, end - start + 1 );
}