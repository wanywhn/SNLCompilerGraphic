#ifndef LL1_PARSE_H
#define LL1_PARSE_H

#include "globals.h"

#include <QObject>
#include <QStack>
#include <QThread>


/**
  * feizhongjiefu
  **/

extern QSet<LexType> NTSet;
extern QSet<LexType> TTSet;

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
