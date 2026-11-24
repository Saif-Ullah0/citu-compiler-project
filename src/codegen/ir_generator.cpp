#include "codegen/ir_generator.h"

IRGenerator::IRGenerator(const std::vector<Token>& tokens) 
    : tokens(tokens), currentPos(0), tempCount(1) {}

Token IRGenerator::current() {
    if (currentPos >= tokens.size()) return tokens.back();
    return tokens[currentPos];
}

void IRGenerator::advance() {
    if (currentPos < tokens.size()) currentPos++;
}

bool IRGenerator::match(const std::string& lexeme) {
    if (current().lexeme == lexeme) {
        advance();
        return true;
    }
    return false;
}

std::string IRGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}

std::string IRGenerator::parseExpr(std::ostream& out) {
    std::string left = parseTerm(out);
    
    while (current().lexeme == "+" || current().lexeme == "-") {
        std::string op = current().lexeme;
        advance();
        std::string right = parseTerm(out);
        
        std::string temp = newTemp();
        out << temp << " = " << left << " " << op << " " << right << "\n";
        left = temp;
    }
    return left;
}

std::string IRGenerator::parseTerm(std::ostream& out) {
    std::string left = parseFactor(out);
    
    while (current().lexeme == "*" || current().lexeme == "/") {
        std::string op = current().lexeme;
        advance();
        std::string right = parseFactor(out);
        
        std::string temp = newTemp();
        out << temp << " = " << left << " " << op << " " << right << "\n";
        left = temp;
    }
    return left;
}

std::string IRGenerator::parseFactor(std::ostream& out) {
    Token t = current();
    
    if (t.type == TokenType::IDENTIFIER || t.type == TokenType::INT_CONST) {
        advance();
        return t.lexeme;
    }
    
    if (t.lexeme == "(") {
        advance();
        std::string val = parseExpr(out);
        match(")");
        return val;
    }
    return "ERROR";
}

void IRGenerator::generate(std::ostream& out) {
    while (currentPos < tokens.size() - 1) {
        Token t = current();

        // 1. Assignments: id = expr ;
        if (t.type == TokenType::IDENTIFIER && currentPos + 1 < tokens.size() && tokens[currentPos + 1].lexeme == "=") {
            std::string id = t.lexeme;
            advance(); // skip id
            advance(); // skip =
            
            std::string result = parseExpr(out);
            out << id << " = " << result << "\n";
            
            match(";");
        }
        // 2. Declarations: int x ; (Just skip for IR, as we only generate executable logic)
        else if (t.lexeme == "int" || t.lexeme == "char" || t.lexeme == "float") {
            advance();
            if (current().type == TokenType::IDENTIFIER) advance();
            match(";");
        }
        else {
            advance();
        }
    }
}