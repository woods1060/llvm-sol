//
// Created by wang on 29/04/18
// v1.0.0
//
//

#include "SolcLLVM.h"
SolcLLVM::SolcLLVM() {
    m_StrGlobals.clear();
    m_BasicBlock.clear();
    m_Builder = new llvm::IRBuilder<>(m_Context);
}
SolcLLVM::~SolcLLVM() {}
void SolcLLVM::setNowContract(std::string strNowContract)
{
    m_NowContract = strNowContract;

}
/**
 * 创建Module
 * @param strFileName
 * @return
 */
bool SolcLLVM::CreateModule(std::string strFileName)
{
    m_Module = new llvm::Module(strFileName, m_Context);
    if(m_Module != nullptr)
    {
        std::cout<<"Create Module succes"<<std::endl;
        return true;
    }
    return false;
}
/**
 * 记录当前块的一个局部变量
 * @param strVarName
 * @param v
 * @return
 */
bool SolcLLVM::insertLocalVar(std::string strVarName, llvm::Value *v)
{
    auto m = GetBlockValuesMap(m_NowBlock->m_block->getName());
    if(m == nullptr)
    {
        m = new mapLLVMVlaue();
    }
    m->insert(pairValue(strVarName,v));
    m_LocalValue.insert(pairMapValue(m_NowBlock->m_block->getName(),m));
    return true;

}
/**
 * 获取Solc类型对应llvm的类型
 * @param strType
 * @return
 */
llvm::Type* SolcLLVM::GetTypes(std::string strType)
{
    if(strType.compare(LLVM_TYPE_VOID) == 0)
    {
        return m_Builder->getVoidTy();
    }
    else if(strType==LLVM_TYPE_UINT||strType==LLVM_TYPE_INT)
    {
        return m_Builder->getIntNTy(256);
    }
    else if(strType == LLVM_TYPE_BOOL)
    {
        return m_Builder->getInt1Ty();
    }
    else if(strType.find("int") != -1)
    {
        if(strType.find("uint") != -1)
        {
            strType = strType.replace(strType.find("uint"), 4, "");
            return m_Builder->getIntNTy(std::atoi(strType.c_str()));
        }
        strType = strType.replace(strType.find("int"), 3, "");
        return m_Builder->getIntNTy(std::atoi(strType.c_str()));
    }
    //这个地方应该返回个什么鬼？就这样吧
    return m_Builder->getVoidTy();
}

bool SolcLLVM::SetGlobalString(std::string strName, std::string strContext)
{
    llvm::Value * strLLVMTmp = m_Builder->CreateGlobalStringPtr(strContext);
    m_StrGlobals.insert(pairGlobalString(strName,strLLVMTmp));
    return true;
}
llvm::Value* SolcLLVM::GetGlobalString(std::string strName)
{
    llvm::Value*  v;
    auto iter = m_StrGlobals.find(strName);
    if(iter != m_StrGlobals.end())
    {
        std::cout<<iter->first;
        v = iter->second;
    }
    return v;
}
/**
 * 创建一个Block
 * @param strBlockName
 * @param func
 * @return
 */
bool SolcLLVM::CreateBlock(std::string &strBlockName, llvm::Function *func)
{
    CodeBlock* cBlock = new CodeBlock();
    llvm::BasicBlock *blockTmp = nullptr;
    if(func == nullptr)
    {
        cBlock->m_Func  = m_NowBlock->m_Func;
        blockTmp = llvm::BasicBlock::Create(llvm::getGlobalContext(),strBlockName, m_NowBlock->m_Func);
        cBlock->m_strBlockName = blockTmp->getName();
    }
    else
    {
        cBlock->m_Func  = func;
        blockTmp = llvm::BasicBlock::Create(llvm::getGlobalContext(),strBlockName, func);
        cBlock->m_strBlockName = blockTmp->getName();
    }
    cBlock->m_block = blockTmp;//当前快
    cBlock->m_ParentBlock = m_NowBlock;//指向父块
    if(m_NowBlock != nullptr)
    {
        //父块链接子块
        m_NowBlock->m_MapChildBlock->insert(pairCodeBlock(strBlockName,cBlock));
    }
    strBlockName=cBlock->m_block->getName();
    std::cout<<"create block : "<<strBlockName<<std::endl;
    m_BasicBlock.insert(pairCodeBlock(strBlockName,cBlock));
    return true;
}
/**
 * 设置插入点 block
 * @param strBlockName
 * @return
 */
