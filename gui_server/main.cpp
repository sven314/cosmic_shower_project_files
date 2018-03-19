#include "cosmic_shower_project_data_manager.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cosmic_shower_project_data_manager w;
    w.show();
    return a.exec();
}
