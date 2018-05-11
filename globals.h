/************************************************************/
/* 文件	 globals.h											*/
/* 说明  类PASCAL语言编译器的全局类型和变量,应在其他包含文件之前	*/
/* 主题  编译器结构:原理和实例								*/	
/************************************************************/

/* 如已经联入头文件globals.h则不再联入 */
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/****** globals.h所包含头文件 ******/
#include "stdio.h"
#include "stdlib.h"

/* 定义常量FALSE为0 */
#ifndef FALSE
#define FALSE 0
#endif

/* 定义常量TRUE为1 */
#ifndef TRUE
#define TRUE 1
#endif


/* 定义保留字数量常量MAXRESERVED为21 */
#define MAXRESERVED 21


/* MAXTOKENLEN为单词最大长度定义为40 */
#define MAXTOKENLEN 40

/*初始化符号表中变量的偏移*/
#define INITOFF 7

/* SCOPESIZE为符号表scope栈的大小*/
#define SCOPESIZE 1000

/*定义token的长度*/
#define TOKENLEN sizeof (TokenType)

/*定义链表节点的长度*/
#define CHAINNODELEN sizeof (ChainNodeType) 


/********************** 单词的词法类型 ********************/
typedef enum 
{
	/* 簿记单词符号 */
    ENDFILE,	ERROR,
	/* 保留字 */
    PROGRAM,	PROCEDURE,	TYPE,	VAR,		IF,
	THEN,		ELSE,		FI,		WHILE,		DO,
	ENDWH,		BEGIN,		END,	READ,		WRITE,
	ARRAY,		OF,			RECORD,	RETURN, 

	INTEGER,	CHAR,
	/* 多字符单词符号 */
    ID,			INTC,		CHARC,
    /*特殊符号 */
	ASSIGN,		EQ,			LT,		PLUS,		MINUS,
	TIMES,		OVER,		LPAREN,	RPAREN,		DOT,
	COLON,		SEMI,		COMMA,	LMIDPAREN,	RMIDPAREN,
	UNDERANGE
} LexType;


/*********** 单词的类型，包括词法信息和语义信息 ************/
typedef struct tokenType
    { int     lineshow;
	  LexType Lex;
      char    Sem[MAXTOKENLEN+1];
    } TokenType; 


/***********链表的结点类型，包括Token部分和指针部分**********/
typedef struct  node  
    { TokenType   Token;      //单词
      struct node *nextToken; //指向下一个单词的指针
    } ChainNodeType;  




/******************************************************
 ******************   语法分析树   ********************
 ******************************************************/

/*语法树根节点ProK,程序头结点PheadK，声明类型节点DecK,
  标志子结点都是类型声明的结点TypeK,标志子结点都是变量声明的结点VarK,
  函数声明结点FuncDecK,语句序列节点StmLK,语句声明结点StmtK,
  表达式结点ExpK*/
typedef enum {ProK,PheadK,DecK,TypeK,VarK,ProcDecK,StmLK,StmtK,ExpK}
              NodeKind;


/*声明类型Deckind 类型的枚举定义：
  数组类型ArrayK,字符类型CharK,
  整数类型IntegerK,记录类型RecordK,
  以类型标识符作为类型的IdK*/ 
typedef enum {ArrayK,CharK,IntegerK,RecordK,IdK}  DecKind;



/* 语句类型StmtKind类型的枚举定义:			*
 * 判断类型IfK,循环类型WhileK				*
 * 赋值类型AssignK,读类型ReadK              *
 * 写类型WriteK，函数调用类型CallK          */
typedef enum {IfK,WhileK,AssignK,ReadK,WriteK,CallK,ReturnK} StmtKind;


/* 表达式类型ExpKind类型的枚举定义:         *
 * 操作类型OpK,常数类型ConstK,变量类型VarK */
typedef enum {OpK,ConstK,VariK} ExpKind;


/* 变量类型VarKind类型的枚举定义:           *
 * 标识符IdV,数组成员ArrayMembV,域成员FieldMembV*/
typedef enum {IdV,ArrayMembV,FieldMembV} VarKind; 


/* 类型检查ExpType类型的枚举定义:           *
 * 空Void,整数类型Integer,字符类型Char      */ 
typedef enum {Void,Integer,Boolean} ExpType;

/* 参数类型ParamType类型的枚举定义：        *
 * 值参valparamType,变参varparamType        */
typedef enum {valparamType,varparamType} ParamType; 

/* 定义语法树节点的最大子节点数MAXCHILDRREN为3 */
/* 过程声明部分的子节点child[0]指向参数部分，
   子节点child[1]指向声明体部分，子节点child[2]
   指向函数的语句部分；*/ 
