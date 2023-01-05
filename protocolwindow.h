#ifndef PROTOCOLWINDOW_H
#define PROTOCOLWINDOW_H

#include <QGraphicsView>
#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsItem>
#include "protocolrates.h"
#include <QMessageBox>
#include <fightqueue.h>
#include "protocolrates.h"
#include "protocolnames.h"
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient : public QObject
{
    Q_OBJECT

private:
    QByteArray data;
    QString address;
    int mat;

public:
    TcpClient(QByteArray _data, QString _address, int _mat, QWidget* parent) : QObject(parent)
    {
        data = _data;
        address = _address;
        mat = _mat;
        qDebug()<<address<<mat<<data.length();
    }

public slots:
    void run(void){
        qDebug()<<"tcp address = "<<address<<mat;
        QTcpSocket* tcp = new QTcpSocket(this);
        tcp->connectToHost(QHostAddress("192.168.0.102"), mat);
        if (tcp->waitForConnected(100)){
            qDebug()<<"Connected!";
            int i = tcp->write("hello");
            qDebug()<<"Writed = "<<i;
        }
        tcp-> close();

        /*
        sock = socket.socket()
        sock.settimeout(0.1)
        try:
            sock.connect(self.addr)
            sock.send(self.data)
        except:
            print("err socket")
        finally:
            sock.close()
            */
    }

};

class LeftRightPushButton : public QPushButton
{
    Q_OBJECT

public:
    LeftRightPushButton(QString title, QWidget* parent = nullptr) : QPushButton(title, parent)
    {

    }

private:
    void mouseReleaseEvent(QMouseEvent* e){
        if(e->button() == Qt::LeftButton)
            left_right_clicked(false, this->objectName());
        else if(e->button() == Qt::RightButton)
            left_right_clicked(true, this->objectName());
    }

signals:
    void left_right_clicked(bool, QString);

};

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(int* curX, QWidget* parent = nullptr) : QGraphicsView(parent)
    {
        currentX = curX;
    }

private:
    int* currentX;
    void mouseDoubleClickEvent(QMouseEvent* e)
    {
        if(e->button() == Qt::RightButton){
            ProtocolRates* item = static_cast<ProtocolRates*>(itemAt(e->pos().x(), e->pos().y()));
            if(item == NULL)
                return;

            int posX = item->pos().x();
            int itemW = item->w;
            QList<QGraphicsItem*> l = scene()->items(posX, e->pos().y(), scene()->width() - posX, e->pos().y() + 1, Qt::IntersectsItemShape, Qt::AscendingOrder);
            qDebug()<<"shift 2"<<l;
            QMessageBox* dialog = new QMessageBox(QMessageBox::Warning, "Первое техническое действие!", "Вы уверены?", QMessageBox::Ok | QMessageBox::Cancel);
            int result = dialog->exec();
            if(result == QMessageBox::Ok){
                item = new ProtocolRates("+", "+", false, false);
                item->setSize(height() / 4.0, height()/3);
                item->setPos(posX - itemW / 2 + item->w / 2, height()/2);
                scene()->addItem(item);
                *currentX += item->w;
                for(int i = 0; i < l.length(); i++){
                    l.at(i)->setPos(l.at(i)->pos().x() + item->w, l.at(i)->pos().y());
                    qDebug()<<"shift 1";
                }

                shift(item->w, posX);
            }
        }
        if(e->button() == Qt::LeftButton){
            ProtocolRates* item = static_cast<ProtocolRates*>(scene()->itemAt(e->pos().x(), e->pos().y(), QTransform()));
            if(item == NULL || item->flagNoStrikethrough == true)
                return;
            QMessageBox* dialog = new QMessageBox(QMessageBox::Warning, "Зачеркивание!", "Вы уверены?",
                                                  QMessageBox::Ok | QMessageBox::Cancel);
            int result = dialog->exec();
            if(result == QMessageBox::Ok){
                item->strikethrough = true;
                int penWidth = 1;
                item->update(QRectF(-item->w / 2 - penWidth / 2, -item->h / 2 - penWidth / 2,
                                     item->w + penWidth, item->h + penWidth));
                strike(item->pos().x(), item->pos().y());
            }
        }
}

signals:
    void strike(int, int);
    void shift(int, int);

};

class ProtocolWindow : public QWidget
{
    Q_OBJECT
public:
    ProtocolWindow(QWidget* parent = nullptr);
    void setMat(QString);

private:


