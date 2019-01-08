//
// Created by wang on 01/05/18.
//

#ifndef LLVM_SOL_DATASTRUCT_H
#define LLVM_SOL_DATASTRUCT_H

#include <iostream>
struct TypeRef{
    //Solidity 参数类型
    std::string m_SolidityName;
    //Llvm 参数类型
    std::string m_LlvmName;
};
#endif //LLVM_SOL_DATASTRUCT_H
