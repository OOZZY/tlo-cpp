# tlo-cpp

A C++ library.

## Library Components

* A unit testing framework
* Implementations of a few dynamic programming algorithms:
    * Longest common subsequence length
    * Longest common subsequence distance
    * Levenshtein distance
    * Damerau-Levenshtein distance
* Some utility functions on top of `std::filesystem`, `std::string`, and `std::chrono`
* A class for parsing command-line arguments
* Wrapper classes encapsulating SQLite 3 objects and functions

## Build Requirements

* CMake
* C++17 development environment for which CMake can generate build files
* SQLite 3

## Clone, Build, and Test

Clone into tlo-cpp directory.

```
$ git clone --branch develop git@github.com:OOZZY/tlo-cpp.git
```

Build.

```
$ mkdir build
$ cd build
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tlo-cpp
$ make
```

Run tests with make.

```
$ make test
```

Run tests directly.

```
$ ./tlo-cpp-test
```

## CMake Options

* TLO\_CPP_\_COLORED\_DIAGNOSTICS
    * Tell the compiler to use colors in diagnostics (GNU/Clang only)
    * On by default
* TLO\_CPP_\_USE\_LIBCPP
    * Use libc++ (Clang only)
    * Off by default
* TLO\_CPP_\_LINK\_FS
    * Link to filesystem library of older GNU and Clang (GNU/Clang only)
    * Prior to LLVM 9, using `std::filesystem` required linker option `-lc++fs`
    * Prior to GCC 9, using `std::filesystem` required linker option `-lstdc++fs`
    * Off by default
