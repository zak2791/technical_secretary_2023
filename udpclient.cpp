#include "udpclient.h"
#include "QNetworkDatagram"
#include <QThread>
#include <QDataStream>
#include <QTimer>
#include <QNetworkInterface>

UdpClient::UdpClient(int _mat, QObject *parent) : QObject(parent)
{
    state = 0;
    mat = _mat;
    process = true;
    port = mat * 2000;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++){
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
                ip.append(list[nIter].toString());
            }
    }

    udp_socket = new QUdpSocket(this);
    udp_socket->bind(port);
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString::number(mat);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendUdp()));
    timer->start(3000);

}

void UdpClient::sendUdp(){
    for(int i = 0; i < ip.length(); i++){
        QList<QString> _ip = ip[i].split(".");
        if(_ip.length() == 4){
            char c = mat + 48;
            udp_socket->writeDatagram(&c, 1, QHostAddress(QString(_ip[0] + "." + _ip[1] + "." + _ip[2] + ".255")), port + 1);
        }
    }
    if(state == 0) state++;
    else if(state == 1){
        conn(2, "");
        state++;
    }
}

void UdpClient::readPendingDatagrams(){
    QNetworkDatagram dtg;
    do {
        dtg = udp_socket->receiveDatagram();
    }while(udp_socket->hasPendingDatagrams());
    if(state){
        state = 0;
        qDebug()<<"dtg.senderPort()"<<dtg.senderPort();
        conn(1, dtg.senderAddress().toString());
    }
}

