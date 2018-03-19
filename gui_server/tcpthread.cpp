#include "tcpthread.h"

tcpThread::tcpThread(int socketDescriptor, const QString &testString, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), text(testString)
{
}

void tcpThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_0);
    //out.setVersion(QDataStream::Qt_5_10);
    out << text;

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
