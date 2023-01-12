#include "fight.h"

#include <QFile>
#include <QTextStream>
//#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QPainter>
#include <QTcpSocket>
#include "protocolwindow.h"
#include <QWheelEvent>

MouseWheelFilter::MouseWheelFilter(QObject* pobj) : QObject(pobj){

}

bool MouseWheelFilter::eventFilter(QObject* pobj, QEvent* pe){
    if(pe->type() == QEvent::Wheel){
        return true;
    }else
        return false;

}

Fight::Fight(QString r,
             QString b,
             QString nr,
             QString nb,
             QString t,
             int nf,
             QString addr,
             int _mat,
             QWidget* parent) : QLabel(parent)
{
    address = addr;
    mat = _mat;
    QList<QString> _red = r.split(":");
    if(_red.length() == 2)
        red = _red.at(1) + " (" + _red.at(0) + ")";
    else
        red = r;

    QList<QString> _blue = b.split(":");
    if(_blue.length() == 2)
        blue = _blue.at(1) + " (" + _blue.at(0) + ")";
    else
        blue = b;
    note_red = nr;
    note_blue = nb;
    num_fight = nf;
    title = t;//QString::number(t);
    setGeometry(0, 0, 560, 100);

    QLabel* lblRef = new QLabel("Судьи", this);
    lblRef->setGeometry(400, 20, 160, 20);
    lblRef->setAlignment(Qt::AlignHCenter);

    lblFight = new QLabel(this);
    lblFight->setGeometry(0, 20, 560, 80);
    lblFight->setVisible(false);

    btnOk = new QPushButton(QString::number(num_fight), this);
    btnOk->setGeometry(0, 0, 40, 20);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(sendPix()));

    //судьи
    cmbMain = nullptr;
    refMain = "";

    cmbRef = nullptr;
    refRef = "";

    cmbSaid = nullptr;
    refSaid = "";

    setFrameStyle(1);

    db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("baza_in.db");
    query = QSqlQuery (db);

    QFile referees("referees.txt");
    if(!referees.exists()){
        referees.open(QFile::WriteOnly);
        referees.close();
    }
    show();
}

void Fight::sendPix(){
    qDebug()<<num_fight;
    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("baza_out.db");
    if (!db.open()){
        qDebug()<<"Cannot open database:"<< db.lastError();
    }else{
        QSqlQuery query;
        if(!query.exec("SELECT result FROM rounds WHERE fight = " + QString::number(num_fight))){
            qDebug()<<"error database"<< db.lastError();
        }else{
            if(query.next()){
                qDebug()<<"address = "<<address<<"mat = "<<mat;
                QByteArray ba = query.value(0).toByteArray();
                ba.prepend(num_fight);
                QTcpSocket* tcp = new QTcpSocket(this);
                tcp->connectToHost(QHostAddress(address), mat);
                if (tcp->waitForConnected(100)){
                    qDebug()<<"Connected!";
                    int i = tcp->write(ba);
                    qDebug()<<"Writed = "<<i<<num_fight;
                    if(tcp->waitForReadyRead(2000)){
                        if(tcp->readAll() == "ok"){
                            if(!query.exec("UPDATE rounds SET sent = 1 WHERE fight = " + QString::number(num_fight))){
                                qDebug()<<"error database"<< db.lastError();
                            }else{
                                btnOk->setEnabled(false);
                                btnOk->setStyleSheet("background-color: green; color: white;");
                            }
                        }
                    }else{
                        qDebug()<<"not data";
                    }
                }
                tcp-> close();
                delete tcp;
            }
        }
        db.close();
    }
}

