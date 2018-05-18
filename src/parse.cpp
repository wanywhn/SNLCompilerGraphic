#include "parse.h"
#include "globals.h"
#include "utils.h"
#include <QDebug>

void Parse::set_token_head(Token *head) {
    this->head = head;
}

Parse::Parse() : root(nullptr), line0(0) {

}

void Parse::run() {
    root = program();
    if (head->getLex() != ENDFILE) {
        syntaxError("end before file");
    } else {
        //TODO Success
        emit parse_success();
    }
}

/**
 * program ::= programHead declarePart programBody
 * @return
 */
TreeNode *Parse::program() {
    TreeNode *ph = programHead();
    TreeNode *dp = declarePart();
    TreeNode *pb = programBody();

    root = newRootNode();
    if (nullptr != root) {
        root->lineno = 0;
        if (nullptr != ph) root->child[0] = ph;
        else syntaxError("need a program head");
        if (nullptr != dp) root->child[1] = dp;
        if (nullptr != pb) root->child[2] = pb;
        else syntaxError("nedd a program body");
    }
    match(DOT);

    return root;
}

void Parse::syntaxError(QString msg) {

    qDebug()<<msg;
    //TODO show the msg
}

void Parse::match(LexType expected) {
    if (nullptr!=head&&head->getLex() == expected) {
        head = head->next;
        if(head!=nullptr){
        line0 = head->getLine();
        lineno=line0;

        }
    } else {
        syntaxError("ERROR not match,except:"+lexName[expected]+" get:"+lexName[head->getLex()]+" in line "+head->getLine());
        exit(0);
    }

}

/**
 * programHead ::=PROGRAM ProgramName
 * @return
 */
TreeNode *Parse::programHead() {
    TreeNode *t = newPheadNode();
    match(PROGRAM);
    if ((nullptr != t) && (head->getLex() == ID)) {
        t->lineno = 0;
        strcpy(t->name[0], head->getSem().toStdString().c_str());
    match(ID);
    }else{
        syntaxError("need a program name"+lineno);
    }
    return t;
}


/*
 * declarePart ::=typeDec varDec procDec
 */
TreeNode *Parse::declarePart() {
    TreeNode *tp = newDecANode(TypeK);
    TreeNode *pp = tp;
    if (nullptr != tp) {
        tp->lineno = 0;//TODO lineno=lineno??
        TreeNode *tp1 = typeDec();
        if (nullptr != tp1)
            tp->child[0] = tp1;

        else {
            free(tp);
            tp = nullptr;
        }
    }
    TreeNode *varp = newDecANode(VarK);
    if (nullptr != varp) {
        varp->lineno = 0;//TODO lineno??
        TreeNode *tp2 = varDec();
        if (nullptr != tp2)
            varp->child[0] = tp2;
        else {
            free(varp);
            varp = nullptr;
        }
    }

    TreeNode *proc = procDec();
    if (nullptr == proc) {}
    if (nullptr == varp) { varp = proc; };
    if (nullptr == tp) { pp = tp = varp; };
    if (tp != varp) {
        tp->sibling = varp;
        tp = varp;
    }
    if (varp != proc) {
        varp->sibling = proc;
        varp = proc;
    };
    return pp;
}


/**
 * typeDec ::=0|TypeDeclaration
 * @return
 */
TreeNode *Parse::typeDec() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case TYPE:
            t = typeDeclaration();
            break;
        case VAR:
        case PROCEDURE:
        case BEGIN:
            break;
        default :
            syntaxError("unexpected token:" + head->getSem());
            head = head->next;
            break;
    }
    return t;
}

/**
 * TypeDeclaration ::=TYPE TypeDecList
 * @return
 */
TreeNode *Parse::typeDeclaration() {
    match(TYPE);
    auto t = typeDecList();
    if (nullptr == t) {
        syntaxError("need a declaration");
    }
    return t;

}

/**
 * TypeDecList ::= TypeId=TypeDef;TypeDecMore
 * @return
 */
