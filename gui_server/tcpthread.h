#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>

class tcpThread : public QThread
{
    Q_OBJECT
public:
    tcpThread(int socketDescriptor, const QString &text, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
};

#endif // TCPTHREAD_H
