cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project("tlo-cpp")
enable_testing()

# Threads required. See CMake documentation on FindThreads module.
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SQLite3 REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options -pedantic -Wall -Wextra -Werror)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options ${global_compile_options} -Weverything
    -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructors
    -Wno-global-constructors)
endif()

set(global_link_libraries Threads::Threads SQLite::SQLite3)

option(TLO_CPP_COLORED_DIAGNOSTICS
  "Tell the compiler to use colors in diagnostics (GNU/Clang only)." ON)
if (TLO_CPP_COLORED_DIAGNOSTICS)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(global_compile_options ${global_compile_options} -fdiagnostics-color)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(global_compile_options ${global_compile_options} -fcolor-diagnostics)
  endif()
endif()

option(TLO_CPP_USE_LIBCPP "Use libc++ (Clang only)." OFF)
if (TLO_CPP_USE_LIBCPP)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(global_compile_options ${global_compile_options} -stdlib=libc++)
    set(global_link_libraries ${global_link_libraries} c++)
  endif()
endif()

option(TLO_CPP_LINK_FS
  "Link to filesystem library of older GNU and Clang (GNU/Clang only)." OFF)
if (TLO_CPP_LINK_FS)
  if (TLO_CPP_USE_LIBCPP)
    # Prior to LLVM 9, using std::filesystem required linker option -lc++fs.
    set(global_link_libraries ${global_link_libraries} c++fs)
  else()
    # Prior to GCC 9, using std::filesystem required linker option -lstdc++fs.
    set(global_link_libraries ${global_link_libraries} stdc++fs)
  endif()
endif()

macro(prepend listVar prefix)
  set(${listVar} "")
  foreach(item ${ARGN})
    list(APPEND ${listVar} "${prefix}${item}")
  endforeach(item)
endmacro(prepend)

set(tlo_cpp_headers chrono.hpp damerau-levenshtein.hpp filesystem.hpp hash.hpp
  lcs.hpp levenshtein.hpp options.hpp sqlite3.hpp stop.hpp string.hpp)
prepend(tlo_cpp_headers ${PROJECT_SOURCE_DIR}/include/tlo-cpp/
  ${tlo_cpp_headers})

set(tlo_cpp_private_headers container.hpp)
prepend(tlo_cpp_private_headers ${PROJECT_SOURCE_DIR}/src/
  ${tlo_cpp_private_headers})

set(tlo_cpp_sources chrono.cpp container.cpp damerau-levenshtein.cpp
  filesystem.cpp hash.cpp lcs.cpp levenshtein.cpp options.cpp sqlite3.cpp
  stop.cpp string.cpp)
prepend(tlo_cpp_sources ${PROJECT_SOURCE_DIR}/src/ ${tlo_cpp_sources})

add_library(tlo-cpp STATIC ${tlo_cpp_headers} ${tlo_cpp_private_headers}
  ${tlo_cpp_sources})
set_target_properties(tlo-cpp PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-cpp PUBLIC cxx_std_17)
target_compile_options(tlo-cpp PRIVATE ${global_compile_options})
target_include_directories(tlo-cpp PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tlo-cpp PUBLIC ${global_link_libraries})

add_library(tlo-cpp-test-main STATIC
  ${PROJECT_SOURCE_DIR}/include/tlo-cpp/test.hpp
  ${PROJECT_SOURCE_DIR}/src/test.cpp ${PROJECT_SOURCE_DIR}/src/test-main.cpp)
set_target_properties(tlo-cpp-test-main PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-cpp-test-main PUBLIC cxx_std_17)
target_compile_options(tlo-cpp-test-main PRIVATE ${global_compile_options})
target_include_directories(tlo-cpp-test-main PUBLIC
  ${PROJECT_SOURCE_DIR}/include)

set(tlo_cpp_test_sources damerau-levenshtein-test.cpp lcs-test.cpp
  levenshtein-test.cpp test-test.cpp)
prepend(tlo_cpp_test_sources ${PROJECT_SOURCE_DIR}/test/
  ${tlo_cpp_test_sources})

add_executable(tlo-cpp-test ${tlo_cpp_test_sources})
set_target_properties(tlo-cpp-test PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(tlo-cpp-test PRIVATE ${global_compile_options})
target_link_libraries(tlo-cpp-test PRIVATE tlo-cpp tlo-cpp-test-main)

add_test(NAME tlo-cpp-test COMMAND tlo-cpp-test)
