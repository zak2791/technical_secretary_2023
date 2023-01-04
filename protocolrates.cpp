#include "protocolrates.h"

ProtocolRates::ProtocolRates(QString _rate,
                             int _value,
                             bool _parter,
                             bool _strike,  
                             QGraphicsItem * parent) : QGraphicsItem(parent)
{
    w = 0;
    h = 0;
    f = QFont();
    rate = _rate;
    parter = _parter;
    setAcceptHoverEvents(true);
    color = QColor("white");
    strikethrough = false;
    flagNoStrikethrough = _strike;
    value = _value;
    typeValue = false;
}

ProtocolRates::ProtocolRates(QString _rate,
                             QString _value,
                             bool _parter,
                             bool _strike,
                             QGraphicsItem * parent) : QGraphicsItem(parent)
{
    w = 0;
    h = 0;
    f = QFont();
    rate = _rate;
    parter = _parter;
    setAcceptHoverEvents(true);
    color = QColor("white");
    strikethrough = false;
    flagNoStrikethrough = _strike;
    svalue = _value;
    typeValue = true;
}

bool ProtocolRates::getTypeValue(){
    return typeValue;
}

int ProtocolRates::getIValue(){
        return value;
}

QString ProtocolRates::getSValue(){
        return svalue;
}

QRectF ProtocolRates::boundingRect() const{
    int penWidth = 1.0;
        return QRectF(-w / 2 - penWidth / 2, -h / 2 - penWidth / 2,
                       w + penWidth, h + penWidth);
}

void ProtocolRates::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*){
        p->setFont(f);
        p->fillRect(-w / 2, -h / 2, w, h, color);
        if(parter)
            p->drawEllipse(-h / 2, -h / 2, h, h);

        p->drawText(-w / 2, -h / 2, w, h, Qt::AlignCenter, rate);
        if(strikethrough){
            p->drawLine(-w / 2, -h / 2, w / 2, h / 2);
            p->drawLine(w / 2, -h / 2, -w / 2, h / 2);
        }
}

void ProtocolRates::setSize(int _w, int _h){
    w = _w;
    h = _h;
    f.setPixelSize(h);
}

bool ProtocolRates::sceneEvent(QEvent* e){
    if(!flagNoStrikethrough){
        int penWidth = 1;
        if(e->type() == QEvent::GraphicsSceneHoverEnter){
            color = QColor("gray");
            update(QRectF(-w / 2 - penWidth / 2, -h / 2 - penWidth / 2,
                           w + penWidth, h + penWidth));
        }
        if(e->type() == QEvent::GraphicsSceneHoverLeave){
            color = QColor("white");
            update(QRectF(-w / 2 - penWidth / 2, -h / 2 - penWidth / 2,
                           w + penWidth, h + penWidth));
        }
    }
        return true;
 }
