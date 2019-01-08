cmake_minimum_required(VERSION 3.10)
project(llvm_sol)
include_directories("/home/wang/llvm-3.5.2/llvm/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
        Json/SolcJsonOperator.cpp
        Json/SolcJsonOperator.h
        SolcMain.cpp
        Json/jsoncpp.cpp
        DataStruct.h Builder/SolcLLVM.cpp Builder/SolcLLVM.h SolcMain.h)
add_executable(llvm_sol ${SOURCE_FILES})