TreeNode *Parse::typeDecList() {
    auto t = newDecNode();
    if (nullptr != t) {
        t->lineno = line0;
        typeId(t);
        match(EQ);
        typeName(t);
        match(SEMI);
        auto *more = typeDecMore();
        if (nullptr != more) {
            t->sibling = more;
        }
    }
    return t;
}

/**
 * typeId ::=id
 * @param pNode
 */
void Parse::typeId(TreeNode *pNode) {

    int tnum = pNode->idnum;
    if (head->getLex() == ID) {
        strcpy(pNode->name[tnum], head->getSem().toStdString().c_str());
        tnum += 1;
    }
    pNode->idnum = tnum;
    match(ID);

}

/**
 * typeName ::=baseType|structureType|id
 * @param pNode
 */
void Parse::typeName(TreeNode *pNode) {
    if (nullptr != pNode) {
        switch (head->getLex()) {
            case INTEGER_T:
            case CHAR_T:
                baseType(pNode);
                break;
            case ARRAY:
            case RECORD:
                structureType(pNode);
                break;
            case ID:
                pNode->kind.dec = IdK;
                strcpy(pNode->type_name, head->getSem().toStdString().c_str());
                match(ID);
                break;
            default :
                head = head->next;
                syntaxError("unexpected toekn");
                break;
        }
    }

}

/**
 * baseType ::= INTEGER|CHAR
 * @param pNode
 */
void Parse::baseType(TreeNode *pNode) {
    switch (head->getLex()) {
        case INTEGER_T:
            match(INTEGER_T);
            pNode->kind.dec = IntegerK;
            break;
        case CHAR_T:
            match(CHAR_T);
            pNode->kind.dec = CharK;
            break;
        default:
            head = head->next;
            syntaxError("unexpected token");
            break;
    }

}

/**
 * structureType :==arrayType|recType
 * @param pNode
 */
void Parse::structureType(TreeNode *pNode) {
    switch (head->getLex()) {
        case ARRAY:
            arrayType(pNode);
            break;
        case RECORD:
            pNode->kind.dec = RecordK;
            recType(pNode);
            break;
        default:
            head = head->next;
            syntaxError("unexpected token");
            break;
    }

}

/**
 * arrayTyoe ::=ARRAY [low..top] OF BaseType
 * @param pNode
 */
void Parse::arrayType(TreeNode *pNode) {
    match(ARRAY);
    match(LMIDPAREN);
    if (head->getLex() == INTC_VAL) {
        pNode->attr.ArrayAttr.low = head->getSem().toInt();
    }
    match(INTC_VAL);
    match(UNDERRANGE);
    if (head->getLex() == INTC_VAL) {
        pNode->attr.ArrayAttr.up = head->getSem().toInt();
    }
    match(INTC_VAL);
    match(RMIDPAREN);
    match(OF);
    baseType(pNode);
    pNode->attr.ArrayAttr.childtype = pNode->kind.dec;
    pNode->kind.dec = ArrayK;


}

/**
 * recType ::= RECORD fieldDecLIst END
 * @param pNode
 */
void Parse::recType(TreeNode *pNode) {
    match(RECORD);
    TreeNode *p = nullptr;
    p = fieldDecList();
    if (nullptr != p) {
        pNode->child[0] = p;
    } else syntaxError("need a record body");
    match(END);


}

/**
 * fieldDecList ::=baseType idList;FieldDecMore|ArrayType idList;FieldDecMore
 * @return
 */
TreeNode *Parse::fieldDecList() {
    auto *t = newDecNode();
    TreeNode *p = nullptr;
    t->lineno = line0;
    switch (head->getLex()) {
        case INTEGER_T:
        case CHAR_T:
            baseType(t);
            idList(t);
            match(SEMI);
            p = fieldDecMore();
            break;
        case ARRAY:
            arrayType(t);
            idList(t);
            match(SEMI);
            p = fieldDecMore();
            break;
        default :
        //    head = head->next;
            syntaxError("unexpected token");
            break;
    }
    t->sibling = p;

}

/**
 * idList ::= ID IdMore
 * @param pNode
 */
