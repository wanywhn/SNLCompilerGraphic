#include "parseitem.h"

#include <QPainter>


ParseItem::ParseItem(const QString &text, QGraphicsItem *parent):QGraphicsTextItem (text,parent)
{
    myparent=parent;

}

void ParseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsTextItem::paint(painter,option,widget);
    auto rect=boundingRect();
    painter->drawRect(rect);
}
