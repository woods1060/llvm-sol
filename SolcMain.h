//
// Created by wang on 08/05/18.
//

#ifndef LLVM_SOL_SOLCMAIN_H
#define LLVM_SOL_SOLCMAIN_H
#include "Json/SolcJsonOperator.h"
#include "Builder/SolcLLVM.h"
#define  JSON_FILE_PATH    "/home/wang/llvm-3.5.2/llvm/llvm-sol/check.json"
ASTreeRoot  g_ASTreeRootNode;
SolcLLVM    g_LLVM;
void Log(std::string str)
{
    std::cout<<"log:"<<str<<std::endl;
}
/**
 *  While
 * @return
 */
bool ASTreeWhileNode::Transfrom()
{
    Log("while"+m_name);
    auto i = m_Children.begin();
    std::string strBlockName=g_LLVM.m_NowBlock->m_block->getName();
    std::string whileValue = "WhileValue";
    //条件
    std::string strBlockCond,strBlockTrue,strBlockBreak;
    //创建一个条件块，管他有没有用
    strBlockCond = strBlockName;
    g_LLVM.CreateBlock(strBlockCond);
    g_LLVM.SetInsertPoint(strBlockCond);
    if((*i)->Case(SOLC_JSONNAME_LITERAL) || (*i)->Case(SOLC_JSONNAME_IDENTI))
    {
        ExpressionVar ev((*i)->m_Attributes[SOLC_JSON_TYPE].asString()
                    ,(*i)->m_Attributes[SOLC_JSON_VALUE].asString()
                    ,(*i)->Case(SOLC_JSONNAME_IDENTI)?false:true);
        g_LLVM.CreateLocalVariable(LLVM_TYPE_BOOL,g_LLVM.m_NowContract,whileValue,&ev);
    }
    else if((*i)->Case(SOLC_JSONNAME_BINARY))
    {
        (*i)->Transfrom();
        whileValue = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
    }
    g_LLVM.ExitNowBlock();
    ++i;
    //Block1
    if(i != m_Children.end())
    {
        strBlockTrue = strBlockName;
        g_LLVM.CreateBlock(strBlockTrue);
        g_LLVM.SetInsertPoint(strBlockTrue);
        (*i)->Transfrom();
        g_LLVM.ExitNowBlock();
    }
    //Block2
    strBlockBreak = strBlockName;
    g_LLVM.CreateBlock(strBlockBreak);
    g_LLVM.SetInsertPoint(strBlockBreak);
    //后面的内容都在BLOCK3里
    g_LLVM.SetInsertPoint(strBlockName);
    g_LLVM.CreateWhileExpression(whileValue,strBlockCond,strBlockBreak,strBlockTrue);
    return true;
}
/**
 * 解析表达式节点
 * @param asTreeNode
 * @param strVarName 返回这个表达式的值 名
 */

bool ASTreeBinaryNode::Transfrom()
{
    Log(m_name);
    std::string strType = m_Attributes[SOLC_JSON_TYPE].asString();
    std::string strOpertor = m_Attributes["operator"].asString();
    ExpressionVar leftEV,rightEV;
    auto i = m_Children.begin();
    if((*i)->Case(SOLC_JSONNAME_BINARY))
    {
        std::string strLeftVatName;
        (*i)->Transfrom();
        strLeftVatName = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
        leftEV.m_strType = strType;
        leftEV.m_strValue = strLeftVatName;
        leftEV.m_isConstant = false;
    }
    else if((*i)->Case(SOLC_JSONNAME_IDENTI) || (*i)->Case(SOLC_JSONNAME_LITERAL))
    {
        leftEV.m_strType = strType;
        leftEV.m_strValue = (*i)->m_Attributes[SOLC_JSON_VALUE].asString();
        leftEV.m_isConstant = (*i)->Case(SOLC_JSONNAME_IDENTI)?false:true;
    }
    ++i;
    if((*i)->Case(SOLC_JSONNAME_BINARY))
    {
        std::string strRightVatName;
        (*i)->Transfrom();
        strRightVatName = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
        rightEV.m_strType = strType;
        rightEV.m_strValue = strRightVatName;
        rightEV.m_isConstant = false;
    }
    else if((*i)->Case(SOLC_JSONNAME_IDENTI) || (*i)->Case(SOLC_JSONNAME_LITERAL))
    {
        rightEV.m_strType = strType;
        rightEV.m_strValue = (*i)->m_Attributes[SOLC_JSON_VALUE].asString();
        rightEV.m_isConstant = (*i)->Case(SOLC_JSONNAME_IDENTI)?false:true;
    }
    Log("BinaryOperation:L:"+leftEV.m_strValue+strOpertor+"R:"+rightEV.m_strValue);
    g_LLVM.CreateBinaryOperation(leftEV,rightEV,strOpertor,m_strTmpVerName);
    return true;
}
/**
 * 三元运算符
 * 需要返回一个Value,三元表达式则不需要
 * @param asTreeNode
 */
