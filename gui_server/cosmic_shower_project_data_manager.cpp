#include "cosmic_shower_project_data_manager.h"
#include "ui_cosmic_shower_project_data_manager.h"
#include <QDesktopWidget>
#include <QDebug>

cosmic_shower_project_data_manager::cosmic_shower_project_data_manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cosmic_shower_project_data_manager)
{
    ui->setupUi(this);

    ui->horizontalLayout_3->setAlignment(Qt::AlignLeft);
    ui->verticalLayout_4->setAlignment(Qt::AlignLeft);
    ui->verticalLayout_5->setAlignment(Qt::AlignLeft);
    ui->verticalLayout_6->setAlignment(Qt::AlignLeft);

    //QString sPath = QApplication::applicationDirPath();
    //sPath.append("/content");
    //QString sPath = "./content";
    QString sPath = QApplication::applicationDirPath();
    if(!QDir(sPath).exists())
    {
        QDir().mkdir(sPath);
    }
    timeFolderFormat = "yyyy-MM-dd";
    timeFileFormat = "yyyy-MM-dd_hh-mm-ss";
    process2 = new QProcess;
    connect(process2,SIGNAL(readyReadStandardOutput()),this,SLOT(writeConsole2()));
    process1 = new QProcess;
    connect(process1,SIGNAL(readyReadStandardOutput()),this,SLOT(writeConsole1()));

    /*  make models and drag and drop possible  */
    dirmodel = new DragDropFileSystemModel(this);
    dirmodel->setReadOnly(true);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);

    filemodel = new DragDropFileSystemModel(this);
    filemodel->setReadOnly(false);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    filemodel->setRootPath(sPath);
    ui->listView->setModel(filemodel);

    listmodel = new DragDropStandardItemModel(0,1);
    ui->listView_2->setModel(listmodel);
    ui->listView_2->installEventFilter(this);
    listmodel->setColumnCount(1);
    listmodel->setRowCount(0);
    //listmodel->setData(listmodel->index(0,0),"test");


    QModelIndex idx1 = dirmodel->index(sPath);
    ui->treeView->setRootIndex(idx1);
    ui->treeView->setColumnWidth(0,300);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    QModelIndex idx2 = filemodel->index(sPath);
    ui->listView->setRootIndex(idx2);
    ui->listView->installEventFilter(this);


    sPath.append("/data");
    QString date = QDate::currentDate().toString(timeFolderFormat);
    QString timedate = QDateTime::currentDateTime().toString(timeFileFormat);
    QString pathFolderCurrentDate = sPath+"/"+date;
    QString pathFileCurrentDate = pathFolderCurrentDate+"/data_"+timedate+".dat";
    ui->lineEditPath->setText(pathFileCurrentDate);

    ui->lineEditPath_2->setText(sPath+"/analyzed_"+timedate+".dat");

    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(false);
    ui->listView->setDropIndicatorShown(true);
    ui->listView->setDragDropMode(QAbstractItemView::DragOnly);

    ui->listView_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView_2->setDragEnabled(true);
    ui->listView_2->setAcceptDrops(true);
    ui->listView_2->viewport()->setAcceptDrops(true);
    ui->listView_2->setDropIndicatorShown(true);
    ui->listView_2->setDragDropMode(QAbstractItemView::DropOnly);
    ui->listView_2->setDragDropOverwriteMode(false);
    /* until here all about drag and drop */

    ui->comboBox->addItem("compare");
    ui->comboBox->addItem("differenz_zeilen");
    //qDebug("end of cosmic shower ui initialisation");

    // mycounter->installEventFilter(this);

    /* tcp-server stuff */
    /*if (!myTcpServer.listen()){
        QMessageBox::critical(this, tr("Threaded Server"),
                              tr("Unable to start the server: %1.")
                              .arg(myTcpServer.errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use first non-localhost IPv4 address
   /* use localhost for testing
    for (int i = 0; i < ipAddressesList.size(); ++i){
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()){
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    // if not found, use localhost:
    if (ipAddress.isEmpty()){
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        ui->textBrowser->setText(tr("server is running on\n\nIP: %1\nport: %2\n\n")
                                 .arg(ipAddress).arg(myTcpServer.serverPort()));
    }*/
}

