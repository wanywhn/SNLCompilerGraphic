#include "lex.h"

#include <QDebug>
#include <QFile>
#include <QThread>

#define sleep_time 100

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
                                  {INTEGER, "INTEGER"},
                                  {CHAR, "CHAR"},
                                  {INTEGER_T, "INTEGER_T"},
                                  {CHAR_T, "CHAR_T"},
                                  {ID, "ID"},
                                  {INTC_VAL, "INTC_VAL"},
                                  {CHAR_VAL, "CHAR_VAL"},
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
    {"fi", FI},           {"char", CHAR},
    {"while", WHILE},     {"do", DO},
    {"endwh", ENDWH},     {"read", READ},
    {"write", WRITE},     {"return", RETURN},
    {"integer", INTEGER},
};
void Lex::setFileName(QString filename) {
  this->filename=filename;
}

Token *Lex::getTokenList() {
  Token *tmp = head;
  head = head->next;
  delete tmp;
  return head;
}


bool Lex::ischar(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lex::isnum(char c) { return (c >= (0 + '0') && c <= (9 + '0')); }

bool Lex::issinglesep(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' ||
         c == ';' || c == '[' || c == ']' || c == '=' || c == '<' || c == ',';
}

Token *Lex::getsinglesep(char c) {
  QMap<char, LexType> map = {{'+', PLUS},   {'-', MINUS},     {'*', TIMES},
                             {'/', DIVIDE}, {'(', LPAREN},    {')', RPAREN},
                             {';', SEMI},   {'[', LMIDPAREN}, {']', RMIDPAREN},
                             {'=', EQ},     {'<', LT},        {',', COMMA}};
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
    auto file = new QFile(filename);
    file->open(QIODevice::ReadOnly);
    ins.setDevice(file);

    head = new Token(0, ERROR, "");
    current = head;
    char lookhead;
    while (!ins.atEnd()) {
      ins >> lookhead;
      emit charget(lookhead);
      thread()->msleep(sleep_time);

      if (ischar(lookhead)) {
        idBuff.append(lookhead);
        emit idbuff_changed(idBuff);
        thread()->msleep(sleep_time);

        ins >> lookhead;
        emit charget(lookhead);
        thread()->msleep(sleep_time);
        while (ischar(lookhead) || isnum(lookhead)) {
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
        thread()->msleep(sleep_time);
      } else if (isnum(lookhead)) {
        idBuff.append(lookhead);
        emit idbuff_changed(idBuff);

        ins >> lookhead;
        emit charget(lookhead);

        while (isdigit(lookhead)) {
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
      } else if (issinglesep(lookhead)) {
          emit idbuff_changed(QString(lookhead));
        Token *tmp = getsinglesep(lookhead);
        emit token_get(tmp);
        current->next = tmp;
        current = tmp;
      } else {
        switch (lookhead) {

        case '\n': {
          ++line_number;
          continue;
        }
        case ':': {
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
          } else {
            Token *tmp = new Token(line_number, ASSIGN, lexName[ASSIGN]);
            emit token_get(tmp);
            current->next = tmp;
            current = tmp;
          }
          idBuff.clear();


          break;
        }
        case '{': {
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          while (lookhead != '}') {
            ins >> lookhead;
            emit charget(lookhead);
      thread()->msleep(sleep_time);
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          }
          idBuff.clear();
          break;
        }
        case '.': {
            idBuff.append(lookhead);
          char next;
          ins >> next;
          emit charget(next);
            idBuff.append(lookhead);
          Token *tmp;
          if (next == '.') {
            tmp = new Token(line_number, UNDERRANGE, lexName[UNDERRANGE]);
          } else {
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
            idBuff.append(lookhead);
            emit idbuff_changed(idBuff);
          char next;
          ins >> next;
          emit charget(next);

      thread()->msleep(sleep_time);
          if (isnum(next) || ischar(next)) {
            idBuff.append(next);
            emit idbuff_changed(idBuff);
            ins >> next;
            idBuff.append(lookhead);
            if (next == '\'') {
              Token *tmp = new Token(line_number, CHAR_VAL, idBuff);
              current->next = tmp;
              current = tmp;
              emit token_get(tmp);
            } else {
              ins.seek(ins.pos() - 2);
            }
          } else {
            ins.seek(ins.pos() - 1);
            qDebug() << "following ' isn't a char or dig";
          }
          idBuff.clear();
          break;
        }
        default: {}
        }
      }
    }
}