bool ASTreeTupleNode::Transfrom()
{
    Log("Tuple "+m_name);
    auto child = *m_Children.begin();
    std::string strType = m_Attributes[SOLC_JSON_TYPE].asString();
    std::string CondValue = "condValue";
    //直接在这里进行子节点的转换，否则与三元表达式冲突,奇葩的三元运算符
    //创建一个Value 去存放表达式返回的值？Try 1 try
    m_strTmpVerName = "TupVar";
    g_LLVM.CreateLocalVariable(strType,g_LLVM.m_NowContract,m_strTmpVerName);
    int iTmp = 0;
    std::string strBlockName=g_LLVM.m_NowBlock->m_block->getName();
    std::string strBlockTrue,strBlockFalse,strBlockBreak;
    std::string strTmpVar;
    for(auto i = child->m_Children.begin();i != child->m_Children.end();++i)
    {
        if(iTmp != 0)
        {
            std::string *strBlock = nullptr;
            iTmp == 1?strBlock = &strBlockTrue:strBlock = &strBlockFalse;
            g_LLVM.CreateBlock(*strBlock);
            g_LLVM.SetInsertPoint(*strBlock);
        }
        if((*i)->Case(SOLC_JSONNAME_LITERAL) || (*i)->Case(SOLC_JSONNAME_IDENTI))//字面常量
        {
            ExpressionVar ev((*i)->m_Attributes[SOLC_JSON_TYPE].asString()
                    ,(*i)->m_Attributes[SOLC_JSON_VALUE].asString()
                    ,(*i)->Case(SOLC_JSONNAME_IDENTI)?false:true);
            if(iTmp == 0)
                g_LLVM.CreateLocalVariable(LLVM_TYPE_BOOL,g_LLVM.m_NowContract,CondValue,&ev);
            else
            {
                g_LLVM.CreateLocalVariable(LLVM_TYPE_BOOL,g_LLVM.m_NowContract,strTmpVar,&ev);
                g_LLVM.SetLocalVar(strTmpVar,m_strTmpVerName);
            }
        }
        if((*i)->Case(SOLC_JSONNAME_BINARY))//二元表达式
        {
            (*i)->Transfrom();
            if(iTmp == 0)
                CondValue = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
            else
            {
                strTmpVar = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
                g_LLVM.SetLocalVar(strTmpVar,strTmpVar);
            }
        }
        if((*i)->Case(SOLC_JSONNAME_TUPLE))//三元表达式
        {
            (*i)->Transfrom();
            if(iTmp == 0)
                CondValue = ((ASTreeTupleNode*)(*i))->m_strTmpVerName;
            else
            {
                strTmpVar = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
                g_LLVM.SetLocalVar(strTmpVar,strTmpVar);
            }
        }
        if(iTmp!=0)
        {
            g_LLVM.ExitNowBlock();
        }
        ++iTmp;
    }
    //Block3
    strBlockBreak = strBlockName;
    g_LLVM.CreateBlock(strBlockBreak);
    g_LLVM.SetInsertPoint(strBlockBreak);
    //后面的内容都在BLOCK3里
    g_LLVM.SetInsertPoint(strBlockName);
    g_LLVM.CreateIfExpression(CondValue,strBlockBreak,strBlockTrue,strBlockFalse);
    return true;
}
/**
 * 赋值运算
 * @param asTreeNode
 * @param strStructName
 */
