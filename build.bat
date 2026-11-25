@echo off
if not exist build mkdir build

g++ -std=c++14 -Wall -Wextra -Iinclude ^
    src\main.cpp ^
    src\lexer\lexer.cpp ^
    src\lexer\keyword_table.cpp ^
    src\utils\file_io.cpp ^
    src\parser\table_parser.cpp ^
    src\semantic\semantic_analyzer.cpp ^
    src\codegen\ir_generator.cpp ^
    -o build\compiler.exe

if %ERRORLEVEL% EQU 0 (
    echo [OK] build\compiler.exe
) else (
    echo [FAIL] exit code %ERRORLEVEL%
)