# CS426 Compiler Construction - Project Phase 1

**Student ID:** BSCS23065  
**Student Name:** Saif Ullah  
**Repository URL:** https://github.com/Saif-Ullah0/citu-compiler-pipeline

---

## 1. Overview

This phase focuses on implementing and demonstrating the core structure of a custom compiler pipeline using a C-like language syntax. The examples below illustrate the supported language constructs, including variable declarations, loops, conditions, functions, and structs.

---

## 2. Example Programs (3)

### Program 1: Basic Logic (While Loop & If-Else)

This program demonstrates variable declaration, arithmetic operations, a while loop, and if-else branching.

```c
main shro
    int limit;
    int i;
    int sum;

    limit = 5;
    i = 0;
    sum = 0;

    jabtak (i < limit) shro
        agar (i > 2) shro
            sum = sum + i;
        khatam
        magar shro
            sum = sum - i;
        khatam
        i = i + 1;
    khatam
khatam
```

### Program 2: Functions and Parameters

This program demonstrates function definition, parameter passing, and return values.

```c
int add(int a, int b) shro
    int result;
    result = a + b;
    return result;
khatam

main shro
    int x;
    x = add(10, 20);
khatam
```

### Program 3: Structs (No Classes)

This program demonstrates a struct definition and usage. Classes and do-while loops are restricted in this project design.

```c
struct Point shro
    int x;
    int y;
khatam

main shro
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
khatam
```

---

## 3. Summary

These examples cover the fundamental constructs required for the compiler front-end: lexical structure, parsing of statements, control flow, function support, and structured data handling. The project is designed to progressively build a working compiler pipeline for a custom language.
