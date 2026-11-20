#include "lexer/lexer.h"
#include "utils/file_io.h"
#include "parser/table_parser.h"
#include "semantic/semantic_analyzer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <mode> <input_file> <output_file>\n";
        std::cerr << "Modes: lex, parseLL1, parseSLR1\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    try {
        std::string source = readFile(inputPath);

        if (mode == "lex") {
            std::cout << "[Phase 1] Lexical Analysis...\n";
            Lexer lexer(source);
            std::vector<Token> tokens = lexer.tokenize();
            writeTokens(outputPath, tokens);
            std::cout << "Success: Tokens written to " << outputPath << "\n";
        } 
        else if (mode == "parseLL1" || mode == "parseSLR1") {
            // 1. First, Lex the input into tokens
            Lexer lexer(source);
            std::vector<Token> tokens = lexer.tokenize();

            std::ofstream outFile(outputPath);
            if (!outFile.is_open()) {
                throw std::runtime_error("Cannot open output file: " + outputPath);
            }
            
            // 2. Then Parse
            bool useLL1 = (mode == "parseLL1");
            TableParser parser(tokens, useLL1);
            
            std::cout << "[Phase 2] Parsing (" << mode << ")... Check " << outputPath << " for trace.\n";
            bool success = (mode == "parseLL1") ? parser.parse(outFile) : parser.parseSLR1(outFile);
            if (success){
                std::cout << "Parsing Successful!\n";
            } else {
                std::cerr << "Parsing Failed. See " << outputPath << "\n";
                return 1;
            }
        }
                else if (mode == "semantic") {
            Lexer lexer(source);
            std::vector<Token> tokens = lexer.tokenize();
            
            std::ofstream outFile(outputPath);
            if (!outFile.is_open()) {
                throw std::runtime_error("Cannot open output file: " + outputPath);
            }

            std::cout << "[Phase 3] Semantic Analysis... Check " << outputPath << "\n";
            SemanticAnalyzer analyzer(tokens);
            
            if (analyzer.analyze(outFile)) {
                std::cout << "Semantic Analysis Successful (No errors).\n";
            } else {
                std::cerr << "Semantic Analysis Failed (Errors found).\n";
                return 1;
            }
        }
        else {
            std::cerr << "Unknown mode: " << mode << "\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}