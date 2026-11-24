#pragma once
#include "common/token.h"
#include <vector>
#include <string>
#include <iostream>

class IRGenerator {
public:
    IRGenerator(const std::vector<Token>& tokens);
    void generate(std::ostream& out);

private:
    std::vector<Token> tokens;
    size_t currentPos;
    int tempCount;

    // Helpers for expression parsing
    std::string newTemp();
    std::string parseExpr(std::ostream& out);
    std::string parseTerm(std::ostream& out);
    std::string parseFactor(std::ostream& out);

    // Token helpers
    Token current();
    void advance();
    bool match(const std::string& lexeme);
};