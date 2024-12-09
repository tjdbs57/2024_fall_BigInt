# 2024 Fall BigInt Project

## Overview
This project implements a Big Integer (BigInt) library that supports multi-precision arithmetic operations, such as addition, subtraction, multiplication, and division. The goal is to handle large numbers that cannot fit within standard data types in C.

## Project Structure

```plaintext
.vscode/
dh/
  ├── lib/
  │   └── libbigint_64bit.so
  ├── src/
  │   ├── client.c
  │   ├── prime.c
  │   ├── prime.h
  │   └── server.c
  └── Makefile
doxygen/
  ├── html/
  └── Doxyfile
graph/
  ├── measurement_cycle...
  ├── measurement_cycle...
  └── success_rate_plot.png
include/
  ├── arithmetic.h
  ├── bigint.h
  ├── config.h
  ├── test.h
  └── utils.h
lib/
  └── libbigint_64bit.so
src/
  ├── arithmetic.c
  ├── main.c
  └── utils.c
test/
  ├── cal.py
  ├── cycle_single.txt
  ├── cycle.txt
  ├── test_all.py
  ├── test.c
  └── time.py
trash/
.gitignore
LICENSE
Makefile
README.md
```

# Project Build and Execution Guide

This document explains how to compile and run the project using the provided `Makefile`.

## Prerequisites

Ensure the following tools are installed on your system before proceeding:
- **GCC**: A C compiler (e.g., `gcc`).
- **Make**: A build automation tool.
- **Python**: Python 3 is required for running test scripts.

## Build Instructions

The project uses a `Makefile` to automate the build process. Follow the steps below:

### 1. Compile the Project
To compile the project, run the following command:

```bash
make
```
### 2. Clean Build Artifacts
To remove all compiled files and directories created during the build process, run:

```bash
make clean
```

### 3. Rebuild the Project
To clean the build artifacts and recompile the project from scratch, use:

```bash
make rebuild
```

### 4. Run the Executable
Once the project is successfully compiled, you can run the program using:

```bash
make run
```

## Testing and Verification

### 1. Running Tests
To verify the correctness of the project using test scripts, run:

```bash
make verify
```

## Performance Measurement

### 1. Memeory Leak Detection
To check for memory leaks (for Linux/Unix systems), run:

```bash
make valgrind
```

### 2. Execution Time Measurement
To measure the execution time of the program, use:

```bash
make measure
```

### 3. Performance Comparison
To compare the program's performance across different executions, run:

```bash
make compare
```

## Notes
The Makefile automatically detects your operating system and adjusts file naming conventions accordingly (e.g., .exe on Windows or .so on Linux).

Python 3 is required to run the test scripts (test_all.py, time.py).

Ensure valgrind is installed on your system if you want to perform memory leak detection.