bool SolcLLVM::SetInsertPoint(std::string strBlockName)
{
    CodeBlock* block = nullptr;
    auto iter = m_BasicBlock.find(strBlockName);
    if(iter != m_BasicBlock.end())
    {
        block = iter->second;
    }
    else{
        return false;
    }
    m_NowBlock = block;
    m_Builder->SetInsertPoint(block->m_block);
    return true;
}
/**
 * 获取指定块内的变量
 * @param strBlockName
 * @return
 */
mapLLVMVlaue* SolcLLVM::GetBlockValuesMap(std::string strBlockName)
{
    mapLLVMVlaue*  m = nullptr;
    auto iter = m_LocalValue.find(strBlockName);
    if(iter != m_LocalValue.end())
    {
        m = iter->second;
    }
    return m;
}
/**
 * 在指定块内开一个变量
 * @param strVarName
 * @param strBlockName
 * @return
 */
llvm::Value* SolcLLVM::GetLocalValues(std::string strVarName,std::string strBlockName)
{
    if(strBlockName.empty())
    {
        //如果传入空，默认当前快
        strBlockName = m_NowBlock->m_block->getName();
    }
    mapLLVMVlaue*  m = GetBlockValuesMap(strBlockName);
    if(m == nullptr)
    {
        return nullptr;
    }
    llvm::Value* v = nullptr;
    auto iter = m->find(strVarName);
    if(iter != m->end())
    {
        v = iter->second;
    }
    return v;
}
/**
 *
 * @param strType 变量类型
 * @param strStructName 所在STRUCT的命名
 * @param strVarName 变量名
 * @param ev 右值
 * @return
 */
bool SolcLLVM::CreateLocalVariable(std::string strType,std::string strStructName,std::string &strVarName,ExpressionVar* ev)
{
    mapLLVMVlaue*  m = GetBlockValuesMap(m_NowBlock->m_block->getName());
    if(m == nullptr)
    {
        m = new mapLLVMVlaue();
    }
    llvm::Value *var = m_Builder->CreateAlloca(GetTypes(strType), nullptr,strVarName);
    if(ev != nullptr)
    {
        if(ev->m_isConstant)
        {
            //从常量初始化
            auto v = getConstant(strType,ev->m_strValue);
            m_Builder->CreateStore(v,var);
        }
        else
        {
            //从作用域内寻找
            auto v = FindValue(strStructName,m_NowBlock->m_block->getName(),ev->m_strValue);
            m_Builder->CreateStore(v,var);
        }
    }
    strVarName = var->getName();
    std::cout<<"Crate Value " << strVarName<<std::endl;
    m->insert(pairValue(strVarName,var));
    m_LocalValue.insert(pairMapValue(m_NowBlock->m_block->getName(),m));
    return true;
}
/**
 * 为变量赋值
 * @param strVar
 * @param strPtr
 * @return
 */
bool SolcLLVM::SetLocalVar(std::string strVar, std::string strPtr)
{
    llvm::Value* var = FindValue(m_NowContract,m_NowBlock->m_strBlockName,strVar);
    if(var == nullptr)
    {
        return false;
    }
    llvm::Value* ptr = FindValue(m_NowContract,m_NowBlock->m_strBlockName,strPtr);
    if(ptr == nullptr)
    {
        return false;
    }
    m_Builder->CreateStore(var,ptr);
    return true;
}
llvm::Value* SolcLLVM::GetGlobalValues(std::string strName)
{
    llvm::Value*  v;
    auto iter = m_GlobalValue.find(strName);
    if(iter != m_GlobalValue.end())
    {
        v = iter->second;
    }
    return v;
}