bool ASTreeAssignmentNode::Transfrom()
{
    Log(m_name);
    std::string strOperator = m_Attributes[SOLC_JSON_OPERATOR].asString();
    std::string strType = m_Attributes[SOLC_JSON_TYPE].asString();
    //取左值信息
    auto itor = m_Children.begin();
    std::string strLeft = (*itor)->m_Attributes[SOLC_JSON_VALUE].asString();
    ++itor;
    ExpressionVar rightEv;
    //右值
    if((*itor)->Case(SOLC_JSONNAME_IDENTI) || (*itor)->Case(SOLC_JSONNAME_LITERAL) )
    {
        rightEv.m_strType = strType;
        rightEv.m_strValue = (*itor)->m_Attributes[SOLC_JSON_VALUE].asString();
        rightEv.m_isConstant = (*itor)->Case(SOLC_JSONNAME_IDENTI)?false:true;
    }
    if((*itor)->Case(SOLC_JSONNAME_BINARY))//二元运算
    {
        Log((*itor)->m_name);
        std::string strRightName;
        //咱们先去搞二元运算
        (*itor)->Transfrom();
        strRightName = ((ASTreeBinaryNode*)(*itor))->m_strTmpVerName;
        rightEv.m_strType = strType;
        rightEv.m_strValue = strRightName;
        rightEv.m_isConstant = false;
    }
    if(strOperator.compare("=") == 0)//=
    {
        g_LLVM.CreateAssignment(strLeft,rightEv);
    }
    else//+= -= *= %= 各种
    {
        //类似二元运算处理
        ExpressionVar leftEV;
        leftEV.m_strType = strType;
        leftEV.m_strValue = strLeft;
        leftEV.m_isConstant = false;
        std::string strRightName;

        //先计算
        g_LLVM.CreateBinaryOperation(leftEV,rightEv,strOperator,strRightName);
        rightEv.m_strValue = strRightName;
        //再赋值
        g_LLVM.CreateAssignment(strLeft,rightEv);
    }
    return true;
}
/*表达式转换*/
bool ASTreeStatementNode::Transfrom()
{
    Log(m_name);
    for (auto i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        if((*i)->Case(SOLC_JSONNAME_ASSIGNMENT))//赋值运算
        {
            (*i)->Transfrom();
        }
    }
    return true;
}
/*成员变量转换*/
bool ASTreeVarDeclarationNode::Transfrom()
{
    Log(m_name);
    std::string strVarName;
    std::string strType;
    std::string strConstant;
    std::string strIdentvarName;
    strVarName = m_Attributes[SOLC_JSON_NAME].asString();

    ExpressionVar ev;
    for (auto i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        //变量类型
        if((*i)->Case(SOLC_JSONNAME_VARIABLE))
        {
            strVarName = (*i)->m_Attributes[SOLC_JSON_NAME].asString();
            strType = (*i)->m_Attributes[SOLC_JSON_TYPE].asString();
        }
            //从字面常量初始化?从已定义的变量？或者接受者？或者形参？
        else if((*i)->Case(SOLC_JSONNAME_LITERAL) || (*i)->Case(SOLC_JSONNAME_IDENTI))
        {
            ev.m_strType    = strType;
            ev.m_strValue   = (*i)->m_Attributes[SOLC_JSON_VALUE].asString();
            ev.m_isConstant = (*i)->Case(SOLC_JSONNAME_IDENTI)?false:true;
        }
            //右值是一个表达式
        else if((*i)->Case(SOLC_JSONNAME_STATEMENT))
        {
            /*预留*/
        }
    }
    if(!ev.m_strValue.empty())
    {
        g_LLVM.SetContractVar(strType,strVarName,strVarName,ev);
    }
    return true;
}
/*局部变量*/
bool ASTreeLocaVerNode::Transfrom()
{
    Log(m_name);
    std::string strVarName;
    std::string strType;
    ExpressionVar ev;
    for (auto i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        //变量类型
        if((*i)->Case(SOLC_JSONNAME_VARIABLE))
        {
            strVarName = (*i)->m_Attributes[SOLC_JSON_NAME].asString();
            strType = (*i)->m_Attributes[SOLC_JSON_TYPE].asString();
        }
            //从字面常量初始化?从已定义的变量？或者接受者？或者形参？
        else if((*i)->Case(SOLC_JSONNAME_LITERAL) || (*i)->Case(SOLC_JSONNAME_IDENTI))
        {
            ev.m_strType    = strType;
            ev.m_strValue   = (*i)->m_Attributes[SOLC_JSON_VALUE].asString();
            ev.m_isConstant = (*i)->Case(SOLC_JSONNAME_IDENTI)?false:true;
        }
            //右值是一个表达式
        else if((*i)->Case(SOLC_JSONNAME_STATEMENT))
        {
            /*预留*/
        }
    }
    g_LLVM.CreateLocalVariable(strType,strVarName,strVarName,&ev);
    return true;
}
/**
 * 转换代码快节点
 * @param asTreeNode
 */