#define MAXCHILDREN 3 

/*提前声明符号表结构*/
struct symbtable;

/********** 语法树节点treeNode类型 *********/
typedef struct treeNode

   { 
	 struct treeNode * child[MAXCHILDREN];		/* 子节点指针	*/			
     struct treeNode * sibling;					/* 兄弟节点指针	*/
     int lineno;								/* 源代码行号	*/
     NodeKind nodekind;						    /* 节点类型		*/
     union 
	 {
		 DecKind  dec;
	     StmtKind stmt; 
		 ExpKind  exp; 
	 } kind;                       /* 具体类型     */

	 int idnum;                    /* 相同类型的变量个数 */ 
	 
	 char name[10][10];            /* 标识符的名称  */

	 struct symbtable * table[10]; /* 与标志符对应的符号表地址，在语义分析阶段填入*/  

	 struct 
	 {
		struct
			{	
				int low;              /* 数组下界     */
				int up;               /* 数组上界     */
				DecKind   childtype;  /* 数组的子类型 */
			}ArrayAttr;               /* 数组属性     */
	         
		struct
			{	
				ParamType  paramt;     /* 过程的参数类型*/
			}ProcAttr;                 /* 过程属性      */ 
		
		struct
			{
				LexType op;           /* 表达式的操作符*/						
				int val;		      /* 表达式的值	   */ 
				VarKind  varkind;     /* 变量的类别    */
				ExpType type;         /* 用于类型检查  */
			}ExpAttr;	              /* 表达式属性    */

      char type_name[10];             /* 类型名是标识符  */      
             
	 } attr;                          /* 属性	       */
}TreeNode;

/*非终极符的总数*/
#define  NTMLNUM    68

/*终极符的总数*/
#define  TMLNUM     42

/*LL1分析表的大小*/
#define  TABLESIZE  104

/******************类型和变量声明*********************/

/* 1.进行LL1语法分析用到的类型及对应的变量  */

/*所有非终极符，其各自含义可参考LL1文法*/
typedef enum
{ 
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
  
}  NontmlType; 

/*所有终极符，取自单词的词法类型的枚举定义，可参考zglobals.h头文件*/
typedef LexType  TmlType;


/*实现LL1分析用的分析栈，存放的是终极符和非终极符 */
typedef struct Node
{  /*内容标志*/
   int flag;    /* flag为1，表示栈中内容为非终极符；*/
                /* flag为2，表示栈中内容为终极符    */
   /*内容*/
   union { NontmlType Ntmlvar ;  /*非终极符部分*/ 
           TmlType tmlvar;       /*终极符部分*/
         } var;   

   /*指向下一个节点的指针*/
   struct Node *underNode;

}  StackNode;


/* 2.创建语法树所需的类型及变量**/

/*为建立声明和语句部分的语法树所设的指针栈，*
 *注意栈中存的是存放指针的地址              */
typedef struct NodePA
{  TreeNode       **pointer;
   struct NodePA  *underNode;
}  StackNodePA;


/*为生成表达式部分的语法树所设的指针栈，*
 *栈中存的是存放指向树节点的指针        */
typedef struct NodeP
{  TreeNode      *pointer;
   struct NodeP  *underNode;
}StackNodeP;

/*符号栈顶指针*/
extern StackNode *StackTop;

/*栈空标志*/
extern  int STACKEMPTY; 

/*语法树栈顶指针*/
extern  StackNodePA  *StackTopPA;
/*栈空标志*/
extern  int  paSTACKEMPTY;

/*操作符栈的栈顶指针*/
extern  StackNodeP  *OpStackTop;
/*操作符栈空标志*/
extern  int OpSTACKEMPTY ;  

/*操作数栈的栈顶指针*/
extern  StackNodeP  *NumStackTop;

/*操作数栈空标志*/
extern  int NumSTACKEMPTY ;



/******************************************************
 **********语义分析需要用到的类型及变量定义************
 ******************************************************/


/*标识符的类型*/
typedef  enum    {typeKind,varKind,procKind}IdKind;

/*变量的类别。dir表直接变量(值参)，indir表示间接变量(变参)*/						
typedef  enum    {dir,indir}AccessKind;

/*形参表的结构定义*/
typedef struct  paramTable
{ 
	struct symbtable   *  entry;/*指向该形参所在符号表中的地址入口*/
	struct paramTable  *  next;
}ParamTable;