bool SolcLLVM::CreateGlobalVariable(std::string strType,std::string strVarName,std::string strConstant,std::string strIndentVarName)
{
    llvm::Constant* v = nullptr;
    if(!strConstant.empty())
    {
        //从字面常量初始化
        v = getConstant(strType,strConstant);
    }
    if(!strIndentVarName.empty())
    {
        //从已定义的全局变量上初始化
        v = (llvm::Constant*)GetGlobalValues(strIndentVarName);
    }
    llvm::Value *var = new llvm::GlobalVariable(*m_Module,GetTypes(strType),false,llvm::GlobalVariable::CommonLinkage,v,strVarName);
    m_GlobalValue.insert(pairValue(strVarName,var));
    return true;
}
/**
 * 初始化函数，创建函数的最后一部
 * @param strFuncName
 * @return
 */
bool SolcLLVM::InitFunc(std::string strFuncName)
{
    llvm::Function* f = GetFunction(strFuncName);
    //f.
}
/**
 * 申明一个函数
 * @param strFuncName
 * @param strFuncType
 * @param argTypes
 * @param argsName
 * @return
 */
bool SolcLLVM::CreateFunc(std::string strFuncName, std::string strFuncType,Types* argTypes,ArgsName* argsName)
{
    llvm::FunctionType *funcType;
    if(argsName == nullptr)
    {
        argsName = new ArgsName();
    }
    if(argTypes == nullptr)
    {
        argTypes = new Types();
    }
    argTypes->insert(argTypes->begin(),GetContract(m_NowContract)->getType());
    argsName->insert(argsName->begin(),"this");
    funcType = llvm::FunctionType::get(GetTypes(strFuncType),*argTypes,false);
    llvm::Function *func =
            llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, strFuncName, m_Module);
    //赐予形参名
    if(argsName != NULL)
    {
        int Idx = 0;
        for (llvm::Function::arg_iterator AI = func->arg_begin(); Idx != argsName->size();
             ++AI, ++Idx) {
            AI->setName((*argsName)[Idx]);
        }
    }
    m_Functions.insert(pairFunction(strFuncName,func));
    return true;
}
/**
 * 获取函数
 * @param strName
 * @return
 */
llvm::Function* SolcLLVM::GetFunction(std::string strName)
{
    llvm::Function*  f = nullptr;
    auto iter = m_Functions.find(strName);
    if(iter != m_Functions.end())
    {
        f = iter->second;
    }
    return f;
}
/**
 * 声明一个结构提类型
 * @param strName
 * @param mapVarIndex
 * @return
 */
bool SolcLLVM::CreateStructType(std::string strName,MapVarIndex* mapVarIndex)
{
    llvm::StructType* stype = llvm::StructType::create(m_Context,strName);
    m_StructType.insert(pairStructType(strName,stype));
    m_StructMapVar.insert(pairStructMapVar(strName,mapVarIndex));
    return true;
}
/**
 * 声明一个合约类型，与结构体类似
 * @param strName
 * @param mapVarIndex
 * @return
 */
bool SolcLLVM::CreateContractType(std::string strName,MapVarIndex* mapVarIndex)
{
    llvm::StructType* stype = llvm::StructType::create(m_Context,strName);
    m_ContractType.insert(pairStructType(strName,stype));
    m_ContractMapVar.insert(pairStructMapVar(strName,mapVarIndex));
    return true;
}
/**
 * 获取成员变量名对应的index
 * @param varName
 * @return
 */
int SolcLLVM::getContractVarIndex(std::string varName)
{
    MapVarIndex *m = GetContractVerIndex(m_NowContract);
    if(m== nullptr)
    {
        return -1;
    }
    auto i = m->find(varName);
    if(i != m->end())
    {
        return i->second;
    }
    return -1;
}
/**
 * 获取Struct Value Index
 * @param strName
 * @return
 */
