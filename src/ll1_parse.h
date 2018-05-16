#ifndef LL1_PARSE_H
#define LL1_PARSE_H

#include "globals.h"

#include <QObject>
#include <QStack>
#include <QThread>


/**
  * feizhongjiefu
  **/
QSet<LexType> NTSet={
    Program,	      ProgramHead,	    ProgramName,	DeclarePart,
    TypeDec,        TypeDeclaration,	TypeDecList,	TypeDecMore,
    TypeId,	      TypeName,			BaseType,	    StructureType,
    ArrayType,      Low,	            Top,            RecType,
    FieldDecList,   FieldDecMore,	    IdList,	        IdMore,
    VarDec,	      VarDeclaration,	VarDecList,		VarDecMore,
    VarIdList,	  VarIdMore,		ProcDec,		ProcDeclaration,
    ProcDecMore,    ProcName,		    ParamList,		ParamDecList,
    ParamMore,      Param,		    FormList,		FidMore,
    ProcDecPart,    ProcBody,	    	ProgramBody,	StmList,
    StmMore,        Stm,				AssCall,		AssignmentRest,
    ConditionalStm, StmL,			    LoopStm,		InputStm,
    InVar,          OutputStm,		ReturnStm,		CallStmRest,
    ActParamList,   ActParamMore,		RelExp,			OtherRelE,
    Exp,			  OtherTerm,		Term,           OtherFactor,
    Factor,         Variable,			VariMore,		FieldVar,
    FieldVarMore,   CmpOp,			AddOp,          MultOp
  };

QSet<LexType> TTSet={
    /* 簿记单词符号 */
    ENDFILE,	ERROR,
    /* 保留字 */
    PROGRAM,	PROCEDURE,	TYPE,	VAR,		IF,
    THEN,		ELSE,		FI,		WHILE,		DO,
    ENDWH,		BEGIN,		END,	READ,		WRITE,
    ARRAY,		OF,			RECORD,	RETURN,

    INTEGER_T,	CHAR_T,
    /* 多字符单词符号 */
    ID,			INTC_VAL,		CHARC_VAL,
    /*特殊符号 */
    ASSIGN,		EQ,			LT,		PLUS,		MINUS,
    TIMES,DIVIDE,		LPAREN,	RPAREN,		DOT,
    COLON,		SEMI,		COMMA,	LMIDPAREN,	RMIDPAREN,
    UNDERRANGE,
};

/**
 * @brief lexName zhongjiefu
 */
extern int lineno;
extern QMap<LexType, QString> lexName ;
class LL1_parse:public QThread
{
    Q_OBJECT
public:
    LL1_parse();

    static LL1_parse *getInstance(Token *head){
        static LL1_parse *instance=new LL1_parse();
        instance->head=head;
        return instance;
    }
    // QThread interface
protected:
    void run() override;


private:
    void createLL1Table();
    void process(int id);
private:
    Token *head;
    QMap<QPair<LexType,LexType>,int> table;
    QStack<LexType> parse_stack;
    QStack<TreeNode *> tree_stack;
};

#endif // LL1_PARSE_H
