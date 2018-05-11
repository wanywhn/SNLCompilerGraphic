//
// Created by tender on 18-5-11.
//

#include "utils.h"

TreeNode *newRootNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = ProK;
    t->lineno = lineno;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}


/********************************************************/
/* 功  能 创建程序头类型语法树节点函数					*/
/********************************************************/
TreeNode *newPheadNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = PheadK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}


/********************************************************/
/* 功  能 创建声明语法树节点函数,没有指明具体的节点声明 */
/*        类型,在语法树的第二层			                */
/********************************************************/
TreeNode *newDecANode(NodeKind kind) {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = kind;
    t->lineno = lineno;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}



//尝试


/********************************************************/
/* 功  能 类型标志语法树节点创建函数					*/
/********************************************************/
TreeNode *newTypeNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = TypeK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++)
        t->table[i] = nullptr;

    return t;
}

/********************************************************/
/* 功  能 变量声明标志语法树节点创建函数				*/
/********************************************************/
TreeNode *newVarNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = VarK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++)
        t->table[i] = nullptr;

    return t;
}

//试试，如果行的话，就把它们合成一个！！！！






/********************************************************/
/* 功  能 创建声明类型语法树节点函数					*/
/********************************************************/
TreeNode *newDecNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = DecK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}


/********************************************************/
/* 功  能 创建函数类型语法树节点函数					*/
/********************************************************/
TreeNode *newProcNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = ProcDecK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}

/********************************************************/
/* 功  能 创建语句标志类型语法树节点函数				*/
/********************************************************/
TreeNode *newStmlNode() {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = StmLK;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}

/********************************************************/
/* 功  能 创建语句类型语法树节点函数					*/
/********************************************************/
TreeNode *newStmtNode(StmtKind kind) {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->idnum = 0;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }
    return t;
}


/********************************************************/
/* 功  能 表达式类型语法树节点创建函数					*/
/********************************************************/
TreeNode *newExpNode(ExpKind kind) {
    auto t = new TreeNode;

    int i;

    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
    t->sibling = nullptr;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->attr.ExpAttr.varkind = IdV;
    t->attr.ExpAttr.type = Void;
    for (i = 0; i < 10; i++) {
        strcpy(t->name[i], "\0");
        t->table[i] = nullptr;
    }

    return t;
}
