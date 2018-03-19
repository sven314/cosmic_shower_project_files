#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDate>
#include <mycounter.h>
#include <QProcess>
//#include <QTcpSocket>
//#include <QDataStream>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    bool currentlyRunning;
    QString timeFolderFormat;
    QString timeFileFormat;
    QProcess *process1;
    MyCounter *mycounter;
    explicit client(QWidget *parent = 0);
    ~client();
    void start_stop_counter();
    void on_startRunning();
    void on_stopRunning();
    void startConnection();

public slots:
    void writeConsole();

private slots:
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_clicked();
    //void readData();
    //void displayError(QAbstractSocket::SocketError socketError);
    //void on_testButton_clicked();

private:
    Ui::client *ui;
    //QTcpSocket *tcpSocket = nullptr;
    //QDataStream data_stream;
};

#endif // CLIENT_H
