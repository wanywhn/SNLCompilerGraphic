#include "ll1_parse.h"
#include "utils.h"

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
        {{TypeDef,}},



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
        {{ProcDec,ProcD}},






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
        {{ProcDecPart,VAR},55},
        {{ProcDecPart,PROCEDURE},55},
        {{ProcDecPart,BEGIN}},
        {{ProcDecPart,TYPE},55},
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
