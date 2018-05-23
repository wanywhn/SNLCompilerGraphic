#include "lex.h"

#include <QDebug>
#include <QFile>
#include <QThread>
#include <utility>


QMap<LexType, QString> lexName = {{PROGRAM, "PROGRAM"},
                                  {TYPE, "TYPE"},
                                  {VAR, "VAR"},
                                  {PROCEDURE, "PROCEDURE"},
                                  {BEGIN, "BEGIN"},
                                  {END, "END"},
                                  {ARRAY, "ARRAY"},
                                  {OF, "OF"},
                                  {RECORD, "RECORD"},
                                  {IF, "IF"},
                                  {THEN, "THEN"},
                                  {ELSE, "ELSE"},
                                  {FI, "FI"},
                                  {WHILE, "WHILE"},
                                  {DO, "DO"},
                                  {ENDWH, "ENDWH"},
                                  {READ, "READ"},
                                  {WRITE, "WRITE"},
                                  {RETURN, "RETURN"},
                                  {INTEGER_T, "INTEGER"},
                                  {CHAR_T, "CHAR"},
                                  {INTEGER_T, "INTEGER_T"},
                                  {CHAR_T, "CHAR_T"},
                                  {ID, "ID"},
                                  {INTC_VAL, "INTC_VAL"},
                                  {CHARC_VAL, "CHAR_VAL"},
                                  {ASSIGN, ":="},
                                  {EQ, "="},
                                  {LT, "<"},
                                  {PLUS, "+"},
                                  {MINUS, "-"},
                                  {TIMES, "*"},
                                  {DIVIDE, "/"},
                                  {LPAREN, "("},
                                  {RPAREN, ")"},
                                  {DOT, "."},
                                  {COLON, ":"},
                                  {SEMI, ";"},
                                  {COMMA, ","},
                                  {LMIDPAREN, "["},
                                  {RMIDPAREN, "]"},
                                  {UNDERRANGE, ".."},
                                  {ENDFILE, "EOF"},
                                  {ERROR, "ERROR"}};
QMap<QString, LexType> reservedWords = {
    {"program", PROGRAM}, {"type", TYPE},
    {"var", VAR},         {"procedure", PROCEDURE},
    {"begin", BEGIN},     {"end", END},
    {"array", ARRAY},     {"of", OF},
    {"record", RECORD},   {"if", IF},
    {"then", THEN},       {"else", ELSE},
    {"fi", FI},           {"char", CHAR_T},
    {"while", WHILE},     {"do", DO},
    {"endwh", ENDWH},     {"read", READ},
    {"write", WRITE},     {"return", RETURN},
    {"integer", INTEGER_T},
};
QMap<char, LexType> map = {{'+', PLUS},   {'-', MINUS},     {'*', TIMES},
                             {'/', DIVIDE}, {'(', LPAREN},    {')', RPAREN},
                             {';', SEMI},   {'[', LMIDPAREN}, {']', RMIDPAREN},
                             {'=', EQ},     {'<', LT},        {',', COMMA},
                             {EOF,ENDFILE}};


const Token *Lex::getTokenList() {
    if(ERROR==head->getLex()){
  Token *tmp = head;
  head = head->next;
  delete tmp;
    };
  return head;
}