struct typeIR;
/*标识符的属性结构定义*/
typedef struct 
{
	struct typeIR  * idtype;		/*指向标识符的类型内部表示*/
	IdKind    kind;					/*标识符的类型*/
	union   
	{
		struct
		{
			AccessKind   access;   /*判断是变参还是值参*/
			int          level;    
			int          off;
			bool         isParam;  /*判断是参数还是普通变量*/

		}VarAttr;/*变量标识符的属性*/	
		struct
		{
			int         level;     /*该过程的层数*/

			ParamTable  * param;   /*参数表*/

			int         mOff;	   /*过程活动记录的大小*/
			                
			int         nOff;  	   /*sp到display表的偏移量*/
							
			int         procEntry; /*过程的入口地址*/

			int         codeEntry ;/*过程入口标号,用于中间代码生成*/     
			               
		}ProcAttr;/*过程名标识符的属性*/
	
	}More;/*标识符的不同类型有不同的属性*/

}AttributeIR;
                        

/*符号表的结构定义*/
typedef struct  symbtable
{
	char  idName[10];
	AttributeIR  attrIR;
	struct symbtable  *  next;

}SymbTable;
	
/*使用scope栈的局部符号表方法中所用到的scope栈*/
extern SymbTable * scope[1000];

/*scope栈的层数*/
extern int Level;

/*在同层的变量偏移*/
extern int Off;

/*记录主程序的displayOff*/
extern int mainOff;

/*记录当前层的displayOff*/
extern int savedOff;




/******************************************************
 *****************   类型内部表示    ******************
 ******************************************************/

/*类型的枚举定义*/
typedef  enum {intTy,charTy,arrayTy,recordTy,boolTy}TypeKind;


struct typeIR;

/*域类型单元结构定义*/
typedef struct fieldchain
{
	char   id[10];              /*变量名*/
	int    off;                 /*所在记录中的偏移*/
	struct typeIR  *  UnitType; /*域中成员的类型*/
	struct fieldchain  * Next;
}fieldChain;


/*类型的内部结构定义*/
typedef   struct  typeIR
{	
	int				size;   /*类型所占空间大小*/
	TypeKind		kind;
	union
	{   struct
		{	struct typeIR	* indexTy;
			struct typeIR	* elemTy;
			int    low;     /*记录数组类型的下界*/
			int    up;      /*记录数组类型的上界*/
		}ArrayAttr;
		fieldChain * body;  /*记录类型中的域链*/
	} More;
}TypeIR;



/**************************************************************/
/******下面是中间代码生成部分需要用到的类型及变量定义**********/
/**************************************************************/

/*ARG结构分为：标号，数值，地址三种*/
typedef  enum { LabelForm , ValueForm , AddrForm }  ArgForm ;

/*定义ARG结构*/
typedef struct    
{  ArgForm   form ;
   union 
    {   int  value ;  /*纪录整数值*/
    	int  label ;  /*纪录标号的值*/
        struct  
		{   char    name[10] ;	/*注：变量名字已经没用，这里保留只是为了显示结果清晰*/
            int     dataLevel ;
		    int     dataOff ;
		    AccessKind  access ; /*类型AccessKind在前面定义*/
		}addr;   /*变量的ARG结构需要纪录的信息*/
   }Attr ;
} ArgRecord;

/*中间代码的类别*/
typedef  enum 
{ ADD , SUB , MULT ,DIV , EQC ,LTC,  
  READC, WRITEC ,RETURNC ,ASSIG,AADD ,LABEL,
  JUMP0, JUMP ,CALL, VARACT ,VALACT,
  PENTRY , ENDPROC, MENTRY,WHILESTART,ENDWHILE
}  CodeKind ;

/*中间代码的结构*/
typedef struct 
{    CodeKind   codekind;
     ArgRecord  *arg1;  /*以后考虑这里是用指针好，还是直接用
						  ArgRecord类型的变量好*/
     ArgRecord  *arg2;
     ArgRecord  *arg3;
} CodeR;

/*源程序对应的中间代码序列表示*/
typedef  struct  codeFile
{   CodeR  codeR;
	struct codeFile  *former;
    struct codeFile  *next ;
} CodeFile ;



/**************************************************************/
/******下面是常表达式优化需要用到的类型及变量定义**************/
/**************************************************************/

/*常量定值表，用于常表达式优化*/
typedef  struct  constDefT
{ 
    ArgRecord  *variable;  /*用变量的ARG结构表示变量*/
    int  constValue;       /*定值*/
	struct constDefT  *former;
    struct constDefT  *next;

} ConstDefT;


/**************************************************************/
/******下面是公共表达式优化需要用到的类型及变量定义************/
/**************************************************************/

