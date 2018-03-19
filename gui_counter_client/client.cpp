#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client),
    tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    timeFolderFormat = "yyyy-MM-dd";
    timeFileFormat = "yyyy-MM-dd_hh-mm-ss";
    QFont font = ui->pushButton->font();
    font.setBold(true);
    ui->pushButton->setFont(font);
    font = ui->label_3->font();
    font.setBold(true);
    ui->label_3->setFont(font);
    QPalette palette = ui->pushButton->palette();
    palette.setColor(ui->pushButton->foregroundRole(),Qt::darkGray);
    //ui->pushButton->setPalette(palette);
    currentlyRunning = false;
    palette = ui->label_3->palette();
    palette.setColor(ui->label_3->foregroundRole(),Qt::darkGray);
    ui->label_3->setPalette(palette);
    ui->label_3->setText("not running");
    ui->pushButton->setText("start");
    /*QString sPath = QApplication::applicationDirPath();
    QString program = sPath+"/data";
    ui->lineEditPath->setText(sPath);*/

    // setup lineEdit Entries from Timestamp
    QString sPath = QApplication::applicationDirPath();
    QString date = QDate::currentDate().toString(timeFolderFormat);
    QString timedate = QDateTime::currentDateTime().toString(timeFileFormat);
    QString pathFolderCurrentDate = sPath+"/content/data/"+date;
    QString pathFileCurrentDate = pathFolderCurrentDate+"/data_"+timedate+".dat";
    ui->lineEditPath->setText(pathFileCurrentDate);

    /*
    // tcpSocket configuration
    data_stream.setDevice(tcpSocket);
    connect(tcpSocket, &QIODevice::readyRead, this, &client::readData);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &client::displayError);
    */
}

client::~client()
{
    if (currentlyRunning){
    delete process1;
    }
    delete ui;
}

void client::on_stopRunning()
{
    currentlyRunning = false;
    QPalette palette = ui->pushButton->palette();
    palette.setColor(ui->pushButton->foregroundRole(),Qt::darkGray);
    //ui->pushButton->setPalette(palette);
    currentlyRunning = false;
    palette = ui->label_3->palette();
    palette.setColor(ui->label_3->foregroundRole(),Qt::darkGray);
    ui->label_3->setPalette(palette);
    ui->label_3->setText("not running");
    ui->pushButton->setText("start");
}

void client::on_startRunning()
{
    currentlyRunning = true;
    QPalette palette = ui->pushButton->palette();
    palette.setColor(ui->pushButton->foregroundRole(),Qt::darkGreen);
    //ui->pushButton->setPalette(palette);

    palette = ui->label_3->palette();
    palette.setColor(ui->label_3->foregroundRole(),Qt::darkGreen);
    ui->label_3->setPalette(palette);
    ui->label_3->setText("running");
    ui->pushButton->setText("stop");
}

void client::on_checkBox_stateChanged(int arg1)
{
    if (ui->checkBox->isChecked())
    {
        //ui->lineEditPath->setReadOnly(true);
        ui->lineEditPath->setDisabled(true);
    }else{
        //ui->lineEditPath->setReadOnly(false);
        ui->lineEditPath->setDisabled(false);
    }
}

void client::start_stop_counter(){
    if (!currentlyRunning)
    {
        on_startRunning();
        QString sPath = QApplication::applicationDirPath();
        QString program = sPath+"/timestamp_diffmean_systime";
        QStringList arguments;
        /*arguments.append("-o");
        if (ui->checkBox->isChecked())
        {
            QString date = QDate::currentDate().toString(timeFolderFormat);
            QString timedate = QDateTime::currentDateTime().toString(timeFileFormat);
            QString pathFolderCurrentDate = sPath+"/content/data/"+date;
            QString pathFileCurrentDate = pathFolderCurrentDate+"/data_"+timedate+".dat";
            arguments.append(pathFileCurrentDate);
        }else{
            QString temp = ui->lineEditPath->text();
            QString outputFile;
            for (int i=1;i<temp.size();i++)
            {
                outputFile+=temp[i];
            }
            //qDebug()<<outputFile;
            arguments.append(outputFile);
        }
        */
        arguments.append("-s");
        arguments.append("27");
        arguments.append("-p");
        arguments.append("-1");
        //qDebug()<<arguments;
        process1 = new QProcess;
        connect(process1,SIGNAL(readyReadStandardOutput()),this,SLOT(writeConsole()));
        process1->start(program,arguments);
        if (ui->checkBox_3->isChecked())
        {
            mycounter = new MyCounter(this);
           // mycounter->showMaximized();
            mycounter->showFullScreen();
            connect(ui->textBrowser,SIGNAL(textChanged()),mycounter,SLOT(countUp()));
        }
        process1->waitForBytesWritten();
        //process1->waitForFinished();
    }else{
        on_stopRunning();
       /* if (process1->state()!=QProcess::NotRunning)
        {*/
            process1->kill();
            process1->close();
       // }
    }
}

void client::on_pushButton_clicked()
{
    start_stop_counter();
}

void client::writeConsole()
{
    ui->textBrowser->setText(process1->readAllStandardOutput());
}

/*
// Tcp Socket test code

void client::on_testButton_clicked()
{
    startConnection();
}

void client::startConnection(){
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1",
                             ui->lineEditPath->text().toInt());
    if(tcpSocket->waitForConnected(3000)){
        //qDebug("connected!");
    }
}

void client::readData()
{
    data_stream.startTransaction();
    QString testData;
    data_stream >> testData;

    if (!data_stream.commitTransaction()){return;}
    ui->textBrowser->setText(testData);
}

void client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
}
*/
