#ifndef LEX_H
#define LEX_H

#include <QObject>
#include <QTextStream>

enum LexType{
    PROGRAM=1,TYPE,   VAR,    PROCEDURE,
    BEGIN,    END,    ARRAY,  OF,
    RECORD,   IF,     THEN,   ELSE,
    FI,       WHILE,  DO,     ENDWH,
    READ,     WRITE,  RETURN,INTEGER, CHAR,
    //types, just a symbol of type **name**, no actual sematic infomation
    INTEGER_T, CHAR_T,
    //
    ID,
    //not like INTEGER_T and CHAR_T, these two do have certain meanings
    INTC_VAL, CHAR_VAL,
    //
    ASSIGN, EQ,     LT,     PLUS, MINUS,
    TIMES,  DIVIDE, LPAREN, RPAREN,
    DOT,    COLON,  SEMI,   COMMA,
    LMIDPAREN, RMIDPAREN, UNDERRANGE,
    //
    ENDFILE = 0, ERROR = -1,

};

extern QMap<LexType,QString> lexName;
extern QMap<QString,LexType> reservedWords;
class Token{
public:
    Token(int line ,LexType lex, QString sem){
        this->line=line;
        this->lex=lex;
        this->sem=sem;
    }
    int getLine(){return line;}
    QString getLexName(){return lexName[lex];}
    LexType getLex(){return lex;}
    QString getSem(){return sem;}

    Token *next;

private:
    int line;
    LexType lex;
    QString sem;
};

class Lex : public QObject
{
    Q_OBJECT
public:
    static Lex * getInstance(){
        static Lex *lex=new Lex;
        return lex;

    }
    void setFileName(QString filename);
    Token * getTokenList();

    private:
    explicit Lex(QObject *parent = nullptr);

signals:


public slots:


private:
    QTextStream ins;
    double intBuff;
    QString idBuff;
    int line_number;



};

#endif // LEX_H
