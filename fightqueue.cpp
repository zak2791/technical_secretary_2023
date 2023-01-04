#include "fightqueue.h"

#include <QScrollArea>
#include <QtSql>
#include <QSqlQuery>
#include <fight.h>

FightQueue::FightQueue(QString addr, int _mat, QWidget *parent) : QDialog(parent)
{
    address = addr;
    mat = _mat;
    setGeometry(10, 50, 600, 600);
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setGeometry(10, 10, 580, 580);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* fight_window = new QWidget(this);
    fight_window->setGeometry(0, 0, 560, 0);

    QSqlDatabase db_in = QSqlDatabase::addDatabase ("QSQLITE");
    db_in.setDatabaseName("baza_in.db");

    currentY = 0;

    if (!db_in.open()){
        qDebug() << "Cannot open database:" << db_in.lastError();
    }else{
        QSqlQuery query_in(db_in);
        QString sql = "SELECT * FROM rounds";
        if(!query_in.exec(sql)){
            qDebug()<<"error database";
        }else{
            QList<QString>  v3;
            QList<QString>  v4;
            QList<QString>  v5;
            QList<QString>  v6;
            QList<QString>  v2;
            QList<int>      v7;
            while(query_in.next()){
                v3.append(query_in.value(3).toString());
                v4.append(query_in.value(4).toString());
                v5.append(query_in.value(5).toString());
                v6.append(query_in.value(6).toString());
                v2.append(query_in.value(2).toString());
                v7.append(query_in.value(7).toInt());
            }
            db_in.close();

            QSqlDatabase db_out = QSqlDatabase::addDatabase ( "QSQLITE");
            db_out.setDatabaseName("baza_out.db");
            QSqlQuery query_out(db_in);

            if(!db_out.open()){
                qDebug() << "Cannot open database 2:" << db_out.lastError();
            }

            QString sql_out = "SELECT * FROM rounds WHERE fight = ";

            QList<QByteArray> lba;
            QList<int> l_sent;  //флаг отсылки данных главному секретарю
            for(int i = 0; i < v3.length(); i++){
                QString sql = "SELECT * FROM rounds WHERE fight = " + QString::number(v7.at(i));
                if(!query_out.exec(sql)){
                    qDebug()<<"error database" << db_out.lastError();
                }else{
                    if(query_out.next()){
                        lba.append(query_out.value(2).toByteArray());
                        l_sent.append(query_out.value(3).toInt());
                    }else{
                        lba.append(NULL);
                        l_sent.append(-1);
                    }
                }
            }

            int max_fight = -1;
            if(!query_out.exec("SELECT MAX(fight) FROM rounds")){
                qDebug()<<"error database MAX(fight)" << db_out.lastError();
            }else{
                if(query_out.next()){
                    max_fight = query_out.value(0).toInt();
                    //Fight* obj = findChild<Fight*>(QString::number(max_fight));
                    //scroll->ensureWidgetVisible(obj);
                }
            }

            db_out.close();

            for(int i = 0; i < lba.length(); i++){
                f.append(new Fight(v3.at(i),
                                   v4.at(i),
                                   v5.at(i),
                                   v6.at(i),
                                   v2.at(i),
                                   v7.at(i),
                                   address,
                                   mat,
                                   fight_window));
                f.last()->move(0, currentY);
                f.last()->setObjectName(QString::number(v7.at(i)));
                if(lba.at(i) == "")
                    f.last()->setCombo();
                else
                    f.last()->setPix(lba.at(i));

                if(l_sent.at(i) > 0 || l_sent.at(i) == -1){
                    f.last()->btnOk->setEnabled(false);
                    if(l_sent.at(i) > 0)
                        f.last()->btnOk->setStyleSheet("background-color: green; color: white;");
                }

                connect(f.last(), SIGNAL(select_fight(QString)), this, SLOT(selectFight(QString)));
                connect(f.last(), SIGNAL(show_fight(QString)), this, SLOT(showFight(QString)));
                currentY += 110;

            }

            fight_window->setGeometry(0, 0, 560, currentY);
            scroll->setWidget(fight_window);

            if(max_fight != -1){
                Fight* obj = findChild<Fight*>(QString::number(max_fight));
                scroll->ensureWidgetVisible(obj);
            }
        }
    }
}

void FightQueue::selectFight(QString s){
    select_fight(s);
}

void FightQueue::showFight(QString s){
    show_fight(s);
}

void FightQueue::closeEvent(QCloseEvent*){
    reject();
}
