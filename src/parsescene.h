#ifndef PARSESCENE_H
#define PARSESCENE_H

#include <QGraphicsScene>
#include "globals.h"



class ParseScene : public QGraphicsScene
{
public:
    ParseScene();
    void show_parsetree(TreeNode *root);
};

#endif // PARSESCENE_H
