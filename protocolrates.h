#ifndef PROTOCOLRATES_H
#define PROTOCOLRATES_H

#include <QGraphicsItem>
#include <QFont>
#include <QPainter>
#include <QEvent>

class ProtocolRates : public QGraphicsItem
{
public:
    ProtocolRates(QString _rate,
                  int _value,
                  bool _parter=false,
                  bool _strike=false,
                  QGraphicsItem * parent=nullptr);

    ProtocolRates(QString _rate,
                  QString _value,
                  bool _parter=false,
                  bool _strike=false,
                  QGraphicsItem * parent=nullptr);

    int w;
    int h;
    void setSize(int, int);
    bool flagNoStrikethrough;
    bool strikethrough;        //если True: отобразить зачеркнутым
    int getIValue(void);
    QString getSValue(void);
    bool getTypeValue(void);

private:
    QFont f;
    QString rate;
    bool parter;
    QColor color;
    int value;
    QString svalue;
    bool typeValue;

    virtual QRectF boundingRect(void) const;
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    virtual bool sceneEvent(QEvent*);
};

#endif // PROTOCOLRATES_H
