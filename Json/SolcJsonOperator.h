//
// Created by wang on 27/04/18.
//

#ifndef LLVM_SOL_SOLCJSONOPERATOR_H
#define LLVM_SOL_SOLCJSONOPERATOR_H
#define SOLC_JSON_ATTRIBUTES    "attributes"
#define SOLC_JSON_CHILDREN      "children"
#define SOLC_JSON_NAME          "name"
#define SOLC_JSON_TYPE          "type"
#define SOLC_JSON_ID            "id"
#define SOLC_JSON_SRC           "src"
#define SOLC_JSON_VALUE         "value"
#define SOLC_JSON_ABSPATH       "absolutePath"
#define SOLC_JSON_OPERATOR      "operator"

#define SOLC_JSONNAME_SOURCE        "SourceUnit"
#define SOLC_JSONNAME_PRAGMA        "PragmaDirective"
#define SOLC_JSONNAME_CONTRACT      "ContractDefinition"
#define SOLC_JSONNAME_VARIABLE      "VariableDeclaration"
#define SOLC_JSONNAME_LOCALVAR      "VariableDeclarationStatement"
#define SOLC_JSONNAME_FUNC          "FunctionDefinition"
#define SOLC_JSONNAME_TYPE          "ElementaryTypeName"
#define SOLC_JSONNAME_LITERAL       "Literal"
#define SOLC_JSONNAME_BOLCK         "Block"
#define SOLC_JSONNAME_STATEMENT     "ExpressionStatement"
#define SOLC_JSONNAME_BINARY        "BinaryOperation"
#define SOLC_JSONNAME_TUPLE         "TupleExpression"
#define SOLC_JSONNAME_CONDITIONAL   "Conditional"
#define SOLC_JSONNAME_ASSIGNMENT    "Assignment"
#define SOLC_JSONNAME_IDENTI        "Identifier"
#define SOLC_JSONNAME_IF            "IfStatement"
#define SOLC_JSONNAME_WHILE         "WhileStatement"
#define SOLC_JSONNAME_RETURN        "Return"
#include "json/json.h"
#include <list>
using namespace std;
/*这个class....*/
class ASTreeNode{
public:
    int m_id;
    std::string m_name;
    std::string m_src;
    Json::Value m_Attributes;
    std::list<ASTreeNode*> m_Children;
    ASTreeNode* m_ParentNode;
    ASTreeNode(){
        m_name = "";
        m_id = 0;
        m_src = "";
        m_Attributes = Json::nullValue;
        m_ParentNode = nullptr;
        m_Children.clear();
    };
    ASTreeNode operator=(ASTreeNode ast)
    {
        this->m_Children.clear();
        this->m_Children = ast.m_Children;
        this->m_name = ast.m_name;
        this->m_src = ast.m_src;
        this->m_id = ast.m_id;
        this->m_Attributes = ast.m_Attributes;
        this->m_ParentNode = ast.m_ParentNode;
    }
    virtual ~ASTreeNode()
    {

    }
    bool Case(std::string strCaseName)const
    {
        return (this->m_name.compare(strCaseName) == 0)?true: false;
    }
    virtual bool Transfrom()
    {

    }
};
class ASTreeRoot:public ASTreeNode{
public:
    bool Transfrom() override;
};
class ASTreeContractNode:public ASTreeNode{
public:
    std::string m_strContractName;
    bool Transfrom() override;
};
class ASTreeFuncNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
class ASTreeBlockNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
class ASTreeStatementNode:public ASTreeNode{
public:
    bool Transfrom() override;
};

/**=====运算指令节点=============**/
class ASTreeBinaryNode:public ASTreeNode{
public:
    std::string m_strTmpVerName;
    bool Transfrom() override;
};
class ASTreeTupleNode:public ASTreeNode{
public:
    std::string m_strTmpVerName;
    bool Transfrom() override;
};
class ASTreeConditionalNode:public ASTreeNode{
public:
    std::string m_strTmpVerName;
    bool Transfrom() override;
};

class ASTreeAssignmentNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
/**=========变量定义和初始化**/
class ASTreeVarDeclarationNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
class ASTreeLocaVerNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
/**==========条件和控制结构==========**/
class ASTreeIFNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
class ASTreeWhileNode:public ASTreeNode{
public:
    bool Transfrom() override;
};
//typedef std::list<ASTreeNode*> ASTreeNodeList;

class SolcJsonOperator {
private:
    /*JSON 数据*/
    Json::Value m_JsonRoot;
public:
    SolcJsonOperator();
    /*创建JSON 解析器*/
    SolcJsonOperator(string strFileName);
    Json::Value GetRootChildens();
    Json::Value GetRootAttributes();
    Json::Value GetValue(initializer_list<std::string> strArgs);
    bool SolcParseJson(Json::Value jsonRoot,ASTreeNode& asTreeNode);
    bool ParseJson(ASTreeNode &asTreeNode);
    virtual ~SolcJsonOperator();
    void readASTree(const ASTreeNode& asTreeNode);
    void ClearASTree(ASTreeNode& asTreeNode);
public:
    void parseItemRoot(std::string  strfileName);
};
#endif //LLVM_SOL_SOLCJSONOPERATOR_H
