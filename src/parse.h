#ifndef PARSE_H
#define PARSE_H

#include "globals.h"

#include <QThread>


class Parse : public QThread {
Q_OBJECT

public:
    static Parse *getInstance(Token *head) {
        static auto *tmp = new Parse();
        tmp->set_token_head(head);
        return tmp;
    }

    void set_token_head(Token *head);

private:
    Parse();

    TreeNode *program();

    // QThread interface
protected:
    void run() override;

private:
    Token *head;
    TreeNode *root;
    int line0;
    QString temp_name;

    void syntaxError(QString msg);

    void match(LexType expected);

    TreeNode *programHead();


    TreeNode *declarePart();


    TreeNode *typeDec();

    TreeNode *typeDeclaration();

    TreeNode *typeDecList();

    void typeId(TreeNode *pNode);

    void typeName(TreeNode *pNode);

    void baseType(TreeNode *pNode);

    void structureType(TreeNode *pNode);

    void arrayType(TreeNode *pNode);

    void recType(TreeNode *pNode);

    TreeNode *fieldDecList();

    void idList(TreeNode *pNode);

    void idMore(TreeNode *pNode);

    TreeNode *fieldDecMore();

    TreeNode *typeDecMore();

    TreeNode *varDec();

    TreeNode *varDeclaration();

    TreeNode *varDecList();

    TreeNode *varDecMore();

    void varIdList(TreeNode *t);

    void varIdMore(TreeNode *t);

    TreeNode *programBody();

    TreeNode *stmList();

    TreeNode *stmMore();

    TreeNode *stm();

    TreeNode *assCall();

    TreeNode *assignmentRest();

    TreeNode *conditionalStm();

    TreeNode *loopStm();

    TreeNode *inputStm();

    TreeNode *outputStm();

    TreeNode *returnStm();

    TreeNode *callStmRest();

    TreeNode *actParamList();

    TreeNode *actParamMore();

    TreeNode *mexp();

    TreeNode *simple_exp();

    TreeNode *term();

    TreeNode *factor();

    TreeNode *variable();

    void variMore(TreeNode *t);

    void fieldvarMore(TreeNode *t);

    TreeNode *fieldvar();

    TreeNode *mparam();

    TreeNode *paramMore();

    void paramList(TreeNode *t);

    TreeNode *procDec();

    void formList(TreeNode *t);

    void fidMore(TreeNode *t);

    TreeNode *procDeclaration();

    TreeNode *procBody();

    TreeNode *procDecPart();

    TreeNode *paramDecList();

};

#endif // PARSE_H
