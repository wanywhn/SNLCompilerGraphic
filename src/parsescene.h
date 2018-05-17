#ifndef PARSESCENE_H
#define PARSESCENE_H

#include <QGraphicsScene>
#include "globals.h"



class ParseScene : public QGraphicsScene
{
public:
    ParseScene();
    void show_parsetree(TreeNode *root);


private:
    QString getName(TreeNode *node);
};

#endif // PARSESCENE_H
