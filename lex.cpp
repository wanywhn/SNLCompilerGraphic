#include "lex.h"

QMap<LexType,QString> lexName={
    {PROGRAM, "PROGRAM"},   {TYPE, "TYPE"}, {VAR, "VAR"},       {PROCEDURE, "PROCEDURE"},
    {BEGIN, "BEGIN"},       {END, "END"},   {ARRAY, "ARRAY"},   {OF, "OF"},
    {RECORD, "RECORD"},     {IF, "IF"},     {THEN, "THEN"},     {ELSE, "ELSE"},
    {FI, "FI"},             {WHILE, "WHILE"},{DO, "DO"},        {ENDWH, "ENDWH"},
    {READ, "READ"},         {WRITE, "WRITE"},{RETURN, "RETURN"},{INTEGER, "INTEGER"},
    {CHAR, "CHAR"}, {INTEGER_T, "INTEGER_T"},{CHAR_T, "CHAR_T"},{ID, "ID"},
    {INTC_VAL, "INTC_VAL"},{CHAR_VAL, "CHAR_VAL"},{ASSIGN, ":="},{EQ, "="},
    {LT, "<"},     {PLUS, "+"},         {MINUS, "-"},   {TIMES, "*"},
    {DIVIDE, "/"}, {LPAREN, "("}, {RPAREN, ")"}, {DOT, "."},
    {COLON, ":"},   {SEMI, ";"},     {COMMA, ","},   {LMIDPAREN, "["},
    {RMIDPAREN, "]"},   {UNDERRANGE, ".."},       {ENDFILE, "EOF"},
    {ERROR, "ERROR"}
};
Lex::Lex(QObject *parent) : QObject(parent)
{

}