    bool calculation(void);
    void rate_to_png(void);

    int spacing;
    int margin;


    virtual void resizeEvent(QResizeEvent*);
    virtual void paintEvent(QPaintEvent*);

    FightQueue* q;
    QString address;
    int mat;
    int vin;
    ProtocolRates* lastItem;
    ProtocolRates* pairedItem;

    LeftRightPushButton* btnNk;
    LeftRightPushButton* btnV;
    LeftRightPushButton* btnBP;
    LeftRightPushButton* btnUd;
    LeftRightPushButton* btnNkT;
    LeftRightPushButton* btnYP;
    LeftRightPushButton* btnPT;
    LeftRightPushButton* btnOPB;
    LeftRightPushButton* btnNPB;
    LeftRightPushButton* btnNkd;
    LeftRightPushButton* btnTNk;
    LeftRightPushButton* btnAr;
    LeftRightPushButton* btnPs;  

    QPushButton* btnBack;
    LeftRightPushButton* btn2;
    LeftRightPushButton* btn1;
    LeftRightPushButton* btnA;
    LeftRightPushButton* btn2P;
    LeftRightPushButton* btn1P;
    LeftRightPushButton* btnAP;
    LeftRightPushButton* btnZv;
    LeftRightPushButton* btnVyh;
    LeftRightPushButton* btnZp;
    LeftRightPushButton* btnP1;
    LeftRightPushButton* btnP2;
    LeftRightPushButton* btnP3;
    LeftRightPushButton* btnNy;
    LeftRightPushButton* btnDoc;
    LeftRightPushButton* btnVin;

    QLabel* BallsRed;
    QLabel* BallsBlue;
    QLabel* AktRed;
    QLabel* AktBlue;
    QLabel* ResultRed;
    QLabel* ResultBlue;
    QLabel* nameMain;
    QLabel* nameRef;
    QLabel* nameSide;
    QLabel* time;
    ProtocolNames* itemNameRed;
    ProtocolNames* itemNameBlue;
    QGraphicsScene* sceneNameRed;
    QGraphicsScene* sceneNameBlue;
    QGraphicsView* sportsmenRed;
    QGraphicsView* sportsmenBlue;
    QGraphicsScene* sceneBlue;
    QGraphicsScene* sceneRed;

    MyGraphicsView* rateRed;
    MyGraphicsView* rateBlue;

    int num_fight;

    QGridLayout grid;

    LeftRightPushButton* btn2_red;
    LeftRightPushButton* btn2_blue;
    LeftRightPushButton* btn1_red;
    LeftRightPushButton* btn1_blue;
    LeftRightPushButton* btnA_red;
    LeftRightPushButton* btnA_blue;
    LeftRightPushButton* btn2P_red;
    LeftRightPushButton* btn2P_blue;
    LeftRightPushButton* btn1P_red;
    LeftRightPushButton* btn1P_blue;
    LeftRightPushButton* btnAP_red;
    LeftRightPushButton* btnAP_blue;
    LeftRightPushButton* btnZv_red;
    LeftRightPushButton* btnZv_blue;
    LeftRightPushButton* btnVyh_red;
    LeftRightPushButton* btnVyh_blue;
    LeftRightPushButton* btnZp_red;
    LeftRightPushButton* btnZp_blue;
    LeftRightPushButton* btnP1_red;
    LeftRightPushButton* btnP1_blue;
    LeftRightPushButton* btnP2_red;
    LeftRightPushButton* btnP2_blue;
    LeftRightPushButton* btnP3_red;
    LeftRightPushButton* btnP3_blue;

public slots:
    void Back(void);
    void shiftRed(int, int);
    void shiftBlue(int, int);
    void strikeRed(int, int);
    void strikeBlue(int, int);
    void pastTime(QString);
    void showQueue(void);
    void selectFight(QString);
    void showFight(QString);
    void setAddr(int, QString);
    void resetRate(void);

private slots:
    void Rate(bool, QString);
    void keyMode(int);

signals:
    void change_prav(int, QString);
    void change_vyh(int, QString);
    void set_plus(int, QString);
    void change_rate(int, int, int, int);
    void nameRed(QString);
    void regRed(QString);
    void nameBlue(QString);
    void regBlue(QString);
    void setWeight(QString);

};

#endif // PROTOCOLWINDOW_H
