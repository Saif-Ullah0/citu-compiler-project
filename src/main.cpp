#include "lexer/lexer.h"
#include "utils/file_io.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <mode> <input_file> <output_file>\n";
        std::cerr << "Modes available: lex, parseLL1, parseSLR1, all\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    try {
        // Read the source file
        std::string source = readFile(inputPath);

        if (mode == "lex" || mode == "all") {
            std::cout << "[Phase 1] Lexical Analysis...\n";
            Lexer lexer(source);
            std::vector<Token> tokens = lexer.tokenize();
            writeTokens(outputPath, tokens);
            std::cout << "Success: Tokens written to " << outputPath << "\n";
        } 
        else if (mode == "parseLL1" || mode == "parseSLR1") {
            std::cout << "Parser not implemented yet. Use 'lex' mode.\n";
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