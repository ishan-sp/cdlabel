cmake_minimum_required(VERSION 3.10)
project(NumericalPass)

find_package(LLVM REQUIRED CONFIG
    PATHS "/home/ishan/cdel/llvm-project/build/lib/cmake/llvm"
    NO_DEFAULT_PATH
)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")

add_definitions(${LLVM_DEFINITIONS})

add_library(NumericalPass MODULE NumericalPass.cpp)

target_compile_options(NumericalPass PRIVATE -fno-rtti)

target_include_directories(NumericalPass PRIVATE
    /home/ishan/cdel/llvm-project/llvm/include   # source headers
    /home/ishan/cdel/llvm-project/build/include  # generated headers
)

set_target_properties(NumericalPass PROPERTIES PREFIX "")