MapVarIndex* SolcLLVM::GetStructVerIndex(std::string strName)
{
    MapVarIndex* m = nullptr;
    auto i = m_StructMapVar.find(strName);
    if(i != m_StructMapVar.end() )
    {
        m = i->second;
    }
    return m;
}
/**
 * 合约 Value Index
 * @param strName
 * @return
 */
MapVarIndex* SolcLLVM::GetContractVerIndex(std::string strName)
{
    MapVarIndex* m = nullptr;
    auto i = m_ContractMapVar.find(strName);
    if(i != m_ContractMapVar.end() )
    {
        m = i->second;
    }
    return m;
}
/**
 * 设置Struct 成员变量类型
 * @param strName
 * @param varTypes
 * @return
 */
bool SolcLLVM::SetStructVartype(std::string strName,Types varTypes)
{
    GetStructType(strName)->setBody(varTypes);
    return true;
}
/**
 * 设置合约 成员变量类型
 * @param strName
 * @param varTypes
 * @return
 */
bool SolcLLVM::SetContractVartype(std::string strName,Types varTypes)
{
    GetContractType(strName)->setBody(varTypes);
    return true;
}
/**
 * 取得一个和结构体类型，前提是这个结构体类型已经被声明过了
 * @param strName public privte
 * @return
 */
llvm::StructType* SolcLLVM::GetStructType(std::string strName)
{
    llvm::StructType*  t = nullptr;
    auto iter = m_StructType.find(strName);
    if(iter != m_StructType.end())
    {
        t = iter->second;
    }
    return t;
}
/**
 * 取得一个合约类型，前提是这个合约类型已经被声明过了
 * @param strName public privte
 * @return
 */
llvm::StructType* SolcLLVM::GetContractType(std::string strTypeName)
{
    llvm::StructType*  t = nullptr;
    auto iter = m_ContractType.find(strTypeName);
    if(iter != m_ContractType.end())
    {
        t = iter->second;
    }
    return t;
}
/**
 * 创建一个结构体
 * @param strName 名
 * @param strTypeName 结构体TYPE名
 * @return
 */
bool SolcLLVM::CreateStruct(std::string strName,std::string strTypeName)
{
    llvm::StructType* t = GetStructType(strTypeName);
    llvm::Value* g_V = new llvm::GlobalVariable::GlobalVariable(*m_Module,
            t,
            false,
            llvm::GlobalVariable::CommonLinkage,
            nullptr,
            strName);
    m_GStructVar.insert(pairStruct(strName,g_V));
    return true;
}
/**
 * 申明一个合约，类似结构体的处理方式
 * @param strName
 * @param strTypeName
 * @return
 */
bool SolcLLVM::CreateContract(std::string strName, std::string strTypeName)
{
    llvm::StructType* t = GetContractType(strTypeName);
    llvm::Value* g_V = new llvm::GlobalVariable::GlobalVariable(*m_Module,
                                                                t,
                                                                false,
                                                                llvm::GlobalVariable::CommonLinkage,
                                                                nullptr,
                                                                strName);
    m_GContractVar.insert(pairStruct(strName,g_V));
    return true;
}
/**
 * 取得指定的结构体
 * @param strName
 * @return
 */
llvm::Value* SolcLLVM::GetStruct(std::string strName)
{
    llvm::Value*  v;
    auto iter = m_GStructVar.find(strName);
    if(iter != m_GStructVar.end())
    {
        v = iter->second;
    }
    return v;
}
/**
 * 取得指定的合约
 * @param strName
 * @return
 */
llvm::Value* SolcLLVM::GetContract(std::string strName)
{
    llvm::Value*  v;
    auto iter = m_GContractVar.find(strName);
    if(iter != m_GContractVar.end())
    {
        v = iter->second;
    }
    return v;
}
/**
 * 设置结构体成员变量
 * @param strStructName
 * @param strType
 * @param index
 * @param strConstant
 * @param iIndentVarName
 * @return
 */
