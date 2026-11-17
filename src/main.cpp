#include "lexer/lexer.h"
#include "utils/file_io.h"
#include "parser/table_parser.h"
#include <iostream>
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
            
            // 2. Then Parse
            bool useLL1 = (mode == "parseLL1");
            TableParser parser(tokens, useLL1);
            
            std::cout << "[Phase 2] Table-Driven Parsing (" << mode << ")...\n\n";
            if (parser.parse()) {
                std::cout << "\nParsing Successful!\n";
            } else {
                std::cerr << "\nParsing Failed.\n";
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