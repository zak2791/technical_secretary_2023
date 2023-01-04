#ifndef PROTOCOLNAMES_H
#define PROTOCOLNAMES_H

#include <QGraphicsItem>
#include <QFont>
#include <QPainter>

class ProtocolNames : public QGraphicsItem
{
public:
    ProtocolNames(QGraphicsItem * parent=nullptr);
    void setName(QString);
    void setSize(int, int);
    void setStatus(int);
    int w;
    int h;

private:
    QString Name;
    int status;    //если 0: текст, 1: обведенный текст, 2: перечеркнутый текст
    QFont f;
    virtual QRectF boundingRect(void) const;
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);


};

#endif // PROTOCOLNAMES_H