void Parse::idList(TreeNode *pNode) {
    if (head->getLex() == ID) {
        strcpy(pNode->name[pNode->idnum], head->getSem().toStdString().c_str());
        match(ID);
        pNode->idnum += 1;
    }
    idMore(pNode);

}

/**
 * idMore ::=0|,IdList
 * @param pNode
 */
void Parse::idMore(TreeNode *pNode) {
    switch (head->getLex()) {
        case SEMI:
            break;
        case COMMA:
            match(COMMA);
            idList(pNode);
            break;
        default :
        //    head = head->next;
            syntaxError("unexpected token");
            break;
    }
}

/**
 * fieldDeMore ::=0|FieldDecList
 * @return
 */
TreeNode *Parse::fieldDecMore() {
    TreeNode *p = nullptr;
    switch (head->getLex()) {
        case END:
            break;
        case INTEGER_T:
        case CHAR_T:
        case ARRAY:
            p = fieldDecList();
            break;
        default :
        //    head = head->next;
            syntaxError("unexpected token");
            break;
    }
    return p;
}

/**
 * typeDecMore ::=0|TypeDecList
 * @return
 */
TreeNode *Parse::typeDecMore() {
    TreeNode *p = nullptr;
    switch (head->getLex()) {
        case VAR:
        case PROCEDURE:
        case BEGIN:
            break;
        case ID:
            p = typeDecList();
            break;

        default:
          //  head = head->next;
            syntaxError("unexpected token");
            break;
    }
    return p;
}
/********************************************************************/
/* 产生式 < varDec > ::=  ε |  varDeclaration                      */
/********************************************************************/
TreeNode *Parse::varDec() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case PROCEDURE:
        case BEGIN:
            break;
        case VAR:
            t = varDeclaration();
            break;
        default:
            //head = head->next;
            syntaxError("unexpected token is here! "+lexName[head->getLex()]+head->getLine());
            break;
    }
    return t;
}


/********************************************************************/
/* 产生式 < varDeclaration > ::=  VAR  varDecList                   */
/********************************************************************/
TreeNode *Parse::varDeclaration() {
    match(VAR);
    TreeNode *t = varDecList();
    if (t == nullptr)
        syntaxError("a var declaration is expected!");
    return t;
}


/********************************************************************/
/* 产生式 < varDecList > ::=  typeName varIdList; varDecMore        */
/********************************************************************/
TreeNode *Parse::varDecList() {
    TreeNode *t = newDecNode();
    TreeNode *p = nullptr;

    if (t != nullptr) {
        t->lineno = line0;
        typeName(t);
        varIdList(t);
        match(SEMI);
        p = varDecMore();
        t->sibling = p;
    }
    return t;
}

