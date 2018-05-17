#ifndef PARSEITEM_H
#define PARSEITEM_H

#include <QGraphicsItem>
#include <QObject>

class ParseItem : public QGraphicsTextItem
{
public:
    ParseItem(const QString &text,QGraphicsItem *parent=Q_NULLPTR);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QGraphicsItem *myparent;
};

#endif // PARSEITEM_H