bool ASTreeBlockNode::Transfrom()
{
    Log(m_name);
    for (auto j = m_Children.begin(); j != m_Children.end(); ++j)
    {
        //部分节点还未处理，暂不放开
        if((*j)->Case(SOLC_JSONNAME_LOCALVAR)
           || (*j)->Case(SOLC_JSONNAME_STATEMENT)
           || (*j)->Case(SOLC_JSONNAME_WHILE)
           || (*j)->Case(SOLC_JSONNAME_IF))
        {
            (*j)->Transfrom();
        }
    }
    return true;
}
/**
 * if 语句块
 * @param asTreeNode
 */
bool ASTreeIFNode::Transfrom()
{
    Log("if"+m_name);
    auto i = m_Children.begin();
    std::string ifValue = "ifValue";
    //条件
    if((*i)->Case(SOLC_JSONNAME_LITERAL) || (*i)->Case(SOLC_JSONNAME_IDENTI))//字面常量
    {
        ExpressionVar ev((*i)->m_Attributes[SOLC_JSON_TYPE].asString()
                ,(*i)->m_Attributes[SOLC_JSON_VALUE].asString()
                ,(*i)->Case(SOLC_JSONNAME_IDENTI)?false:true);
        g_LLVM.CreateLocalVariable(LLVM_TYPE_BOOL,g_LLVM.m_NowContract,ifValue,&ev);
    }
    if((*i)->Case(SOLC_JSONNAME_BINARY))//二元表达式
    {
        (*i)->Transfrom();
        ifValue = ((ASTreeBinaryNode*)(*i))->m_strTmpVerName;
    }
    ++i;
    //Block1
    std::string strBlockName=g_LLVM.m_NowBlock->m_block->getName();
    std::string strBlockTrue,strBlockFalse,strBlockBreak;
    if(i != m_Children.end())
    {
        strBlockTrue = strBlockName;
        g_LLVM.CreateBlock(strBlockTrue);
        g_LLVM.SetInsertPoint(strBlockTrue);
        (*i)->Transfrom();
        g_LLVM.ExitNowBlock();
    }
    ++i;
    //Block2
    if(i != m_Children.end())
    {
        strBlockFalse = strBlockName;
        g_LLVM.CreateBlock(strBlockFalse);
        g_LLVM.SetInsertPoint(strBlockFalse);
        (*i)->Transfrom();
        g_LLVM.ExitNowBlock();
    }
    //Block3
    strBlockBreak = strBlockName;
    g_LLVM.CreateBlock(strBlockBreak);
    g_LLVM.SetInsertPoint(strBlockBreak);
    //后面的内容都在BLOCK3里
    g_LLVM.SetInsertPoint(strBlockName);
    g_LLVM.CreateIfExpression(ifValue,strBlockBreak,strBlockTrue,strBlockFalse);
    return true;
}
/**
 * 根节点转换
 * @return
 */