bool SolcLLVM::SetStructVar(std::string strStructName,std::string strType, int index, std::string strConstant, int iIndentVarName)
{
    llvm::Value*  vStruct;
    auto iter = m_GStructVar.find(strStructName);
    if(iter != m_GStructVar.end())
    {
        vStruct = iter->second;
        if(!strConstant.empty())//从字面常量初始化
        {
            //从字面常量初始化
            llvm::Value* v = getConstant(strType,strConstant);
            llvm::Value *elementPtr = m_Builder->CreateStructGEP(vStruct,index);
            m_Builder->CreateStore(v,elementPtr);
        }
        else if(iIndentVarName >= 0)
        {
            llvm::Value *elementPtr1 = m_Builder->CreateStructGEP(vStruct,index);
            llvm::Value *elementPtr2 = m_Builder->CreateStructGEP(vStruct,iIndentVarName);
            m_Builder->CreateStore(elementPtr2,elementPtr1);
        }
        return true;
    }
    return false;
}
/**
 * 设置结构体成员变量
 * @param strContractName
 * @param strType
 * @param index
 * @param strConstant
 * @param iIndentVarName
 * @return
 */
bool SolcLLVM::SetContractVar(std::string strType,std::string strContractName,std::string strVarName,ExpressionVar ev)
{
    std::cout<<"SetContractVar:"<<strVarName<<":"<<ev.m_strValue<<std::endl;
    llvm::Value*  vContract = nullptr;
    int index = getContractVarIndex(strVarName);
    if(strContractName.empty())
    {
        strContractName = m_NowContract;//指向当前合约
    }
    auto iter = m_GContractVar.find(strContractName);
    if(iter != m_GContractVar.end())
    {
        vContract = iter->second;
        if(ev.m_isConstant)//从字面常量初始化
        {
            //从字面常量初始化
            llvm::Value* v = getConstant(ev.m_strType,ev.m_strValue);
            llvm::Value *elementPtr = m_Builder->CreateStructGEP(vContract,index);
            m_Builder->CreateStore(v,elementPtr);
        }
        else
        {
            int i = getContractVarIndex(ev.m_strValue);
            llvm::Value *elementPtr1 = m_Builder->CreateStructGEP(vContract,index);
            llvm::Value *elementPtr2 = m_Builder->CreateStructGEP(vContract,i);
            m_Builder->CreateStore(elementPtr2,elementPtr1);
        }
        return true;
    }
    return false;
}
/**
 * 生成一个字面常量
 * @param strType 常量类型
 * @param strConstant 常量（字符串形式）
 * @return 字面常量
 */
llvm::Constant* SolcLLVM::getConstant(std::string strType,std::string strConstant)
{
    std::cout<<"get Constant type:"<<strType<<" Value "<<strConstant<<std::endl;
    if(strType.find("int") != -1)
    {
        int Constant = std::atoi(strConstant.c_str());
        llvm::Constant* v = llvm::Constant::getIntegerValue(
                (llvm::IntegerType*)GetTypes(LLVM_TYPE_UINT),
                llvm::APInt::APInt(256,Constant));
        return v;
    }
    if(strType == LLVM_TYPE_BOOL)
    {
        llvm::Constant* v;
        (strConstant == "true")
        ?v = m_Builder->getTrue()
        :v = m_Builder->getFalse();
        return v;
    }
    return nullptr;
}
/**
 * 创建一个赋值语句
 * @param strBlockName 当前块名
 * @param strStructName 当前结构体
 * @param strLeft 左值
 * @param right 右值
 * @return
 */
