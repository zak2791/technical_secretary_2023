#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT

private:
    bool process;
    int mat;
    QString address;
    int port;
    QUdpSocket* udp_socket;
    //QUdpSocket* udp_server;
    QByteArray data;
    int state;
    QList<QString> ip;

private slots:
    void sendUdp(void);
    void readPendingDatagrams(void);


public:
    UdpClient(int _mat, QObject* parent = nullptr);

signals:
    void conn(int, QString);

};

#endif // UDPCLIENT_H