bool ASTreeRoot::Transfrom()
{
    g_LLVM.CreateModule(m_Attributes[SOLC_JSON_ABSPATH].asString());
    if(m_Children.size() > 0)
    {
        for (auto i = m_Children.begin(); i != m_Children.end(); ++i)
        {
            (*i)->Transfrom();
        }
    }
    return true;
}
/**
 * 合约节点转换
 * @return
 */
bool ASTreeContractNode::Transfrom()
{
    //成员变量类型
    Types types;
    //记录成员变量的位置
    MapVarIndex mvIndex;
    mvIndex.clear();
    types.clear();
    std::string strContractName = this->m_strContractName;
    //先统计成员变量
    if(this->m_Children.size() > 0)
    {
        int index = 0;
        for (auto i = this->m_Children.begin(); i != this->m_Children.end(); ++i)
        {
            if((*i)->Case(SOLC_JSONNAME_VARIABLE))
            {
                //记录位置
                mvIndex.insert(pairVarIndex((*i)->m_Attributes[SOLC_JSON_NAME].asString(),index++));
                std::string strType = (*i)->m_Attributes[SOLC_JSON_TYPE].asString();
                types.push_back(g_LLVM.GetTypes(strType));
            }
        }
    }
    g_LLVM.CreateContractType(strContractName,&mvIndex);
    g_LLVM.SetContractVartype(strContractName,types);
    g_LLVM.CreateContract(strContractName,strContractName);
    g_LLVM.setNowContract(strContractName);
    //外部定义
    if(this->m_Children.size() > 0)
    {
        for (auto i = this->m_Children.begin(); i != this->m_Children.end(); ++i)
        {
            if((*i)->Case(SOLC_JSONNAME_VARIABLE)
                    ||(*i)->Case(SOLC_JSONNAME_FUNC))
            {
                (*i)->Transfrom();
            }
        }
    }
    return true;
}
/**
 * 函数节点转换
 * @return
 */
bool ASTreeFuncNode::Transfrom()
{
    //传入参数类型
    Types argTypes;
    argTypes.clear();
    //形参数名
    ArgsName argsName;
    argsName.clear();
    std::string strFuncName = this->m_Attributes[SOLC_JSON_NAME].asString();
    auto i = this->m_Children.begin();
    if((*i)->m_Children.size() != 0)
    {
        for (auto j = (*i)->m_Children.begin(); j != (*i)->m_Children.end(); ++j)
        {
            std::string strType = (*j)->m_Attributes[SOLC_JSON_TYPE].asString();
            std::string strParaName = (*j)->m_Attributes[SOLC_JSON_NAME].asString();//记录参数名
            argsName.push_back(strParaName);
            argTypes.push_back(g_LLVM.GetTypes(strType));
        }
    }
    //返回参数
    ++i;
    if((*i)->m_Children.size() != 0)
    {
        for (auto j = (*i)->m_Children.begin(); j != (*i)->m_Children.end(); ++j)
        {
            /*
             * 针对多个返回参数的还没想好怎么搞
             */

        }
    }
    g_LLVM.CreateFunc(strFuncName,LLVM_TYPE_VOID,&argTypes,&argsName);
    ++i;
    //根Block
    g_LLVM.CreateBlock(strFuncName,g_LLVM.GetFunction(strFuncName));
    g_LLVM.SetInsertPoint(strFuncName);
    if((*i)->m_Children.size() != 0)
    {
        for (auto j = (*i)->m_Children.begin(); j != (*i)->m_Children.end(); ++j)
        {
            //部分节点还未处理，暂不放开
            if((*j)->Case(SOLC_JSONNAME_LOCALVAR)
               || (*j)->Case(SOLC_JSONNAME_STATEMENT)
               || (*j)->Case(SOLC_JSONNAME_WHILE)
               || (*j)->Case(SOLC_JSONNAME_IF))
            {
                (*j)->Transfrom();
            }
        }
    }
    return true;
}
#endif //LLVM_SOL_SOLCMAIN_H
