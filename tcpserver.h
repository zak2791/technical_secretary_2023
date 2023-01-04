#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
//#include <QTcpSocket>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    TcpServer(int _mat, QObject* parent = nullptr);

private slots:
    virtual void slotNewConnection(void);
    void slotReadClient(void);

private:
    QTcpServer* server;
    int port;

signals:
    void conn(int);

};

#endif // TCPSERVER_H