bool SolcLLVM::CreateAssignment( std::string strLeft,const ExpressionVar right,std::string strStructName,std::string strBlockName)
{
    //左值
    llvm::Value* leftValue = FindValue(m_NowContract,m_NowBlock->m_block->getName(),strLeft);
    if(leftValue == nullptr)
    {
        return false;
    }
    //右值
    llvm::Value* rightValue;
    if(right.m_isConstant)
    {
        llvm::Value* rightValue = getConstant(right.m_strType,right.m_strValue);
        m_Builder->CreateStore(rightValue,leftValue);
        return true;
    }
    rightValue = FindValue(strStructName,m_NowBlock->m_block->getName(),right.m_strValue);
    if(rightValue == nullptr)
    {
        return false;
    }
    m_Builder->CreateStore(rightValue,leftValue);
    return true;
}
/**
 *  制定作用域寻找Value
 * @param strStructName 结构体名
 * @param strBlockName Block名
 * @param strVarName 变量名
 * @return 找到的变量
 */
llvm::Value* SolcLLVM::FindValue(std::string strContractName, std::string strBlockName, std::string strVarName)
{
    std::cout<<"find value::"<<strVarName<<std::endl;
    llvm::Value* value = nullptr;
    if(strContractName.empty())
    {
        strContractName = m_NowContract;
    }
    //优先选择局部变量
    CodeBlock* block = m_NowBlock;
    std::string strFuncName;
    while(block != nullptr)
    {
        std::cout<<"find in block: "<<std::string(block->m_block->getName())<<std::endl;
        value = GetLocalValues(strVarName,block->m_block->getName());
        if(value != nullptr)
        {
            return value;
        }
        block = block->m_ParentBlock;//指向父块
    }
    llvm::Function* f = m_NowBlock->m_Func;
    if(f == nullptr)
    {
       return nullptr;
    }
    if(value == nullptr)
    {
        //再从形参里选
        for (llvm::Function::arg_iterator AI = f->arg_begin(); AI != f->arg_end(); ++AI) {
            if(AI->getName().data() == strVarName)
            {
                std::cout<<"find vale:"<<strVarName<<" in func: "<<std::string(f->getName())<<" args"<<std::endl;
                //形参
                value = AI;
                return value;
            }
        }
    }

    if(value == nullptr)
    {
        //还有可能来自接收者
        llvm::Value* receiver = f->arg_begin();
        MapVarIndex* m = GetContractVerIndex(strContractName);
        if(m == nullptr)
        {
            return nullptr;
        }
        if(m->find(strVarName) != m->end())
        {
            std::cout<<strContractName<<"CreateStructGEP:"<<std::string(receiver->getName())<<std::endl;
            //接受者提供
            value = m_Builder->CreateStructGEP(receiver,0);

            return value;
        }
    }
    return value;
}
/**
 * 创建一个二元运算
 * @param leftVar 左
 * @param rightVar 右
 * @param Operation 运算符
 * @param strNewVarName
 * @return
 */
