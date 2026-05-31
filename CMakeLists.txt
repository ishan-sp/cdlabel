cmake_minimum_required(VERSION 3.10)
project(NumericalPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG
    PATHS "/home/bhavin/llvm-project/build/lib/cmake/llvm"
    NO_DEFAULT_PATH
)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")

add_definitions(${LLVM_DEFINITIONS})

add_library(NumericalPass MODULE src/NumericalPass.cpp)

target_compile_options(NumericalPass PRIVATE -fno-rtti)

target_include_directories(NumericalPass PRIVATE
    /home/bhavin/llvm-project/llvm/include   # source headers
    /home/bhavin/llvm-project/build/include  # generated headers
)

set_target_properties(NumericalPass PROPERTIES PREFIX "")