/********************************************************************/
/* 产生式 < varDecMore > ::=  ε |  varDecList                      */
/********************************************************************/
TreeNode *Parse::varDecMore() {
    TreeNode *t = nullptr;

    switch (head->getLex()) {
        case PROCEDURE:
        case BEGIN:
            break;
        case INTEGER_T:
        case CHAR_T:
        case ARRAY:
        case RECORD:
        case ID:
            t = varDecList();
            break;
        default:
      //      head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}
/********************************************************************/
/* 产生式 < varIdList > ::=  id  varIdMore                          */
/********************************************************************/
void Parse::varIdList(TreeNode *t) {
    if (head->getLex() == ID) {
        strcpy(t->name[(t->idnum)], head->getSem().toStdString().c_str());
        match(ID);
        t->idnum = (t->idnum) + 1;
    } else {
        syntaxError("a varid is expected here!");
   //     head = head->next;
    }
    varIdMore(t);
}


/********************************************************************/
/* 产生式 < varIdMore > ::=  ε |  , varIdList                      */
/********************************************************************/
void Parse::varIdMore(TreeNode *t) {
    switch (head->getLex()) {
        case SEMI:
            break;
        case COMMA:
            match(COMMA);
            varIdList(t);
            break;
        default:
    //        head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
}
/********************************************************************/
/* 产生式 < programBody > ::=  BEGIN  stmList   END                 */
/********************************************************************/
TreeNode *Parse::programBody() {
    TreeNode *t = newStmlNode();
    match(BEGIN);
    if (t != nullptr) {
        t->lineno = 0;
        t->child[0] = stmList();
    }
    match(END);
    return t;
}


/********************************************************************/
/* 产生式 < stmList > ::=  stm    stmMore                           */
/********************************************************************/
TreeNode *Parse::stmList() {
    TreeNode *t = stm();
    TreeNode *p = stmMore();
    if (t != nullptr)
        if (p != nullptr)
            t->sibling = p;
    return t;
}

/********************************************************************/
/* 产生式 < stmMore > ::=   ε |  ; stmList                         */
/********************************************************************/
TreeNode *Parse::stmMore() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case ELSE:
        case FI:
        case END:
        case ENDWH:
            break;
        case SEMI:
            match(SEMI);
            t = stmList();
            break;
        default:
      //      head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/********************************************************************/
/* 产生式 < stm > ::=   conditionalStm   {IF}                       */
/*                    | loopStm          {WHILE}                    */
/*                    | inputStm         {READ}                     */
/*                    | outputStm        {WRITE}                    */
/*                    | returnStm        {RETURN}                   */
/*                    | id  assCall      {id}                       */
/********************************************************************/
TreeNode *Parse::stm() {

    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case IF:
            t = conditionalStm();
            break;
        case WHILE:
            t = loopStm();
            break;
        case READ:
            t = inputStm();
            break;
        case WRITE:
            t = outputStm();
            break;
        case RETURN:
            t = returnStm();
            break;
        case ID:
            temp_name = head->getSem();
            match(ID);
            t = assCall();
            break;
        default:
      //      head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/********************************************************************/
/* 产生式 < assCall > ::=   assignmentRest   {:=,LMIDPAREN,DOT}     */
/*                        | callStmRest      {(}                    */
/********************************************************************/
TreeNode *Parse::assCall() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case ASSIGN:
        case LMIDPAREN:
        case DOT:
            t = assignmentRest();
            break;
        case LPAREN:
            t = callStmRest();
            break;
        default:
     //       head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/********************************************************************/
/* 产生式 < assignmentRest > ::=  variMore : = exp                  */
/********************************************************************/
TreeNode *Parse::assignmentRest() {
    TreeNode *t = newStmtNode(AssignK);

    /* 赋值语句节点的第一个儿子节点记录赋值语句的左侧变量名，*
     * 第二个儿子结点记录赋值语句的右侧表达式*/
    if (t != nullptr) {
        t->lineno = line0;

        /*处理第一个儿子结点，为变量表达式类型节点*/
        TreeNode *child1 = newExpNode(VariK);
        if (child1 != nullptr) {
            child1->lineno = line0;
            strcpy(child1->name[0], temp_name.toStdString().c_str());
            (child1->idnum)++;
            variMore(child1);
            t->child[0] = child1;
        }

        /*赋值号匹配*/
        match(ASSIGN);

        /*处理第二个儿子节点*/
        t->child[1] = mexp();

    }
    return t;
}

/********************************************************************/
/* 产生式 < conditionalStm > ::= IF exp THEN stmList ELSE stmList FI*/
/********************************************************************/
TreeNode *Parse::conditionalStm() {
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    if (t != nullptr) {
        t->lineno = line0;
        t->child[0] = mexp();
    }
    match(THEN);
    if (t != nullptr) t->child[1] = stmList();
    if (head->getLex() == ELSE) {
        match(ELSE);
        if (t != nullptr)
            t->child[2] = stmList();
    }
    match(FI);
    return t;
}


/********************************************************************/
/* 产生式 < loopStm > ::=      WHILE exp DO stmList ENDWH           */
/********************************************************************/
TreeNode *Parse::loopStm() {
    TreeNode *t = newStmtNode(WhileK);
    match(WHILE);
    if (t != nullptr) {
        t->lineno = line0;
        t->child[0] = mexp();
        match(DO);
        t->child[1] = stmList();
        match(ENDWH);
    }
    return t;
}

/********************************************************************/
/* 产生式 < inputStm > ::=    READ(id)                              */
/********************************************************************/
TreeNode *Parse::inputStm() {
    TreeNode *t = newStmtNode(ReadK);
    match(READ);
    match(LPAREN);
    if ((t != nullptr) && (head->getLex() == ID)) {
        t->lineno = line0;
        strcpy(t->name[0], head->getSem().toStdString().c_str());
        (t->idnum)++;
    }
    match(ID);
    match(RPAREN);
    return t;
}

/********************************************************************/
/* 产生式 < outputStm > ::=   WRITE(exp)                            */
/********************************************************************/
TreeNode *Parse::outputStm() {
    TreeNode *t = newStmtNode(WriteK);
    match(WRITE);
    match(LPAREN);
    if (t != nullptr) {
        t->lineno = line0;
        t->child[0] = mexp();
    }
    match(RPAREN);
    return t;
}

/********************************************************************/
/* 产生式 < returnStm > ::=   RETURN                                */
/********************************************************************/
TreeNode *Parse::returnStm() {
    TreeNode *t = newStmtNode(ReturnK);
    match(RETURN);
    if (t != nullptr)
        t->lineno = line0;
    return t;
}

/********************************************************************/
/* 产生式 < callStmRest > ::=  (actParamList)                       */
/********************************************************************/
TreeNode *Parse::callStmRest() {
    TreeNode *t = newStmtNode(CallK);
    match(LPAREN);
    /*函数调用时，其子节点指向实参*/
    if (t != nullptr) {
        t->lineno = line0;

        /*函数名的结点也用表达式类型结点*/
        TreeNode *child0 = newExpNode(VariK);
        if (child0 != nullptr) {
            child0->lineno = line0;
            strcpy(child0->name[0], temp_name.toStdString().c_str());
            (child0->idnum)++;
            t->child[0] = child0;
        }
        t->child[1] = actParamList();
    }
    match(RPAREN);
    return t;
}

/**
/* 函数名 actParamList
/* 产生式 < actParamList > ::=     ε |  exp actParamMore
**/
TreeNode *Parse::actParamList() {
    TreeNode *t = nullptr;

    switch (head->getLex()) {
        case RPAREN:
            break;
        case ID:
        case INTC_VAL:
            t = mexp();
            if (t != nullptr)
                t->sibling = actParamMore();
            break;
        default:
    //        head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/**
 * 功  能 函数调用实参部分的处理函数
 * 产生式 < actParamMore > ::=     ε |  , actParamList
 */
TreeNode *Parse::actParamMore() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case RPAREN:
            break;
        case COMMA:
            match(COMMA);
            t = actParamList();
            break;
        default:
      //      head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}



/*************************表达式部分********************************/
/****************************************************************************/
/* 产生式 < 表达式 > ::= < 简单表达式 > [< 关系运算符 > < 简单表达式 > ]	*/
/****************************************************************************/
TreeNode *Parse::mexp() {
    /* 调用简单表达式处理函数simple_exp(),返回语法树节点指针给t */
    TreeNode *t = simple_exp();

    /* 当前单词token为逻辑运算单词LT或者EQ */
    if ((head->getLex() == LT) || (head->getLex() == EQ)) {
        /* 创建新的OpK类型语法树节点，新语法树节点指针赋给p */
        TreeNode *p = newExpNode(OpK);

        /* 新语法树节点p创建成功,初始化p第一个子节点成员child[0]
         * 并将当前单词token(为EQ或者LT)赋给语法树节点p的运算符成员attr.op*/
        if (p != nullptr) {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = head->getLex();

            /* 将新的表达式类型语法树节点p作为函数返回值t */
            t = p;
        }

        /* 当前单词token与指定逻辑运算符单词(为EQ或者LT)匹配 */
        match(head->getLex());

        /* 语法树节点t非空,调用简单表达式处理函数simple_exp()	*
         * 函数返回语法树节点指针给t的第二子节点成员child[1]	*/
        if (t != nullptr)
            t->child[1] = simple_exp();
    }

    /* 函数返回表达式类型语法树节点t */
    return t;
}


/************************************************************************/
/* 产生式 < 简单表达式 >::=	< 项 > { < 加法运算符 > < 项 > }			*/
/************************************************************************/
TreeNode *Parse::simple_exp() {
    /* 调用元处理函数term(),函数返回语法树节点指针给t */
    TreeNode *t = term();

    /* 当前单词token为加法运算符单词PLUS或MINUS */
    while ((head->getLex() == PLUS) || (head->getLex() == MINUS)) {
        /* 创建新OpK表达式类型语法树节点，新语法树节点指针赋给p */
        TreeNode *p = newExpNode(OpK);

        /* 语法树节点p创建成功,初始化p第一子节点成员child[0]	*
        * 返回语法树节点指针给p的运算符成员attr.op				*/
        if (p != nullptr) {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = head->getLex();

            /* 将函数返回值t赋成语法树节点p */
            t = p;

            /* 当前单词token与指定加法运算单词(为PLUS或MINUS)匹配 */
            match(head->getLex());

            /* 调用元处理函数term(),函数返回语法树节点指针给t的第二子节点成员child[1] */
            t->child[1] = term();
        }
    }
    /* 函数返回表达式类型语法树节点t */
    return t;
}


/********************************************************************/
/* 产生式 < procDec > ::=  ε |  procDeclaration                    */
/********************************************************************/
TreeNode *Parse::procDec() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case BEGIN:
            break;
        case PROCEDURE:
            t = procDeclaration();
            break;
        default:
            //head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}


/********************************************************************/
/* 产生式 < formList > ::=  id  fidMore                             */
/********************************************************************/
void Parse::formList(TreeNode *t) {
    if (head->getLex() == ID) {
        strcpy(t->name[(t->idnum)], head->getSem().toStdString().c_str());
        t->idnum = (t->idnum) + 1;
        match(ID);
    }
    fidMore(t);
}


/********************************************************************/
/* 产生式 < fidMore > ::=   ε |  , formList                        */
/********************************************************************/
void Parse::fidMore(TreeNode *t) {
    switch (head->getLex()) {
        case SEMI:
        case RPAREN:
            break;
        case COMMA:
            match(COMMA);
            formList(t);
            break;
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
}
/********************************************************************/
/* 产生式 < procDeclaration > ::=  PROCEDURE                        */
/*                                 ProcName(paramList);             */
/*                                 procDecPart                      */
/*                                 procBody                         */
/*                                 procDec                          */
/* 说  明 函数根据文法产生式,调用相应的递归处理函数,生成语法树节点  */
/*        函数的根节点用于记录该函数的名字；第一个子节点指向参数节  */
/*        点，第二个节点指向函数中的声明部分节点；第三个节点指向函  */
/*        数体。
/********************************************************************/
TreeNode *Parse::procDeclaration() {
    TreeNode *t = newProcNode();
    match(PROCEDURE);
    if (t != nullptr) {
        t->lineno = line0;
        if (head->getLex() == ID) {
            strcpy(t->name[0], head->getSem().toStdString().c_str());
            (t->idnum)++;
            match(ID);
        }
        match(LPAREN);
        paramList(t);
        match(RPAREN);
        match(SEMI);
        t->child[1] = procDecPart();
        t->child[2] = procBody();
        t->sibling = procDec();
    }
    return t;
}


/********************************************************************/
/* 产生式 < procBody > ::=  programBody                             */
/********************************************************************/
TreeNode *Parse::procBody() {
    TreeNode *t = programBody();
    if (t == nullptr)
        syntaxError("a program body is requested!");
    return t;
}

/********************************************************************/
/* 产生式 < procDecPart > ::=  declarePart                          */
/********************************************************************/
TreeNode *Parse::procDecPart() {
    TreeNode *t = declarePart();
    return t;
}



/********************************************************************/
/* 产生式 < paramList > ::=  ε |  paramDecList                     */
/********************************************************************/
void Parse::paramList(TreeNode *t) {
    TreeNode *p = nullptr;

    switch (head->getLex()) {
        case RPAREN:
            break;
        case INTEGER_T:
        case CHAR_T:
        case ARRAY:
        case RECORD:
        case ID:
        case VAR:
            p = paramDecList();
            t->child[0] = p;
            break;
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
}


/********************************************************************/
/* 产生式 < paramDecList > ::=  param  paramMore                    */
/********************************************************************/
TreeNode *Parse::paramDecList() {
    TreeNode *t = mparam();
    TreeNode *p = paramMore();
    if (p != nullptr) {
        t->sibling = p;
    }
    return t;
}

/********************************************************************/
/* 产生式 < paramMore > ::=  ε | ; paramDecList                     */
/********************************************************************/
TreeNode *Parse::paramMore() {
    TreeNode *t = nullptr;
    switch (head->getLex()) {
        case RPAREN:
            break;
        case SEMI:
            match(SEMI);
            t = paramDecList();
            if (t == nullptr)
                syntaxError("a param declaration is request!");
            break;
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/********************************************************************/
/* 产生式 < param > ::=  typeName formList | VAR typeName formList  */
/********************************************************************/
TreeNode *Parse::mparam() {
    TreeNode *t = newDecNode();
    if (t != nullptr) {
        t->lineno = line0;
        switch (head->getLex()) {
            case INTEGER_T:
            case CHAR_T:
            case ARRAY:
            case RECORD:
            case ID:
                t->attr.ProcAttr.paramt = valparamType;
                typeName(t);
                formList(t);
                break;
            case VAR:
                match(VAR);
                t->attr.ProcAttr.paramt = varparamType;
                typeName(t);
                formList(t);
                break;
            default:
                head = head->next;

                syntaxError("unexpected token is here!");
                break;
        }
    }
    return t;
}



/****************************************************************************/
/* 产生式 < 项 > ::= < 因子 > { < 乘法运算符 > < 因子 > }					*/
/****************************************************************************/
TreeNode *Parse::term() {
    /* 调用因子处理函数factor(),函数返回语法树节点指针给t */
    TreeNode *t = factor();

    /* 当前单词token为乘法运算符单词TIMES或OVER */
    while ((head->getLex() == TIMES) || (head->getLex() == DIVIDE)) {
        /* 创建新的OpK表达式类型语法树节点,新节点指针赋给p */
        treeNode *p = newExpNode(OpK);

        /* 新语法树节点p创建成功,初始化第一个子节点成员child[0]为t	*
         * 将当前单词token赋值给语法树节点p的运算符成员attr.op		*/
        if (p != nullptr) {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = head->getLex();
            t = p;
        }

        /* 当前单词token与指定乘法运算符单词(为TIMES或OVER)匹配 */
        match(head->getLex());

        /* 调用因子处理函数factor(),函数返回语法树节点指针赋给p第二个子节点成员child[1] */
        p->child[1] = factor();

    }
    /* 函数返回表达式类型语法树节点t */
    return t;
}


/****************************************************************************/
/* 产生式 factor ::= ( exp ) | INTC | variable                  			*/
/****************************************************************************/
TreeNode *Parse::factor() {
    /* 函数返回语法树节点指针t初始为为nullptr */
    TreeNode *t = nullptr;

    switch (head->getLex()) {
        case INTC_VAL :

            /* 创建新的ConstK表达式类型语法树节点,赋值给t */
            t = newExpNode(ConstK);

            /* 新语法树节点t创建成功,当前单词token为数字单词NUM						*
             * 将当前单词名tokenString转换为整数并赋给语法树节点t的数值成员attr.val	*/
            if ((t != nullptr) && (head->getLex() == INTC_VAL)) {
                t->lineno = line0;
                t->attr.ExpAttr.val = head->getSem().toInt();
            }

            /* 当前单词token与数字单词NUM匹配 */
            match(INTC_VAL);
            break;

            /* 当前单词token为标识符单词ID */
        case ID :

            /* 创建新的IdK表达式类型语法树节点t */
            t = variable();
            break;

            /* 当前单词token为左括号单词LPAREN */
        case LPAREN :

            /* 当前单词token与左括号单词LPAREN匹配 */
            match(LPAREN);

            /* 调用表达式处理函数exp(),函数返回语法树节点指针给t */
            t = mexp();

            /* 当前单词token与右括号单词RPAREN匹配 */
            match(RPAREN);

            break;

            /* 当前单词token为其它单词 */
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
    /* 函数返回表达式类型语法树节点t */
    return t;
}

/********************************************************************/
/* 功  能 变量处理函数												*/
/* 产生式 variable   ::=   id variMore                   			*/
/********************************************************************/
TreeNode *Parse::variable() {
    TreeNode *t = newExpNode(VariK);

    if ((t != nullptr) && (head->getLex() == ID)) {
        t->lineno = line0;
        strcpy(t->name[0], head->getSem().toStdString().c_str());
        (t->idnum)++;
    }

    match(ID);
    variMore(t);
    return t;
}


/********************************************************************/
/* 功  能 变量处理函数												*/
/* 产生式 variMore   ::=  ε                             			*/
/*                       | [exp]            {[}                     */
/*                       | . fieldvar       {DOT}                   */
/********************************************************************/
void Parse::variMore(TreeNode *t) {
    switch (head->getLex()
            ) {
        case ASSIGN:
        case TIMES:
        case EQ:
        case LT:
        case PLUS:
        case MINUS:
        case DIVIDE:
        case RPAREN:
        case RMIDPAREN:
        case SEMI:
        case COMMA:
        case THEN:
        case ELSE:
        case FI:
        case DO:
        case ENDWH:
        case END:
            break;
        case LMIDPAREN:
            match(LMIDPAREN);

            /*用来以后求出其表达式的值，送入用于数组下标计算*/
            t->child[0] = mexp();

            t->attr.ExpAttr.varkind = ArrayMembV;

            /*此表达式为数组成员变量类型*/
            t->child[0]->attr.ExpAttr.varkind = IdV;
            match(RMIDPAREN);
            break;
        case DOT:
            match(DOT);
            /*第一个儿子指向域成员变量结点*/
            t->child[0] = fieldvar();

            t->attr.ExpAttr.varkind = FieldMembV;

            t->child[0]->attr.ExpAttr.varkind = IdV;
            break;
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
}
/********************************************************************/
/* 功  能 变量处理函数												*/
/* 产生式 fieldvar   ::=  id  fieldvarMore                          */
/********************************************************************/
TreeNode *Parse::fieldvar() {
    /*注意，可否将此处的IdEK改为一个新的标识，用来记录记录类型的域*/
    TreeNode *t = newExpNode(VariK);

    if ((t != nullptr) && (head->getLex() == ID)) {
        t->lineno = line0;
        strcpy(t->name[0], head->getSem().toStdString().c_str());
        (t->idnum)++;
    }
    match(ID);

    fieldvarMore(t);

    return t;
}

/********************************************************************/
/* 产生式 fieldvarMore   ::=  ε                             		*/
/*                           | [exp]            {[}                 */
/********************************************************************/
void Parse::fieldvarMore(TreeNode *t) {
    switch (head->getLex()) {
        case ASSIGN:
        case TIMES:
        case EQ:
        case LT:
        case PLUS:
        case MINUS:
        case DIVIDE:
        case RPAREN:
        case SEMI:
        case COMMA:
        case THEN:
        case ELSE:
        case FI:
        case DO:
        case ENDWH:
        case END:
            break;
        case LMIDPAREN:
            match(LMIDPAREN);

            /*用来以后求出其表达式的值，送入用于数组下标计算*/
            t->child[0] = mexp();
            t->child[0]->attr.ExpAttr.varkind = ArrayMembV;
            match(RMIDPAREN);
            break;
        default:
            head = head->next;
            syntaxError("unexpected token is here!");
            break;
    }
}
