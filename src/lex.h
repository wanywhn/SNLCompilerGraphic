#ifndef LEX_H
#define LEX_H

#include <QObject>
#include <QTextStream>
#include <QThread>
#include <utility>
#include "globals.h"
#include <QPair>
#include <QVector>

//enum LexType{
//    PROGRAM=1,TYPE,   VAR,    PROCEDURE,
//    BEGIN,    END,    ARRAY,  OF,
//    RECORD,   IF,     THEN,   ELSE,
//    FI,       WHILE,  DO,     ENDWH,
//    READ,     WRITE,  RETURN,INTEGER, CHAR,
//    //types, just a symbol of type **name**, no actual sematic infomation
//    INTEGER_T, CHAR_T,
//    //
//    ID,
//    //not like INTEGER_T and CHAR_T, these two do have certain meanings
//    INTC_VAL, CHAR_VAL,
//    //
//    ASSIGN, EQ,     LT,     PLUS, MINUS,
//    TIMES,  DIVIDE, LPAREN, RPAREN,
//    DOT,    COLON,  SEMI,   COMMA,
//    LMIDPAREN, RMIDPAREN, UNDERRANGE,
//    //
//    ENDFILE = 0, ERROR = -1,
//
//};

extern QMap<LexType,QString> lexName;
extern QMap<QString,LexType> reservedWords;

class Lex : public QThread
{
    Q_OBJECT
public:
    static Lex * getInstance(QString filename){
        static auto *lex=new Lex();
        lex->setFileName(std::move(filename));
        lex->set_speed(300);
        return lex;

    }
    void setFileName(QString filename);
    Token * getTokenList();
    void set_speed(int speed);



private:
//    explicit Lex(QObject *parent = nullptr);

signals:
    void charget(char c);
    void idbuff_changed(QString buff);
    void token_get(Token *token);
    void go_path(QVector<QPair<int,int>> v);


public slots:


private:
    QTextStream ins;
    double intBuff;
    QString idBuff;
    Token *current;
    Token *head;
    QString filename;
    int line_number;
    int sleep_time;
private:
    bool ischar(char c);
    bool isnum(char c);
    bool issinglesep(char c);
    Token *getsinglesep(char c);
    Token *lookup(QString str);


    // QThread interface
protected:
    void run() override;
};

#endif // LEX_H
