#include "protocolnames.h"

ProtocolNames::ProtocolNames(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    Name = "";
    status = 0;
    w = 0;
    h = 0;
    f = QFont();
}

void ProtocolNames::setName(QString name){
    Name = name;
}

void ProtocolNames::setStatus(int _status){
    status = _status;
    update();
}

QRectF ProtocolNames::boundingRect() const{
    int penWidth = 1;
    return QRectF(-w / 2 - penWidth / 2, -h / 2 - penWidth / 2,
                          w + penWidth, h + penWidth);
}

void ProtocolNames::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*){
    p->setFont(f);
    p->fillRect(-w / 2, -h / 2, w, h, QColor("white"));

    p->drawText(-w / 2 + 10, -h / 2, w - 10, h, Qt::AlignLeft | Qt::AlignVCenter, Name);

    if(status == 1)
        p->drawEllipse(-w / 2 + 10, -h / 2 + 10, w - 20, h - 20);
    else if(status == 2){
        p->drawLine(-w / 2 + 10, -h / 2 + 10, w / 2 - 10, h / 2 - 10);
        p->drawLine(w / 2 - 10, -h / 2 + 10, -w / 2 + 10, h / 2 - 10);
    }
}

void ProtocolNames::setSize(int _w, int _h){
    w = _w;
    h = _h;
    f.setPixelSize(h / 5);
}
