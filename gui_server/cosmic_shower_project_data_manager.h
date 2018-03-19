#ifndef COSMIC_SHOWER_PROJECT_DATA_MANAGER_H
#define COSMIC_SHOWER_PROJECT_DATA_MANAGER_H

#include <QWidget>
//#include <QMessageBox>
#include <QFileSystemModel>
#include <QListView>
#include <QDir>
#include <QDateTime>
#include <QProcess>
#include <QDropEvent>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData>
#include <QIcon>
#include <QtCore>
#include <dragdropfilesystemmodel.h>
#include <dragdropstandarditemmodel.h>
//#include <QNetworkInterface>
#include "mycounter.h"
#include "settings.h"
//#include "tcpServer.h"

class QDragEnterEvent;
class QDropEvent;

namespace Ui {
class cosmic_shower_project_data_manager;
}

class cosmic_shower_project_data_manager : public QWidget
{
    Q_OBJECT

/*protected:
    void mousePressEvent(QMouseEvent *event);*/

public:
    explicit cosmic_shower_project_data_manager(QWidget *parent = 0);
    ~cosmic_shower_project_data_manager();

    DragDropFileSystemModel *dirmodel;
    DragDropFileSystemModel *filemodel;
    DragDropStandardItemModel *listmodel;
    bool eventFilter(QObject *object, QEvent *event);
    QProcess *process2;
    QProcess *process1;
    QString timeFolderFormat;
    QString timeFileFormat;
    MyCounter *mycounter;
    Settings *mysettings;


private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_settings_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void writeConsole1();

    void writeConsole2();

private:
    Ui::cosmic_shower_project_data_manager *ui;
    void start_stop_counter();
   // tcpServer myTcpServer;
};

#endif // COSMIC_SHOWER_PROJECT_DATA_MANAGER_H