void Fight::setCombo(){
    QList<QString> ref;
    ref.append("");
    QFile f("referees.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inf(&f);
    inf.setCodec("utf-8");
    while(!inf.atEnd())
        ref.append(inf.readLine());
    f.close();


    if (!db.open()){
        qDebug()<<"Cannot open database:"<< db.lastError();
    }else{

        QString sql = "SELECT * FROM referees WHERE id_fight = " + QString::number(num_fight);
        if(!query.exec(sql)){
            qDebug()<<"error database"<< db.lastError();
        }else{

            QList<int> id;
            QList<int> id_fight;
            QList<QString> ref1;
            QList<QString> ref2;
            QList<QString> ref3;

            cmbMain = new QComboBox(this);
            cmbMain->setGeometry(401, 40, 159, 20);
            cmbMain->addItems(ref);
            cmbMain->setObjectName("main");
            cmbMain->setFocusPolicy(Qt::NoFocus);
            cmbMain->installEventFilter(new MouseWheelFilter(cmbMain));
            connect(cmbMain, SIGNAL(activated(int)), this, SLOT(selectRef(int)));
            cmbRef = new QComboBox(this);
            cmbRef->setGeometry(401, 60, 159, 20);
            cmbRef->addItems(ref);
            cmbRef->setObjectName("ref");
            cmbRef->setFocusPolicy(Qt::NoFocus);
            cmbRef->installEventFilter(new MouseWheelFilter(cmbMain));
            connect(cmbRef, SIGNAL(activated(int)), this, SLOT(selectRef(int)));
            cmbSaid = new QComboBox(this);
            cmbSaid->setGeometry(401, 80, 159, 20);
            cmbSaid->addItems(ref);
            cmbSaid->setObjectName("said");
            cmbSaid->setFocusPolicy(Qt::NoFocus);
            cmbSaid->installEventFilter(new MouseWheelFilter(cmbMain));
            connect(cmbSaid, SIGNAL(activated(int)), this, SLOT(selectRef(int)));

            if(query.next()){

                refMain = query.value(2).toString();//ref1.at(0);
                int index = cmbMain->findText(refMain);
                cmbMain->setCurrentIndex(index);

                refRef = query.value(3).toString();//ref2.at(0);
                index = cmbRef->findText(refRef);
                cmbRef->setCurrentIndex(index);

                refSaid = query.value(4).toString();//ref3.at(0);
                index = cmbSaid->findText(refSaid);
                cmbSaid->setCurrentIndex(index);
            }

        }
        db.close();
    } 
}

void Fight::selectRef(int i){
    qDebug()<<"i = "<<i;
    if(sender()->objectName() == "main")
        refMain = cmbMain->itemText(i);
    else if(sender()->objectName() == "ref")
        refRef = cmbRef->itemText(i);
    else
        refSaid = cmbSaid->itemText(i);

    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("baza_in.db");
    if (!db.open()){
        qDebug()<<"Cannot open database:"<< db.lastError();
    }else{
        QSqlQuery query;
        QString sql = "SELECT * FROM referees WHERE id_fight = " + QString::number(num_fight);
        if(!query.exec(sql)){
            qDebug()<<"error database"<< db.lastError();
        }else{
            if(!query.next()){
                sql = "INSERT INTO referees (id_fight, ref1, ref2, ref3) VALUES ('%1', '%2', '%3', '%4')";
                QString str = sql.arg(num_fight).arg(refMain).arg(refRef).arg(refSaid);
                if(!query.exec(str)){
                    qDebug()<<"error database"<< db.lastError();
                }
            }else{
                sql = "UPDATE referees SET ref1 = '%1', ref2 = '%2', ref3 = '%3' WHERE id_fight = '%4'";
                QString str = sql.arg(refMain).arg(refRef).arg(refSaid).arg(num_fight);
                if(!query.exec(str)){
                    qDebug()<<"error database"<< db.lastError();
                }
            }
        }
        db.close();
    }
}

void Fight::setPix(QByteArray data){
    QPixmap pix;
    pix.loadFromData(data, "PNG");
    lblFight->setPixmap(pix.scaled(lblFight->width(), lblFight->height()));
    lblFight->setVisible(true);
}

void Fight::mousePressEvent(QMouseEvent* e){
    if(e->button() == Qt::LeftButton){
        if(lblFight->isVisible())
            emit show_fight(this->objectName());
        else
            emit select_fight(this->objectName());
        }
    if(e->button() == Qt::RightButton)
        emit show_next(red.split("\n").at(0), blue.split("\n").at(0));
}

void Fight::paintEvent(QPaintEvent*){
    QPainter p(this);

    p.drawLine(0, 0, 560, 0);
    p.drawLine(0, 20, 560, 20);
    p.drawLine(0, 60, 400, 60);
    p.drawLine(0, 99, 560, 99);

    p.drawLine(40, 0, 40, 20);
    p.drawLine(200, 20, 200, 100);
    p.drawLine(400, 20, 400, 100);

    p.drawText(  0, 20, 200, 40, Qt::AlignCenter, red);
    p.drawText(  0, 60, 200, 40, Qt::AlignCenter, blue);
    p.drawText(200, 20, 200, 40, Qt::AlignCenter, note_red);
    p.drawText(200, 60, 200, 40, Qt::AlignCenter, note_blue);
    p.drawText(  0,  0,  40, 20, Qt::AlignCenter, QString::number(num_fight));
    p.drawText( 40,  0, 520, 20, Qt::AlignCenter, title);
}
