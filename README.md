# tlo-cpp

A C++ library.

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
