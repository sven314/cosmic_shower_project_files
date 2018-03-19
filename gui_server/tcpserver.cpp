#include "tcpserver.h"
#include "tcpthread.h"

tcpServer::tcpServer(QObject *parent)
    : QTcpServer(parent)
{
    randomStrings << "1" << "2" << "3";
}

void tcpServer::incomingConnection(qintptr socketDescriptor)
{
    QString testString = randomStrings.at(QRandomGenerator::global()->bounded(randomStrings.size()));
    tcpThread *thread = new tcpThread(socketDescriptor,testString,this);
    connect(thread, SIGNAL(finished()), thread, SLOT (deleteLater()));
}
