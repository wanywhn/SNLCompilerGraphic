#include "parseitem.h"
#include "parsescene.h"

#include <QGraphicsLinearLayout>
#include <QQueue>

ParseScene::ParseScene()
{

}

void ParseScene::show_parsetree(TreeNode *root)
{

    if(root->nodekind!=ProK){
        //TODO ERROR
    }
    QQueue<TreeNode  *> queue;
    QQueue<ParseItem*> iqueue;

    auto item=new ParseItem(getName(root));
    this->addItem(item);

    auto parent=item;
    QGraphicsLineItem *line;
    for(auto i=0;i!=3;++i){
        if(root->child[i]!=nullptr){
            item=new ParseItem(getName(root->child[i]),parent);
            this->addItem(item);
            item->setPos(item->myparent->scenePos()+QPoint(-80+i*80,80));

            line=new QGraphicsLineItem(QLine(item->myparent->scenePos().toPoint(),item->scenePos().toPoint()));
            this->addItem(line);
            line->setPos(item->myparent->scenePos());
            queue.enqueue(root->child[i]);
            iqueue.enqueue(item);
        }
    }
    while (!queue.empty()) {
        auto node=queue.dequeue();
        auto item=iqueue.dequeue();

    auto parent=item;
        for(auto i=0;i!=3;++i){
            if(node->child[i]!=nullptr){
                item=new ParseItem(getName(node->child[i]),parent);
                this->addItem(item);
                item->setPos(item->myparent->scenePos()+QPoint(-80+i*80,80));

                line=new QGraphicsLineItem(QLine(item->myparent->scenePos().toPoint(),item->scenePos().toPoint()));
                this->addItem(line);
                line->setPos(item->myparent->scenePos());

                queue.enqueue(node->child[i]);
                iqueue.enqueue(item);

            }
        }

    }

}

QString ParseScene::getName(TreeNode *node)
{
    switch (node->nodekind) {
    case ProK:
        return "Prok";
    case PheadK:
        return "PheadK";
    case TypeK:
        return "TypeK";
    case VarK:
        return "VarK";
    case ProcDecK:
        return "ProcDeck";
    case StmLK:
        return "StmLK";
    case DecK:
    {
        switch (node->kind.dec) {
        case ArrayK:
            return "ArrayK";
        case CharK:
            return "CharK";
        case IntegerK:
            return "IntegerK";
        case RecordK:
            return "RecordK";
        case IdK:
            return "IdK";
        }
        break;
    }
    case StmtK:
    {
        switch (node->kind.stmt) {
        case IfK:
            return "IfK";
        case WhileK:
            return "WHileK";
        case AssignK:
            return "AssignK";
        case ReadK:
            return "ReadK";
        case WriteK:
            return "WriteK";
        case CallK:
            return "CallK";
        case ReturnK:
            return "ReturnK";
        }
        break;
    case ExpK:
        {
            switch (node->kind.exp) {
            case OpK:
                return "OpK";
            case ConstK:
                return "ConstK";
            case VariK:
                return "IdK";
            }
            break;
        }
    default:
        break;
    }





    }

}
