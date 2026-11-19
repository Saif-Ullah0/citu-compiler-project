#include "semantic/semantic_analyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token>& tokens) 
    : tokens(tokens), currentPos(0) {}

Token SemanticAnalyzer::current() {
    // Your fix: Cast size to int for comparison
    if (currentPos >= (int)tokens.size()) return tokens.back();
    return tokens[currentPos];
}

void SemanticAnalyzer::advance() {
    if (currentPos < (int)tokens.size()) currentPos++;
}

bool SemanticAnalyzer::analyze(std::ostream& out) {
    bool hasError = false;
    
    // Your fix:
    int totalTokens = tokens.size();

    // Loop through all tokens (ignoring END_OF_FILE)
    while (currentPos < totalTokens - 1) {
        Token t = current();

        // 1. Scope Management
        if (t.lexeme == "shro") {
            symbolTable.enterScope();
            advance();
        } 
        else if (t.lexeme == "khatam") {
            symbolTable.exitScope();
            advance();
        }
        // 2. Declarations
        else if (t.lexeme == "int" || t.lexeme == "char" || t.lexeme == "float" || t.lexeme == "struct") {
            std::string type = t.lexeme;
            advance(); 
            
            if (current().type == TokenType::IDENTIFIER) {
                std::string idName = current().lexeme;
                if (!symbolTable.insert(idName, type, current().line)) {
                    hasError = true;
                }
                advance();
            }
        }
        // 3. Usage
        else if (t.type == TokenType::IDENTIFIER) {
            // Your fix:
            if (currentPos + 1 < totalTokens && tokens[currentPos + 1].lexeme == "=") {
                if (!symbolTable.lookup(t.lexeme)) {
                    std::cerr << "Semantic Error (Line " << t.line << "): Undeclared variable '" << t.lexeme << "'\n";
                    hasError = true;
                }
            }
            advance();
        } 
        else {
            advance();
        }
    }

    symbolTable.printToFile(out);
    return !hasError; 
}