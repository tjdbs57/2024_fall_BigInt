# 2024_fall_BigInt
/project-root
│
├── /bin                // Output binary files 
│   └── (empty initially)
│
├── /obj                // Compiled object files
│   └── (empty initially)
│
├── /include            // Header files (.h)
│   ├── config.h           
│   ├── bigint.h           
│   └── utils.h    
│
├── /src                // Source files (.c)
│   ├── bigint.c        // Multi-precision
│   ├── utils.c         // Utility
│   └── main.c          // Main program
│
├── /tests              // Unit tests
│   ├── bigint_tests.c     
│   └── utils_tests.c     
│
├── /libs               // Library (optional)
│   ├── lib.a           // Static Library (Linux)
│   ├── lib.so          // Dynamic Library (Linux)
│   ├── lib.lib         // Static Library (Windows)
│   └── lib.dll         // Dynamic Library (Windows)
│
└── Makefile            // Building the project