bool SolcLLVM::CreateBinaryOperation(ExpressionVar leftVar, ExpressionVar rightVar, std::string strOperation,
                                     std::string &strNewVarName)
{
    llvm::Value* leftV = nullptr;
    llvm::Value* rightV = nullptr;
    if(leftVar.m_isConstant)//左值是字面常量？
    {
        leftV = getConstant(leftVar.m_strType,leftVar.m_strValue);
    }
    else
    {
        leftV = FindValue(m_NowContract,m_NowBlock->m_block->getName(),leftVar.m_strValue);
    }
    if(rightVar.m_isConstant)//右值？
    {
        rightV = getConstant(rightVar.m_strType,rightVar.m_strValue);
    }
    else
    {
        rightV = FindValue(m_NowContract,m_NowBlock->m_block->getName(),rightVar.m_strValue);
    }
    if(rightV == nullptr)
    {
        std::cout<<"find Value : error"<<rightVar.m_strValue<< std::endl;
        return false;
    }
    if(leftV == nullptr)
    {
        std::cout<<"find Value : error  "<<leftVar.m_strValue<<std::endl;
        return false;
    }
    llvm::Value* newValue;

    switch(*strOperation.begin()) {
        case '+':
            if (leftV->getType()->isIntegerTy() == 1) {
                newValue = m_Builder->CreateAdd(leftV, rightV, "add1", false, false);
            } else {
                if (leftV->getType()->isFloatTy() == 1) {
                    newValue = m_Builder->CreateFAdd(leftV, rightV, "add2");
                } else {
                    return nullptr;
                }
            }
            break;
        case '-':

            if (leftV->getType()->isIntegerTy() == 1) {
                newValue = m_Builder->CreateSub(leftV, rightV, "sub1", false, false);
            } else {
                if (leftV->getType()->isFloatTy() == 1) {
                    newValue = m_Builder->CreateFSub(leftV, rightV, "sub2");
                } else {
                    return nullptr;
                }
            }
            break;
        case '*':
            if(strOperation == "***")
            {
                /*
                long p = 1;
                while (n){
                    if (n & 1) p *= x;
                     x *= x;
                     n >>= 1;
                     }
                return p;*/
                return nullptr;
            }
            if(strOperation == "*")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateMul(leftV, rightV, "mul1", false, false);
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFMul(leftV, rightV, "mul2");
                    } else {
                        return nullptr;
                    }
                }
            }
            break;
        case '/':
            if (leftV->getType()->isIntegerTy() == 1) {
                newValue = m_Builder->CreateSDiv(leftV, rightV, "sdiv1", false);
            } else {
                if (leftV->getType()->isFloatTy() == 1) {
                    newValue = m_Builder->CreateFDiv(leftV, rightV, "fdiv2");
                } else {
                    return nullptr;
                }
            }
            break;
        case '%':
            if (leftV->getType()->isIntegerTy() == 1) {
                newValue = m_Builder->CreateSRem(leftV, rightV, "srem1");
            } else {
                if (leftV->getType()->isFloatTy() == 1) {
                    newValue = m_Builder->CreateFRem(leftV, rightV, "frem2");
                } else {
                    return nullptr;
                }
            }
            break;
        case '=':
            if (strOperation == "==")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateICmpEQ(leftV, rightV, "ICmpEQ1");
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFCmpOEQ(leftV, rightV, "FCmpEQ1");
                    } else {
                        return nullptr;
                    }
                }
            }
            break;
        case '!':
            if (strOperation == "!=")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateICmpNE(leftV, rightV, "ICmpNE1");
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFCmpONE(leftV, rightV, "FCmpNE1");
                    } else {
                        return nullptr;
                    }
                }
            }
            break;
        case '>':
            if (strOperation == ">")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateICmpSGT(leftV, rightV, "ICmpSGT1");
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFCmpUGT(leftV, rightV, "FCmpUGT1");
                    } else {
                        return nullptr;
                    }
                }
            }
            if (strOperation == ">=")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateICmpSGE(leftV, rightV, "ICmpSGE1");
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFCmpUGE(leftV, rightV, "FCmpUGE1");
                    } else {
                        return nullptr;
                    }
                }
            }
            if (strOperation == ">>")
            {
                newValue = m_Builder->CreateAShr(leftV,rightV,"Ashr1",false);
            }
            break;
        case '<':
            if(strOperation == "<")
            {
                if (leftV->getType()->isIntegerTy() == 1) {
                    newValue = m_Builder->CreateICmpSLT(leftV, rightV, "ICmpSLT1");
                } else {
                    if (leftV->getType()->isFloatTy() == 1) {
                        newValue = m_Builder->CreateFCmpULT(leftV, rightV, "FCmpULT1");
                    } else {
                        return nullptr;
                    }
                }
            }
            if(strOperation == "<=")
            {
                if(leftV->getType()->isIntegerTy()==1)
                {
                    newValue = m_Builder->CreateICmpSLE(leftV,rightV,"ICmpSLE1");
                }
                else
                {
                    if(leftV->getType()->isFloatTy()==1)
                    {
                        newValue = m_Builder->CreateFCmpULE(leftV,rightV,"FCmpULE1");
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
            if(strOperation == "<<")
            {
                newValue = m_Builder->CreateShl(leftV,rightV,"Shl1",false,false);
            }
            break;
        case '|':
            newValue = m_Builder->CreateOr(leftV,rightV,"Or1");
            break;
        case '&':
            newValue = m_Builder->CreateAnd(leftV,rightV,"And1");
            break;
        case '^':
            newValue = m_Builder->CreateXor(leftV,rightV,"Xor1");
            break;
        case '~':
            newValue = m_Builder->CreateNot(leftV,"Not1");
            break;
        }
    strNewVarName = newValue->getName();
    std::cout<<"create a Value:"<<strNewVarName<<std::endl;
    insertLocalVar(strNewVarName,newValue);
    return true;
}
/**
 * 指定块名获取一个代码块
 * @param strBlockName
 * @return
 */
llvm::BasicBlock* SolcLLVM::GetBlock(std::string strBlockName)
{
    llvm::BasicBlock* b = nullptr;
    auto i = m_BasicBlock.find(strBlockName);
    if(i != m_BasicBlock.end())
    {
        b = i->second->m_block;
    }
    return b;
}
/**
 * 退出当前块
 */
void SolcLLVM::ExitNowBlock()
{
    //退出当前块，设置为父块
    m_NowBlock = m_NowBlock->m_ParentBlock;
}
/**
 * 创建if - else 语句
 * 执行完该函数，实际上当前块已经指向了Break块
 * 我在写什么，我在哪儿
 * @param strBlockTrue
 * @param strBlockFalse
 * @param strBlockBreak
 * @param strCondName
 * @return
 */
bool SolcLLVM::CreateIfExpression(std::string strCondName, std::string strBlockBreak,std::string strBlockTrue, std::string strBlockFalse)
{
    //std::cout<<"if:"<<strCondName<<":"<<strBlockTrue<<":else:"<<strBlockFalse<<":break:"<<strBlockBreak<<std::endl;
    //记录一下当前块名
    std::string strBlockName = m_NowBlock->m_block->getName();
    if(strBlockTrue.empty() && strBlockFalse.empty())
    {
        m_Builder->CreateBr(GetBlock(strBlockBreak));
    }
    else if(!strBlockTrue.empty() && strBlockFalse.empty())
    {
        m_Builder->CreateCondBr(FindValue(m_NowContract,strBlockName,strCondName),GetBlock(strBlockTrue),GetBlock(strBlockBreak));
        SetInsertPoint(strBlockTrue);
        CreateIfExpression("",strBlockBreak);
        SetInsertPoint(strBlockBreak);
    }
    else
    {
        m_Builder->CreateCondBr(FindValue(m_NowContract,strBlockName,strCondName),GetBlock(strBlockTrue),GetBlock(strBlockFalse));
        SetInsertPoint(strBlockTrue);
        CreateIfExpression("",strBlockBreak);
        SetInsertPoint(strBlockFalse);
        CreateIfExpression("",strBlockBreak);
        SetInsertPoint(strBlockBreak);
    }
    return true;
}
/**
 * 创建While语句
 * @param strCondValueName
 * @param strBlockCond 条件
 * @param strBlockBreak 跳出到指定块
 * @param strBlockTrue  真.块
 * @return
 */
bool SolcLLVM::CreateWhileExpression(std::string strCondValueName,std::string strBlockCond, std::string strBlockBreak, std::string strBlockTrue)
{
    ExitNowBlock();
    m_Builder->CreateBr(GetBlock(strBlockCond));
    SetInsertPoint(strBlockCond);
    m_Builder->CreateCondBr(FindValue(m_NowContract,strBlockCond,strCondValueName),GetBlock(strBlockTrue),GetBlock(strBlockBreak));
    SetInsertPoint(strBlockTrue);
    m_Builder->CreateBr(GetBlock(strBlockCond));
    SetInsertPoint(strBlockBreak);
    return true;
}