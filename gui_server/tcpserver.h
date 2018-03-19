#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdlib.h>
#include <QRandomGenerator>
#include <QStringList>
#include <QTcpServer>
#include "tcpthread.h"

class tcpServer : public QTcpServer
{
    Q_OBJECT

public:
    tcpServer(QObject *parent = 0);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList randomStrings;
};

#endif // TCPSERVER_H
