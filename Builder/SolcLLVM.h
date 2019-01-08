//
// Created by wang on 29/04/18.
//

#ifndef LLVM_SOL_SOLCLLVM_H
#define LLVM_SOL_SOLCLLVM_H
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include <iostream>
#include <map>
#include <list>
#include <string>

#define LLVM_TYPE_VOID  "void"
#define LLVM_TYPE_UINT  "uint256"
#define LLVM_TYPE_BOOL  "bool"
#define LLVM_TYPE_INT   "int256"
//表达式变量
struct ExpressionVar
{
    bool m_isConstant;//是否是常量
    std::string m_strType;//类型
    std::string m_strValue;//值
    ExpressionVar()
    {
    }
    ExpressionVar(std::string strType,std::string strValue,bool isConstant)
    {
        this->m_strValue = strValue;
        this->m_isConstant = isConstant;
        this->m_strType = strType;
    }
};

typedef std::vector<llvm::Type*>                    Types;
typedef std::vector<std::string>                    ArgsName;
typedef std::vector<ExpressionVar>                  VecExpressionVar;

typedef std::map<std::string,llvm::Value*>          mapLLVMVlaue;
typedef std::map<std::string,int>                   MapVarIndex;
typedef std::pair<std::string,int>                  pairVarIndex;
typedef std::pair<std::string,llvm::Value*>         pairValue;
typedef std::pair<std::string,mapLLVMVlaue*>        pairMapValue;
typedef std::pair<std::string, llvm::Value*>        pairGlobalString;
typedef std::pair<std::string,llvm::Function*>      pairFunction;
typedef std::pair<std::string,llvm::BasicBlock*>    pairBasicBlock;
typedef std::pair<std::string,llvm::StructType*>    pairStructType;
typedef std::pair<std::string,llvm::Value*>         pairStruct;
typedef std::pair<std::string,MapVarIndex*>         pairStructMapVar;
//typedef
struct CodeBlock
{
    //函数
    llvm::Function*                     m_Func;
    //父块
    CodeBlock*                          m_ParentBlock;
    //当前块
    llvm::BasicBlock*                   m_block;
    //子块
    std::map<std::string,CodeBlock*>*   m_MapChildBlock;
    //名字
    std::string                         m_strBlockName;
    CodeBlock()
    {
        m_ParentBlock= nullptr;
        m_MapChildBlock = new std::map<std::string,CodeBlock*>();
        m_MapChildBlock->clear();
        m_block = nullptr;
    }
    ~CodeBlock()
    {
        delete(this->m_MapChildBlock);
    }
};
typedef std::pair<std::string,CodeBlock*>           pairCodeBlock;
typedef std::map<std::string,CodeBlock*>            mapBasicBlock;
class SolcLLVM {
public:
    llvm::IRBuilder<>*  m_Builder;
    CodeBlock*          m_NowBlock;
    llvm::Module*       m_Module;
    /*当前Contract*/
    std::string         m_NowContract;
    llvm::LLVMContext&  m_Context = llvm::getGlobalContext();
    llvm::Type*         GetTypes(std::string strType);
    llvm::Value*        GetGlobalString(std::string strName);
    llvm::Value*        GetLocalValues(std::string strVarName,std::string strBlockName);
    mapLLVMVlaue*       GetBlockValuesMap(std::string strBlockName);
    llvm::Value*        GetGlobalValues(std::string strName);
    llvm::Function*     GetFunction(std::string strName);
    llvm::StructType*   GetStructType(std::string strTypeName);
    llvm::Value*        GetStruct(std::string strName);

    llvm::StructType*   GetContractType(std::string strTypeName);
    llvm::Value*        GetContract(std::string strName);

    llvm::BasicBlock*   GetBlock(std::string strBlockName);
    MapVarIndex*        GetStructVerIndex(std::string strName);

    MapVarIndex*        GetContractVerIndex(std::string strName);
    /*从作用域寻找对应的Value*/
    llvm::Value*        FindValue(std::string strContractName,std::string strBlockName,std::string strVarName);
    bool CreateFunc(std::string strFuncName,std::string strFuncType,Types *argTypes = NULL,ArgsName *argsName = NULL);
    bool InitFunc(std::string strFuncName);
    bool CreateBlock(std::string &strBlockName,llvm::Function* func = nullptr);
    bool SetInsertPoint(std::string strBlockName);
    bool SetGlobalString(std::string strName,std::string strContext);
    bool CreateModule(std::string strFileName);
    bool CreateStructType(std::string strName,MapVarIndex* mapVarIndex);
    bool CreateContractType(std::string strName,MapVarIndex* mapVarIndex);
    //创建局部变量
    bool CreateLocalVariable(std::string strType,std::string strContractName,std::string &strName,ExpressionVar* ev = nullptr);
    //为变赋值
    bool SetLocalVar(std::string strVar,std::string strPtr);
    bool CreateGlobalVariable(std::string strType,std::string strName,std::string strConstant = "",std::string strIndentVarName = "");
    bool SetStructVartype(std::string strName,Types varTypes);
    bool SetContractVartype(std::string strName,Types varTypes);
    bool CreateStruct(std::string strName,std::string strTypeName);

    bool CreateContract(std::string strName,std::string strTypeName);
    bool SetStructVar(std::string strStructName,std::string strType,int index,std::string strConstant = "",int iIndentVarName = -1);

    bool SetContractVar(std::string strType,std::string strContractName,std::string strVarName,ExpressionVar ev);

    bool CreateAssignment(std::string strLeft,const ExpressionVar right,std::string strStructName = "",std::string strBlockName = "");
    bool CreateBinaryOperation(ExpressionVar leftVar,ExpressionVar rightVar,std::string Operation,std::string &strNewVarName);
    llvm::Constant* getConstant(std::string strType,std::string strConstant);
    void setNowContract(std::string strNowContract);
    bool CreateIfExpression(std::string strCondName,std::string strBlockBreak,std::string strBlockTrue = "",std::string strBlockFalse = "");
    bool CreateWhileExpression(std::string strCondValueName,std::string strBlockCond,std::string strBlockBreak,std::string strBlockTrue = "");
    int  getContractVarIndex(std::string varName);
    void ExitNowBlock();
    SolcLLVM();
    ~SolcLLVM();
private:
    /*GlobalString*/
    std::map<std::string,llvm::Value*>      m_StrGlobals;
    /*BasicBlock*/
    std::map<std::string,CodeBlock*>        m_BasicBlock;
    /*Local Values*/
    std::map<std::string,mapLLVMVlaue*>     m_LocalValue;
    /*Global Values*/
    std::map<std::string,llvm::Value*>      m_GlobalValue;
    /*Functions*/
    std::map<std::string,llvm::Function*>   m_Functions;
    /*StructType*/
    std::map<std::string,llvm::StructType*> m_StructType;
    /*Struct*/
    std::map<std::string,llvm::Value*>      m_GStructVar;
    /*Struct 成员变量*/
    std::map<std::string,MapVarIndex*>      m_StructMapVar;
    /*StructType*/
    std::map<std::string,llvm::StructType*> m_ContractType;
    /*Struct*/
    std::map<std::string,llvm::Value*>      m_GContractVar;
    /*Struct 成员变量*/
    std::map<std::string,MapVarIndex*>      m_ContractMapVar;
    bool insertLocalVar(std::string strVarName,llvm::Value* v);
};


#endif //LLVM_SOL_SOLCLLVM_H
