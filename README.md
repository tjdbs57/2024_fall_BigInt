# 2024 Fall BigInt Project

## Overview
This project implements a Big Integer (BigInt) library that supports multi-precision arithmetic operations, such as addition, subtraction, multiplication, and division. The goal is to handle large numbers that cannot fit within standard data types in C.

## Project Structure

```plaintext
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
  ├── measurement_cycles( 8bit multiplication).png
  ├── measurement_cycles(32bit multiplication).png
  ├── measurement_cycles(64bit multiplication).png
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

### System Requirements
- **GCC**: A C compiler (e.g., `gcc`).
- **Make**: A build automation tool.
- **Python**: Python 3 is required for running test scripts.

### Python Libraries
The Python scripts used in this project depend on the following libraries:
- **matplotlib**: Required for generating plots.

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

## Dynamic Library 
Running the following command will compile the project and generate the dynamic library:

```bash
make all
```

## Notes
The Makefile automatically detects your operating system and adjusts file naming conventions accordingly (e.g., .exe on Windows or .so on Linux).

Python 3 is required to run the test scripts (test_all.py, time.py).

Ensure valgrind is installed on your system if you want to perform memory leak detection.


# Diffie-Hellman Key Exchange Program

This project implements a Diffie-Hellman key exchange program with a server-client architecture. The build process and execution are automated using the provided `Makefile`.

## Prerequisites

Ensure the following tools and libraries are installed on your system before proceeding:

### System Requirements
- **GCC**: A C compiler (e.g., `gcc`).
- **Make**: A build automation tool.

### Required Libraries
- **GMP (GNU Multiple Precision Arithmetic Library)**: Required for handling large integers.
- **libbigint_64bit**: A custom library used by the program. This library should be located in the `../lib` directory relative to this project.

## Setting Library Path
Before running the program, ensure that the libbigint_64bit library is accessible by the system. You can do this by setting the LD_LIBRARY_PATH as follows:

```bash
export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
```
This command adds the ../lib directory to the library search path, allowing the program to find libbigint_64bit.so at runtime.

## Build Instructions

### 1. Compile the Project
To compile both the server and client programs, run:

```bash
make
```

### 2. Clean Build Artifacts
To remove all compiled files and directories created during the build process, use:

```bash
make clean
```

### 3. Rebuild the Project
To clean and recompile the project from scratch, run:

```bash
make rebuild
```

## Program Execution

### Running the Server
To start the Diffie-Hellman key exchange server, use the following command:

```bash
make run_server
```

### Running the Client
To start the Diffie-Hellman key exchange client, use the following command:

```bash
make run_client
```

## Notes
The Makefile automatically creates the necessary directories (obj and bin) during the build process.

The libbigint_64bit library must be present in the ../lib directory, and the header files should be located in the ../include directory.

Ensure the GMP library is installed on your system. On Debian/Ubuntu, it can be installed with:

```bash
sudo apt-get install libgmp-dev
```

The server and client executables are placed in the bin directory as server and client, respectively. For Linux/Unix systems, they can be executed directly, while .exe extensions are used for Windows builds.