bool cosmic_shower_project_data_manager::eventFilter(QObject *object, QEvent *event)
{
    if (/*object == ui->mListVi1w &&*/ event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            if (object == ui->listView_2)
            {
                while(!ui->listView_2->selectionModel()->selectedRows().empty()){
                    ui->listView_2->model()->removeRow(ui->listView_2->selectionModel()->selectedRows().first().row());
                }
                /*foreach(QModelIndex item, ui->listView_2->selectionModel()->selectedIndexes())
                {
                    ui->listView_2->model()->removeRow(item.row());
                }*/
            }
            if (object == ui->listView)
            {
                //qDebug()<<"started";
                for(int i =0; i<ui->listView->selectionModel()->selectedIndexes().size();i++)
                {
                    //qDebug()<<"not empty";
                    QModelIndex idx = ui->listView->selectionModel()->selectedIndexes().first();
                    QFile::remove(filemodel->filePath(idx));
                    //qDebug()<<filemodel->filePath(idx);
                }
            }
        }
           // special Esc handling here
    }
    else
        return false;
}

cosmic_shower_project_data_manager::~cosmic_shower_project_data_manager()
{
    delete ui;
}


// all drag and drop actions and stuff with file manager
void cosmic_shower_project_data_manager::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}

void cosmic_shower_project_data_manager::on_checkBox_stateChanged(int arg1)
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

void cosmic_shower_project_data_manager::on_checkBox_2_stateChanged(int arg1)
{
    if (ui->checkBox_2->isChecked())
    {
        //ui->lineEditPath->setReadOnly(true);
        ui->lineEditPath_2->setDisabled(true);
    }else{
        //ui->lineEditPath->setReadOnly(false);
        ui->lineEditPath_2->setDisabled(false);
    }
}

void cosmic_shower_project_data_manager::on_listView_clicked(const QModelIndex &index)
{

}

// counter stuff
void cosmic_shower_project_data_manager::start_stop_counter()
{
    QString sPath = QApplication::applicationDirPath();
    QString program = sPath+"/timestamp_diffmean_systime";
    QStringList arguments;
    arguments.append("-o");
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
    //qDebug()<<arguments;
    process1->start(program,arguments);
    if (ui->checkBox_3->isChecked())
    {
        mycounter = new MyCounter(this);
        mycounter->showMaximized();
        connect(ui->textBrowser,SIGNAL(textChanged()),mycounter,SLOT(countUp()));
    }
    process1->waitForBytesWritten();
    //process1->waitForFinished();
}

void cosmic_shower_project_data_manager::on_pushButton_clicked()
{
    start_stop_counter();
}

void cosmic_shower_project_data_manager::on_pushButton_2_clicked()
{
    QString sPath = QApplication::applicationDirPath();
    QString exportFile ="";
    if(ui->checkBox_2->isChecked())
    {
        QString timedate = QDateTime::currentDateTime().toString(timeFileFormat);
        exportFile = sPath+"/content/data/analysed_"+timedate+".dat";
    }else{
        exportFile= ui->lineEditPath_2->text();
    }
    QStringList arguments;
    QString program = sPath;
    //QString teststring=program +"/compare";
    if (ui->comboBox->currentText()=="compare"){
        program += "/compare";
        arguments.append("-o");
        arguments.append(exportFile);

        for(int i=0; i<ui->listView_2->model()->rowCount();i++)
        {
            QModelIndex idx = ui->listView_2->model()->index(i,0);
            arguments.append(ui->listView_2->model()->data(idx,Qt::UserRole).toString());
        }
    }
   /*foreach(QString argument,arguments)
   {
       teststring.append(" "+argument);
   }*/
   //qDebug()<<teststring;
   if (QFile::exists(program))
   {
       process2->start(program,arguments,QIODevice::ReadWrite);
       process2->waitForBytesWritten();
       if(process2->waitForFinished())
       {
           ui->textBrowser->append("  Done!");
       }
   }else
   {
       ui->textBrowser->setText("no such program as "+program+" in "+sPath);
   }
  /* while (!process2->waitForFinished(100))
   {

       ui->textBrowser->setText(process2->readAllStandardOutput());
   }
   ui->textBrowser->setText(process2->readAllStandardOutput());*/
   //qDebug()<<process2->waitForFinished();
   /*if (!process2.waitForFinished(1)) {
       process2.kill();
       process2.waitForFinished(1);
   }*/
}

// die Ausgabe aus den Programmen in der Konsole ausgeben
void cosmic_shower_project_data_manager::writeConsole1()
{
    ui->textBrowser->setText(process1->readAllStandardOutput());
}

void cosmic_shower_project_data_manager::writeConsole2()
{
    ui->textBrowser->setText(process2->readAllStandardOutput());
}

// settings
void cosmic_shower_project_data_manager::on_settings_clicked()
{
    mysettings = new Settings(this);
    mysettings->show();
}
