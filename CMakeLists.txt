cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project("tlo-cpp")
enable_testing()

set(private_compile_options "")
set(private_compile_definitions "")
set(public_compile_options "")
set(public_include_directories include)
set(public_link_libraries "")

if (UNIX)
  # Threads required. See CMake documentation on FindThreads module.
  set(CMAKE_THREAD_PREFER_PTHREAD ON)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  list(APPEND public_link_libraries Threads::Threads)
endif()

macro(string_option name docstring default_value)
  if (NOT DEFINED ${name} OR DEFINED CACHE{${name}})
    set(${name} "${default_value}" CACHE STRING "${docstring}")
  endif()
endmacro(string_option)

string_option(TLO_CPP_SQLITE3_INCLUDE_DIRS
  "Include directories containing SQLite 3 headers." ""
)

string_option(TLO_CPP_SQLITE3_LIBRARIES
  "Libraries to link against to use SQLite 3." ""
)

if ("${TLO_CPP_SQLITE3_INCLUDE_DIRS}" STREQUAL "" OR
    "${TLO_CPP_SQLITE3_LIBRARIES}" STREQUAL "")
  find_package(SQLite3 REQUIRED)
  list(APPEND public_link_libraries SQLite::SQLite3)
else()
  add_library(sqlite INTERFACE)
  target_include_directories(sqlite SYSTEM
    INTERFACE ${TLO_CPP_SQLITE3_INCLUDE_DIRS}
  )
  target_link_libraries(sqlite INTERFACE ${TLO_CPP_SQLITE3_LIBRARIES})

  if (UNIX)
    # On Linux, programs using sqlite3 need to be linked with -pthread and -ldl.
    target_link_libraries(sqlite INTERFACE ${CMAKE_DL_LIBS})
  endif()

  list(APPEND public_link_libraries sqlite)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  list(APPEND private_compile_options
    -pedantic
    -Wall
    -Werror
    -Wextra
  )
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  list(APPEND private_compile_options
    -Weverything
    -Wno-c++98-compat
    -Wno-exit-time-destructors
    -Wno-global-constructors
    -Wno-padded
  )
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  list(APPEND private_compile_options
    /Wall
    /wd4514
    /wd4571
    /wd4625
    /wd4626
    /wd4668
    /wd4710
    /wd4711
    /wd4820
    /wd5026
    /wd5027
    /wd5039
    /wd5045
    /WX
  )

  list(APPEND private_compile_definitions _CRT_SECURE_NO_WARNINGS)
endif()

option(TLO_CPP_COLORED_DIAGNOSTICS
  "Tell the compiler to use colors in diagnostics (GNU/Clang only)." ON
)
if (TLO_CPP_COLORED_DIAGNOSTICS)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    list(APPEND private_compile_options -fdiagnostics-color)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND private_compile_options -fcolor-diagnostics)
  endif()
endif()

option(TLO_CPP_USE_LIBCPP "Use libc++ (Clang only)." OFF)
if (TLO_CPP_USE_LIBCPP)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND public_compile_options -stdlib=libc++)
    list(APPEND public_link_libraries c++)
  endif()
endif()

option(TLO_CPP_LINK_FS
  "Link to filesystem library of older GNU and Clang (GNU/Clang only)." OFF
)
if (TLO_CPP_LINK_FS)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Prior to GCC 9, using std::filesystem required linker option -lstdc++fs.
    list(APPEND public_link_libraries stdc++fs)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (TLO_CPP_USE_LIBCPP)
      # Prior to LLVM 9, using std::filesystem required linker option -lc++fs.
      list(APPEND public_link_libraries c++fs)
    else()
      list(APPEND public_link_libraries stdc++fs)
    endif()
  endif()
endif()

macro(prepend listVar prefix)
  set(${listVar} "")
  foreach(item ${ARGN})
    list(APPEND ${listVar} "${prefix}${item}")
  endforeach(item)
endmacro(prepend)

set(tlo_cpp_headers
  chrono.hpp
  command-line.hpp
  damerau-levenshtein.hpp
  filesystem.hpp
  hash.hpp
  lcs.hpp
  levenshtein.hpp
  sqlite3.hpp
  stop.hpp
  string.hpp
)
prepend(tlo_cpp_headers include/tlo-cpp/ ${tlo_cpp_headers})

set(tlo_cpp_private_headers container.hpp)
prepend(tlo_cpp_private_headers src/ ${tlo_cpp_private_headers})

set(tlo_cpp_sources
  chrono.cpp
  command-line.cpp
  container.cpp
  damerau-levenshtein.cpp
  filesystem.cpp
  hash.cpp
  lcs.cpp
  levenshtein.cpp
  sqlite3.cpp
  stop.cpp
  string.cpp
)
prepend(tlo_cpp_sources src/ ${tlo_cpp_sources})

add_library(tlo-cpp STATIC
  ${tlo_cpp_headers}
  ${tlo_cpp_private_headers}
  ${tlo_cpp_sources}
)
set_target_properties(tlo-cpp PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-cpp PUBLIC cxx_std_17)
target_compile_options(tlo-cpp
  PRIVATE ${private_compile_options}
  PUBLIC ${public_compile_options}
)
target_compile_definitions(tlo-cpp PRIVATE ${private_compile_definitions})
target_include_directories(tlo-cpp PUBLIC ${public_include_directories})
target_link_libraries(tlo-cpp PUBLIC ${public_link_libraries})

add_library(tlo-cpp-test-main STATIC
  include/tlo-cpp/test.hpp
  src/test.cpp
  src/test-main.cpp
)
set_target_properties(tlo-cpp-test-main PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-cpp-test-main PUBLIC cxx_std_17)
target_compile_options(tlo-cpp-test-main
  PRIVATE ${private_compile_options}
  PUBLIC ${public_compile_options}
)
target_compile_definitions(tlo-cpp-test-main
  PRIVATE ${private_compile_definitions}
)
target_include_directories(tlo-cpp-test-main
  PUBLIC ${public_include_directories}
)

set(tlo_cpp_test_sources
  chrono-test.cpp
  command-line-test.cpp
  damerau-levenshtein-test.cpp
  hash-test.cpp
  lcs-test.cpp
  levenshtein-test.cpp
  sqlite3-test.cpp
  stop-test.cpp
  string-test.cpp
  test-test.cpp
)
prepend(tlo_cpp_test_sources test/ ${tlo_cpp_test_sources})

add_executable(tlo-cpp-test ${tlo_cpp_test_sources})
set_target_properties(tlo-cpp-test PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(tlo-cpp-test PRIVATE ${private_compile_options})
target_compile_definitions(tlo-cpp-test PRIVATE ${private_compile_definitions})
target_link_libraries(tlo-cpp-test PRIVATE tlo-cpp tlo-cpp-test-main)

add_test(NAME tlo-cpp-test COMMAND tlo-cpp-test)
