
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

#ifndef LUMIN_LEXER_HPP
#define LUMIN_LEXER_HPP

#include <string_view>
#include <vector>
#include <TokenType.hpp>

namespace Lumin::Compiler {

class Lexer {
public:
    explicit Lexer( std::string_view source );

    std::vector<Token> Tokenize();
private:
    Token ScanToken();
    Token IdentifierToken();
    Token NumberToken();
    Token StringToken();
    Token CharacterToken();
    bool Match( char expected );
    [[nodiscard]] char Peek() const;
    [[nodiscard]] char PeekNext() const;
    char Advance();
    Token MakeToken( TokenType type ) const;
    bool IsAtEnd() const;
    static std::string_view Trim( const std::string_view& source );

    std::string_view source;
    size_t start = 0;
    size_t current = 0;
};

}

#endif //LUMIN_LEXER_HPP
