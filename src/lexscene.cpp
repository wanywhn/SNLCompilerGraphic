#include "lexscene.h"

#include <QDebug>

LexScene::LexScene() {
    r=20;
    pos = {
            {-100, -200 },  {-50,  -200 },{-50,-230},   {0,-230},{0,-200},              {50,-200},
            {-100,-150},    {-50,  -150 },{-50,-180},   {0,-180},{0,-150},              {50,-150},
            {-100,-100},                                {0,   -100 },                   {50,-100},
            {-100,-50},     {-50,  -50 },{-50,-80},     {0,-50},                        {50,-80},{50,-50},
            {-100,0},       {-50,  0},                                                  {50,  0   },
            {-100,50},      {-50,50},{-50,  20  },      {0,50},                  {50,20},{50,50},
            {-100,100},     {-50,100},{-50,70},         {0,100},{0,70},     {30,100},   {50,100},{50,70},
            {-100,150},     {-50,150},{50,150},
            {50,200},
    };
    line={
            {0,1}, {1,2}, {2,3}, {3,4}, {1,4}, {4,5},
            {0,6},{6,7},{7,8},{8,9},{9,10},{7,10},{10,11},{5,11},
            {6,12},{12,13},{13,14},{11,14},
        {12,15},{15,16},{16,17},{17,19},{16,18},{19,20},{18,20},{14,19},
            {15,21},{21,22},{22,23},{20,23},
            {21,24},{24,25},{25,26},{26,28},{23,28},{28,29},{25,27},{27,29},
        {24,30},{30,31},{31,32},{32,34},{31,33},{34,33},{34,37},{33,35},{35,36},{37,36},{29,37},
            {30,38},{38,39},{39,40},{36,40},
            {40,41}
    };
    round=QSet<int>::fromList({0,1,7,13,16,18,22,25,27,31,33,35,39,41});

    for(int i=0;i!=pos.length();++i){
        if(round.contains(i)){
            auto el=addEllipse(-r/2,-r/2,r,r);
            el->setPos(pos.at(i));
            items.append(el);
        }else{
            auto p=addLine(0,0,0,0);
            p->setPos(pos.at(i));
            items.append(p);
        }
    }
    for(auto item:line){
        auto x=items.at(item.x());
        auto y=items.at(item.y());
        auto l=addLine(0,0,y->x()-x->x(),y->y()-x->y());
        l->setPos(x->scenePos());
    }
}

void LexScene::show_path(QVector<QPair<int, int> > v)
{
    for(auto item:path_item){
        removeItem(item);
    }
    path_item.clear();
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
//    qDebug()<<v;
    for(auto item:v){
        auto x=items.at(item.first);
        auto y=items.at(item.second);
        auto l=addLine(0,0,y->x()-x->x(),y->y()-x->y(),pen);
        l->setPos(x->scenePos());
        path_item.append(l);

    }

}

void LexScene::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void LexScene::drawForeground(QPainter *painter, const QRectF &rect)
{

}


