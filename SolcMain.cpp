//
//
// Created by wang on 28/04/18.
//

#include <iostream>
#include "SolcMain.h"
#define  JSON_FILE_PATH    "/home/wang/llvm-3.5.2/llvm/llvm-sol/check.json"
int main()
{
    SolcJsonOperator json_operator = SolcJsonOperator(JSON_FILE_PATH);
    json_operator.ParseJson(g_ASTreeRootNode);
    std::string name = json_operator.GetValue({"attributes","absolutePath"}).asString();
    //TransformRoot(g_ASTreeRootNode);
    g_ASTreeRootNode.Transfrom();
    g_LLVM.m_Module->dump();
    return 0;
}