/*值编码表ValuNum*/
typedef  struct  valuNum
{
	ArgRecord  *arg ;
    AccessKind  access;
	union
	{   int  valueCode ;   /*直接变量，存储值编码*/
	  struct 
	  { int   valuecode ;
		int   addrcode  ;
	  } twoCode;         /*间接临时变量，存储值编码和地址码*/ 

	} codeInfo;
   /*指向下一个节点指针*/
   struct valuNum  *next;

} ValuNum;

/*中间代码对应的映象码结构*/
typedef struct
{  int  op1;
   int  op2;
   int  result;
} MirrorCode;

/*可用表达式代码表UsableExpr*/
typedef  struct  usableExpr
{ 
  CodeFile   * code ;		  /*中间代码地址*/
  MirrorCode *  mirrorC;      /*映象码*/
  struct usableExpr  *next ;  /*指向下一个节点*/
} UsableExpr;


/*临时变量的等价表TempEqua*/
typedef  struct tempEqua
{
  ArgRecord  *arg1; /*被替换的临时变量*/
  ArgRecord  *arg2; /*用于替换的临时变量*/
  struct  tempEqua  *next;
} TempEqua ;


/**************************************************************/
/******下面是循环不变式优化需要用到的类型及变量定义************/
/**************************************************************/


/*循环信息表*/
typedef  struct
{
	int       state;        /*循环状况，为0时表示本层循环不可外提*/
	CodeFile  *whileEntry;  /*指向循环入口中间代码*/
    int       varDef;       /*指向本层循环的变量地址表起始处*/
	CodeFile  *whileEnd;    /*指向循环出口中间代码*/
} LoopInfo;
    
/*循环信息栈*/
typedef  struct loopStack
{ 
	LoopInfo  *loopInfo;
    struct loopStack  *under;

} LoopStack;




/**************************************************************/
/******下面是目标代码生成部分需要用到的类型及变量定义**********/
/**************************************************************/

/*标号地址表*/
typedef  struct  labelAddr
{
	int  label;
	int  destNum;
    struct labelAddr  *next;
} LabelAddr;

/*处理回填地址要用到的数据结构*/
typedef struct  backAddr
{  int  backLoc;
   struct  backAddr  *former;
} BackAddr;

extern  BackAddr  *AddrTop;

extern  int AddrEMPTY;



/********************************************************************/
/* 源代码文本文件指针source */
extern FILE *source;

/* 列表输出文件指针listing */
extern FILE* listing; 

/*词法分析结果Token序列的存储文件指针fp*/
extern FILE* fp;

/*目标代码文件指针*/
extern FILE  *code;

/*Token序列中的token数目*/
extern int Tokennum;

/*清单的行号*/
extern int lineno;

/*scope栈的层数*/
extern int Level;

/*在同层的变量偏移*/
extern int Off;

/*记录主程序的displayOff*/
extern int mainOff;

/*记录当前层的displayOff*/
extern int savedOff;

/******************************/
/*生成中间代码部分时用到的变量*/
/******************************/
/*临时变量编号*/
extern  int  TempOffset;
/*标号值*/
extern  int  Label ;

/*第一条中间代码*/
extern  CodeFile  *firstCode ;
/*最后一条中间代码*/
extern  CodeFile  *lastCode ;



extern  CodeFile  *baseBlock[100];

extern  int  blocknum ;

extern  ValuNum  *valuNumT;

extern  UsableExpr  *usableExprT;

extern  TempEqua  *tempEquaT;

/*循环信息栈*/
extern  LoopStack  *loopTop;
extern  bool  loopStackEmpty;



/*保存主程序的display表的偏移*/
extern  int StoreNoff ;



/*************************************
 ***********   追踪标志   ************
 *************************************/

/* 源程序追踪标志,如果该标志为TRUE			 *
 * 语法分析时将源程序行号写入列表文件listing */
extern int EchoSource;

/* 词法扫描追踪标志,如果该标志为TRUE				   *
 * 将每个被词法扫描器识别的单词信息写入列表文件listing */
extern int TraceScan;

/* 语法分析追踪标志,如果该标志为TRUE				     *
 * 将语法树以线性形式(使用子节点缩进)写入列表文件listing */
extern int TraceParse;

/* 语义分析追踪标志,如果该标志为TRUE	 *
 * 将符号表插入和查找写入列表文件listing */
extern int TraceTable;


extern int TraceCode;

/* 错误追踪标志,如果该标志为TRUE *
 * 防止错误产生时进一步传递错误  */
extern int Error;

#endif
	
