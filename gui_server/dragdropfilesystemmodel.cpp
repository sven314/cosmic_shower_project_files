#include <dragdropfilesystemmodel.h>
#include <QDebug>
#include <QMimeData>

QStringList DragDropFileSystemModel::mimeTypes() const
{
    QStringList types;
    //types << "application/vnd.text.list";
    //types << "text/uri-list";
    types << "text/plain";
    //qDebug("filesystemmodel mimeTypes ausgefuehrt");
    return types;
}
QMimeData *DragDropFileSystemModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;
     QByteArray encodedIcon;
     QDataStream stream(&encodedData, QIODevice::WriteOnly);
     QDataStream stream2(&encodedIcon,QIODevice::WriteOnly);
     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             QIcon icon = fileIcon(index);
             stream2 << icon;
             QString text = data(index, Qt::DisplayRole).toString();
             stream << text;
             text = filePath(index);//data(index, Qt::UserRole).toString();
             //qDebug()<<data(index,Qt::UserRole);
             stream << text;
         }
     }
     //mimeData->setData("application/vnd.text.list", encodedData);
     mimeData->setData("application/x-dndicondata",encodedIcon);
     mimeData->setData("text/plain", encodedData);
     //qDebug("filesystemmodel mimeData was set");
     return mimeData;
 }