bool Lex::ischar(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lex::isnum(char c) { return (c >= (0 + '0') && c <= (9 + '0')); }

bool Lex::issinglesep(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' ||
         c == ';' || c == '[' || c == ']' || c == '=' || c == '<' || c == ',' ||c==EOF;
}

Token *Lex::getsinglesep(char c) {
  Token *tmp = new Token(line_number, map[c], lexName[map[c]]);

  return tmp;
}

Token *Lex::lookup(QString str) {
  Token *tmp;
  if (reservedWords.find(str) != reservedWords.end()) {
    tmp = new Token(line_number, reservedWords[str], str);
  } else {
    tmp = new Token(line_number, ID, str);
  }
  return tmp;
}

void Lex::run()
{
    auto str=new QString(doc->toPlainText());
    ins.setString(str);
    ins.seek(0);
    line_number=0;


    head = new Token(0, ERROR, "");
    current = head;
    char lookhead;
    while (!ins.atEnd()) {
      ins >> lookhead;
      emit charget(lookhead);
      thread()->msleep(sleep_time);

      if (ischar(lookhead)) {
          emit go_path({{0,1}});
        idBuff.append(lookhead);
        emit idbuff_changed(idBuff);
        thread()->msleep(sleep_time);

        ins >> lookhead;
        emit charget(lookhead);
        thread()->msleep(sleep_time);
        while (ischar(lookhead) || isnum(lookhead)) {
            emit go_path({{1,4},{4,3},{3,2},{2,1}});
          idBuff.append(lookhead);
          emit idbuff_changed(idBuff);
          thread()->msleep(sleep_time);
          if (ins.atEnd())
            break;
          ins >> lookhead;
          emit charget(lookhead);
          thread()->msleep(sleep_time);
        }
        Token *tmp = lookup(idBuff);
        emit token_get(tmp);
        thread()->msleep(sleep_time);
        current->next = tmp;
        current = tmp;
        ins.seek(ins.pos() - 1);
        idBuff.clear();
        emit idbuff_changed(idBuff);
        emit go_path({{1,5},{5,41}});
        thread()->msleep(sleep_time);
      } else if (isnum(lookhead)) {
          emit go_path({{0,6},{6,7}});
        idBuff.append(lookhead);
        emit idbuff_changed(idBuff);

        ins >> lookhead;
        emit charget(lookhead);

        while (isdigit(lookhead)) {
            emit go_path({{7,10},{10,9},{9,8},{8,7}});
          idBuff.append(lookhead);
        emit idbuff_changed(idBuff);
      thread()->msleep(sleep_time);
          if (ins.atEnd())
            break;
          ins >> lookhead;
        emit charget(lookhead);
        }
        Token *tmp = new Token(line_number, INTC_VAL, idBuff);
        emit token_get(tmp);
        current->next = tmp;
        current = tmp;
        ins.seek(ins.pos() - 1);
        idBuff.clear();
          emit go_path({{7,11},{11,41}});
      } else if (issinglesep(lookhead)) {
          emit go_path({{0,12},{12,13},{13,14},{14,41}});
          emit idbuff_changed(QString(lookhead));
        Token *tmp = getsinglesep(lookhead);
        emit token_get(tmp);
        current->next = tmp;
        current = tmp;
      } else {
        switch (lookhead) {

        case '\n': {
            emit go_path({{0,38},{38,39},{39,40},{40,41}});
          ++line_number;
          continue;
        }
        case ':': {
            emit go_path({{0,15},{15,16}});
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
      thread()->msleep(sleep_time);
          char next;
          ins >> next;
            emit charget(next);
            idBuff.append(next);
            emit idbuff_changed(idBuff);
          if (next != '=') {
            qDebug() << "ERROR,:=";
            ins.seek(ins.pos() - 1);
            emit go_path({{16,17},{17,19},{19,41}});
          } else {
              emit go_path({{16,18},{18,20},{20,41}});
            Token *tmp = new Token(line_number, ASSIGN, lexName[ASSIGN]);
            emit token_get(tmp);
            current->next = tmp;
            current = tmp;
          }
          idBuff.clear();


          break;
        }
        case '{': {
            emit go_path({{0,21},{21,22}});
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          while (lookhead != '}') {
            ins >> lookhead;
            emit charget(lookhead);
      thread()->msleep(sleep_time);
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          }
          emit go_path({{22,23},{23,41}});
          idBuff.clear();
          break;
        }
        case '.': {
            emit go_path({{0,24},{24,25}});
            idBuff.append(lookhead);
          char next;
          ins >> next;
          emit charget(next);
            idBuff.append(lookhead);
          Token *tmp;
          if (next == '.') {
              emit go_path({{25,29},{29,41}});
            tmp = new Token(line_number, UNDERRANGE, lexName[UNDERRANGE]);
          } else {
              emit go_path({{25,26},{26,28},{28,41}});
            tmp = new Token(line_number, DOT, lexName[DOT]);
            ins.seek(ins.pos() - 1);
          }
          idBuff.clear();
          emit token_get(tmp);
          current->next = tmp;
          current = tmp;
          break;
        }
        case '\'': {
            emit go_path({{0,30},{30,31}});
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          char next;
          ins >> next;
          emit charget(next);

      thread()->msleep(sleep_time);
          if (isnum(next) || ischar(next)) {
              emit go_path({{31,33}});
            idBuff.append(next);
            emit idbuff_changed(idBuff);
            ins >> next;
            idBuff.append(lookhead);
            if (next == '\'') {
                emit go_path({{33,35},{35,36},{36,41}});
              Token *tmp = new Token(line_number, CHARC_VAL, idBuff);
              current->next = tmp;
              current = tmp;
              emit token_get(tmp);
            } else {
              ins.seek(ins.pos() - 2);
            }
          } else {
              emit go_path({{31,32},{32,37},{37,41}});
            ins.seek(ins.pos() - 1);
            qDebug() << "following ' isn't a char or dig";
          }
          idBuff.clear();
          break;
        }
        default: {

            if(' '!=lookhead&&32!=lookhead&&9!=lookhead){
            qDebug()<<"unexpected char:"<<lookhead<<" in line:"<<line_number;
            emit go_path({{0,38},{38,40},{40,41}});
            }
        }
        }
      }
    }


        Token *tmp = getsinglesep(EOF);
        current->next = tmp;
        current = tmp;
}

void Lex::set_speed(int speed) {
    sleep_time=speed*3;

}
