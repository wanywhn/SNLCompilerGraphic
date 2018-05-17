#include "ll1_parse.h"
#include "utils.h"

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

QSet<LexType> NTSet={
    //feizhongjiefu
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
    FieldVarMore,   CmpOp,			AddOp,          MultOp,

};

LL1_parse::LL1_parse() {

    table={
        {{Program,PROGRAM},1},
        {{ProgramHead,PROGRAM},2},
        {{ProgramHead,ID},3},
        {{DeclarePart,VAR},4},
        {{DeclarePart,PROCEDURE},4},
        {{DeclarePart,BEGIN},4},
        {{TypeDec,TYPE},6},
        {{TypeDec,VAR},5},
        {{TypeDec,PROCEDURE},5},
        {{TypeDec,BEGIN},5},
        {{TypeDeclaration,TYPE},7},
        {{TypeDecList,ID},8},
        {{TypeDecMore,VAR},9},
        {{TypeDecMore,PROCEDURE},9},
        {{TypeDecMore,BEGIN},9},
        {{TypeDecMore,ID},10},
        {{TypeId,ID},11},
        {{TypeName,INTEGER_T},12},
        {{TypeName,CHAR_T},12},
        {{TypeName,ARRAY},13},
        {{TypeName,RECORD},13},
        {{TypeName,ID},14},


        {{BaseType,INTEGER_T},15},
        {{BaseType,CHAR_T},16},
        {{StructureType,ARRAY},17},
        {{StructureType,RECORD},18},
        {{ArrayType,ARRAY},19},
        {{Low,INTC_VAL},20},
        {{Top,INTC_VAL},21},
        {{RecType,RECORD},22},
        {{FieldDecList,INTEGER_T},23},
        {{FieldDecList,CHAR_T},23},
        {{FieldDecList,ARRAY},24},
        {{FieldDecMore,INTEGER_T},26},
        {{FieldDecMore,CHAR_T},26},
        {{FieldDecMore,ARRAY},26},
        {{FieldDecMore,END},25},
        {{IdList,ID},27},
        {{IdMore,DOT},28},
        {{IdMore,SEMI},29},
        {{VarDec,VAR},31},
        {{VarDec,PROCEDURE},30},
        {{VarDec,BEGIN},30},
        {{VarDeclaration,VAR},32},
        {{VarDecList,INTEGER_T},33},
        {{VarDecList,CHAR_T},33},
        {{VarDecList,ARRAY},33},
        {{VarDecList,RECORD},33},
        {{VarDecList,ID},33},
        {{VarDecMore,INTEGER_T},35},
        {{VarDecMore,CHAR_T},35},
        {{VarDecMore,ARRAY},35},
        {{VarDecMore,RECORD},35},
        {{VarDecMore,PROCEDURE},34},
        {{VarDecMore,BEGIN},34},
        {{VarDecMore,ID},35},
        {{VarIdList,ID},36},
        {{VarIdMore,SEMI},37},
        {{VarIdMore,COLON},38},
        {{ProcDec,PROCEDURE},40},
        {{ProcDec,BEGIN},39},
        {{ProcDeclaration,PROCEDURE},41},
        {{ProcDecMore,PROCEDURE},41},
        {{ProcDecMore,BEGIN},42},
        {{ProcName,ID},44},






        {{ParamList,INTEGER_T},46},
        {{ParamList,CHAR_T},46},
        {{ParamList,ARRAY},46},
        {{ParamList,RECORD},46},
        {{ParamList,VAR},46},
        {{ParamList,ID},46},
        {{ParamList,RMIDPAREN},45},
        {{ParamDecList,INTEGER_T},47},
        {{ParamDecList,CHAR_T},47},
        {{ParamDecList,ARRAY},47},
        {{ParamDecList,RECORD},47},
        {{ParamDecList,VAR},47},
        {{ParamDecList,ID},47},
        {{ParamMore,SEMI},49},
        {{ParamMore,RMIDPAREN},48},
        {{Param,INTEGER_T},50},
        {{Param,CHAR_T},50},
        {{Param,ARRAY},50},
        {{Param,RECORD},50},
        {{Param,END},51},
        {{Param,ID},50},

        {{Param,VAR},51},
        {{FormList,ID},52},
        {{FidMore,SEMI},53},

        {{FidMore,COLON},54},
        {{FidMore,RMIDPAREN},53},

        {{ProcDecPart,TYPE},55},
        {{ProcDecPart,VAR},55},
        {{ProcDecPart,PROCEDURE},55},

        {{ProcDecPart,BEGIN} , 55},

        {{ProcBody,BEGIN},56},

        {{ProgramBody,BEGIN},57},
        {{StmList,IF},58},
        {{StmList,WHILE},58},

        {{StmList,READ},58},
        {{StmList,WRITE},58},
        {{StmList,RETURN},58},
        {{StmList,ID},58},
        {{StmMore,END},59},

        {{StmMore,ELSE},59},
        {{StmMore,FI},59},
        {{StmMore,ENDWH},59},
        {{StmMore,SEMI},60},

        {{Stm,IF},61},
        {{Stm,WHILE},62},
        {{Stm,READ},63},
        {{Stm,WRITE},64},
        {{Stm,RETURN},65},
        {{Stm,ID},66},






        {{AssCall,DOT},67},
        {{AssCall,LPAREN},68},
        {{AssCall,LMIDPAREN},67},
        {{AssCall,ASSIGN},67},
        {{AssignmentRest,DOT},69},
        {{AssignmentRest,LMIDPAREN},69},
        {{AssignmentRest,ASSIGN},69},
        {{ConditionalStm,IF},70},
        {{LoopStm,WHILE},71},
        {{InputStm,READ},72},
        {{InVar,ID},73},
        {{OutputStm,WRITE},74},
        {{ReturnStm,RETURN},75},
        {{CallStmRest,LMIDPAREN},76},
        {{ActParamList,INTC_VAL},78},
        {{ActParamList,ID},78},
        {{ActParamList,LMIDPAREN},78},
        {{ActParamList,RMIDPAREN},77},
        {{ActParamMore,COMMA},80},
        {{ActParamMore,RMIDPAREN},79},
        {{RelExp,INTC_VAL},81},
        {{RelExp,ID},81},
        {{RelExp,LMIDPAREN},81},
        {{OtherRelE,LT},82},
        {{OtherRelE,EQ},82},

        {{Exp,INTC_VAL},83},
        {{Term,INTC_VAL},86},
        {{Factor,INTC_VAL},90},

        {{OtherTerm,END},84},
        {{Term,END},87},
        {{VariMore,END},93},
        {{FieldVarMore,END},97},

        {{OtherTerm,THEN},84},
        {{Term,THEN},87},
        {{VariMore,THEN},93},
        {{FieldVarMore,THEN},97},

        {{OtherTerm,ELSE},84},
        {{Term,ELSE},87},
        {{VariMore,ELSE},93},
        {{FieldVarMore,ELSE},97},

        {{OtherTerm,FI},84},
        {{Term,FI},87},
        {{VariMore,FI},93},
        {{FieldVarMore,FI},97},

        {{OtherTerm,DO},84},
        {{Term,DO},87},
        {{VariMore,DO},93},
        {{FieldVarMore,DO},97},

        {{OtherTerm,ENDWH},84},
        {{Term,ENDWH},87},
        {{VariMore,ENDWH},93},
        {{FieldVarMore,ENDWH},97},

        {{Exp,ID},83},
        {{Term,ID},86},
        {{Factor,ID},91},
        {{Variable,ID},92},
        {{FieldVar,ID},96},

        {{VariMore,DOT},95},

        {{OtherTerm,SEMI},84},
        {{Term,SEMI},87},
        {{VariMore,SEMI},93},
        {{FieldVarMore,SEMI},97},

        {{OtherTerm,COMMA},84},
        {{Term,COMMA},87},
        {{VariMore,COMMA},93},
        {{FieldVarMore,COMMA},97},

        {{Exp,LPAREN},83},
        {{Term,LPAREN},86},
        {{Factor,LPAREN},89},

        {{OtherTerm,RPAREN},84},
        {{Term,RPAREN},87},
        {{VariMore,RPAREN},93},
        {{FieldVarMore,RPAREN},97},

        {{VariMore,LMIDPAREN},94},
        {{FieldVarMore,LMIDPAREN},98},

        {{OtherTerm,RMIDPAREN},84},
        {{Term,RMIDPAREN},87},
        {{VariMore,RMIDPAREN},93},
        {{FieldVarMore,RMIDPAREN},97},

        {{OtherTerm,LT},84},
        {{Term,LT},87},
        {{VariMore,LT},93},
        {{FieldVarMore,LT},97},
        {{CmpOp,LT},99},

        {{OtherTerm,EQ},84},
        {{Term,EQ},87},
        {{VariMore,EQ},93},
        {{FieldVarMore,EQ},97},
        {{CmpOp,EQ},100},

        {{OtherTerm,PLUS},85},
        {{Term,PLUS},87},
        {{VariMore,PLUS},93},
        {{FieldVarMore,PLUS},97},
        {{AddOp,PLUS},101},

        {{OtherTerm,MINUS},85},
        {{Term,MINUS},87},
        {{VariMore,MINUS},93},
        {{FieldVarMore,MINUS},97},
        {{AddOp,MINUS},102},

        {{Term,TIMES},88},
        {{VariMore,TIMES},93},
        {{FieldVarMore,TIMES},97},
        {{MultOp,TIMES},103},

        {{Term,DIVIDE},88},
        {{VariMore,DIVIDE},93},
        {{FieldVarMore,DIVIDE},97},
        {{MultOp,DIVIDE},104},

        {{VariMore,ASSIGN},93},
        {{FieldVarMore,ASSIGN},97},


    };

}

void LL1_parse::run() {
  parse_stack.push(Program);

  auto root = newSpecNode(ProK);

  tree_stack.push(root->child[2]);
  tree_stack.push(root->child[1]);
  tree_stack.push(root->child[0]);

  lineno=head->getLine();
  while (!parse_stack.empty()) {
      if(NTSet.contains(head->getLex())) {
         auto iter=table.find(QPair<LexType,LexType>(parse_stack.top(),head->getLex())) ;
         if(table.cend()!=iter){
             //auto func=iter.value();
             process(iter.value());
         }else{
             //TODO not in table
         }
      }else if(TTSet.contains(head->getLex())){
          if(parse_stack.top()==head->getLex()){
              parse_stack.pop();
              head=head->next;
              lineno=head->getLine();
          }else{
              //TODO ERROR not match
          }

      }else{
          //TODO ERROR no type
      }
  }
}

void LL1_parse::process(int id)
{

}
