//
// Created by wang on 27/04/18.
// V1.0.0. 代码很乱，小心驾驶
//

#include "SolcJsonOperator.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace Json;
SolcJsonOperator::~SolcJsonOperator() {}
SolcJsonOperator::SolcJsonOperator() {}
SolcJsonOperator::SolcJsonOperator(std::string strfileName) {
    ifstream ifs;
    ifs.open(strfileName);
    Json::Reader reader;
    if (!reader.parse(ifs, m_JsonRoot, false))
    {
        cerr << "parse failed \n";
        ifs.close();
        return;
    }
    ifs.close();
}
Json::Value SolcJsonOperator::GetRootAttributes() {
    return  m_JsonRoot.get(SOLC_JSON_ATTRIBUTES,nullValue);
}
Json::Value SolcJsonOperator::GetRootChildens() {
    return m_JsonRoot.get(SOLC_JSON_CHILDREN,nullValue);
}
Json::Value SolcJsonOperator::GetValue(initializer_list<std::string> strArgs) {
    Json::Value v_Tmp = m_JsonRoot;
    for(auto p = strArgs.begin(); p != strArgs.end(); p++)   //使用迭代器访问参数
    {
        if(v_Tmp.type() == arrayValue){//JSON 数组
            int i = std::stoi(*p);
            if(i<v_Tmp.size()) {
                v_Tmp = v_Tmp[i];
            }else{
                v_Tmp = nullValue;
            }
        }else if(v_Tmp.type() == nullValue){//空的
            return nullValue;
        }else if(v_Tmp.type() == Json::ValueType::objectValue){
            v_Tmp = v_Tmp.get(*p,nullValue);
        }else{
            v_Tmp = nullValue;
        }
    }
    return  v_Tmp;
}
void SolcJsonOperator::parseItemRoot(std::string strfileName)
{
    string name = m_JsonRoot[SOLC_JSON_NAME].asString(); // 实际字段保存在这里
    cout<<name<<endl;
}
bool SolcJsonOperator::SolcParseJson(Json::Value jsonRoot,ASTreeNode& asTreeNode) {
    asTreeNode.m_Attributes = jsonRoot[SOLC_JSON_ATTRIBUTES];
    asTreeNode.m_name = jsonRoot[SOLC_JSON_NAME].asString();
    asTreeNode.m_id = jsonRoot[SOLC_JSON_ID].asInt();
    asTreeNode.m_src = jsonRoot[SOLC_JSON_SRC].asString();
    if(jsonRoot[SOLC_JSON_CHILDREN].type() == arrayValue){
        std::list<ASTreeNode*> listAST;
        listAST.clear();
        for (int i = 0; i < jsonRoot[SOLC_JSON_CHILDREN].size(); ++i) {
            if(jsonRoot[SOLC_JSON_CHILDREN][i].type() == objectValue){
                /**/
                ASTreeNode *astNode = nullptr;
                /*合约节点*/
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_CONTRACT)
                {
                    ASTreeContractNode *pNode = new ASTreeContractNode();
                    pNode->m_strContractName = jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_ATTRIBUTES][SOLC_JSON_NAME].asString();
                    astNode = pNode;
                }
                /*函数节点*/
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_FUNC)
                {
                    astNode = new ASTreeFuncNode();
                }
                /*成员变量节点*/
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_VARIABLE)
                {
                    astNode = new ASTreeVarDeclarationNode();
                }
                /*局部变量节点*/
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_LOCALVAR)
                {
                    astNode = new ASTreeLocaVerNode();
                }
                /*Block节点*/
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_BOLCK)
                {
                    astNode = new ASTreeBlockNode();
                }
                /*if else节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_IF)
                {
                    astNode = new ASTreeIFNode();
                }
                /*while 节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_WHILE)
                {
                    astNode = new ASTreeWhileNode();
                }
                /*表达式节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_STATEMENT)
                {
                    astNode = new ASTreeStatementNode();
                }
                /*赋值 初始化 节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_ASSIGNMENT)
                {
                    astNode = new ASTreeAssignmentNode();
                }
                /*二元运算 节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_BINARY)
                {
                    astNode = new ASTreeBinaryNode();
                }
                /*三元运算 节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_TUPLE)
                {
                    astNode = new ASTreeTupleNode();
                }
                /*三元表达式 节点 */
                if(jsonRoot[SOLC_JSON_CHILDREN][i][SOLC_JSON_NAME].asString() == SOLC_JSONNAME_CONDITIONAL)
                {
                    astNode = new ASTreeConditionalNode();
                }
                if(astNode == nullptr)
                {
                    astNode = new ASTreeNode();
                }
                astNode->m_ParentNode = &asTreeNode;//指向父节点
                SolcParseJson(jsonRoot[SOLC_JSON_CHILDREN][i],*astNode);
                listAST.push_back(astNode);
            }
        }
        asTreeNode.m_Children = listAST;
    }
    return true;
}
bool SolcJsonOperator::ParseJson(ASTreeNode &asTreeNode)
{
    if(m_JsonRoot.type() != objectValue)
    {
        return false;
    }
    SolcParseJson(m_JsonRoot,asTreeNode);
    return true;
}
void SolcJsonOperator::readASTree(const  ASTreeNode &asTreeNode)
{
    std::cout<<"log:"<<asTreeNode.m_name<<std::endl;
    if(asTreeNode.m_Children.size() > 0)
    {
        for (auto i = asTreeNode.m_Children.begin(); i != asTreeNode.m_Children.end(); ++i)
        {
            readASTree(**i);
        }
    }
}
void SolcJsonOperator::ClearASTree(ASTreeNode &asTreeNode)
{
    if(asTreeNode.m_Children.size() > 0)
    {
        for (auto i = asTreeNode.m_Children.begin(); i != asTreeNode.m_Children.end(); ++i)
        {
            ClearASTree(**i);
            delete(*i);
        }
        asTreeNode.m_Children.clear();
    }
}