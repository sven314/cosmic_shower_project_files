#include <dragdropstandarditemmodel.h>
#include <QMimeData>
#include <QMimeType>
#include <QStandardItem>
#include <QDebug>
#include <QAbstractItemModel>

/*DragDropStandardItemModel::DragDropStandardItemModel(QWidget *parent) : QStandardItemModel(parent)
{
}*/
Qt::DropActions DragDropStandardItemModel::supportedDropActions()const
{
        return Qt::CopyAction | Qt::MoveAction;
    }


Qt::ItemFlags DragDropStandardItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);//QFileSystemModel::flags(index);

    if (index.isValid())
    {
        return Qt::ItemIsDropEnabled | defaultFlags;
    }else{
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    }
}

QStringList DragDropStandardItemModel::mimeTypes() const
{
    QStringList types;
    //types << "application/vnd.text.list";
    //types << "text/uri-list";
    types << "text/plain";
    types << "application/x-dndicondata";
    //qDebug("standarditemmodel mimeTypes ausgefuehrt");

    return types;
}

bool DragDropStandardItemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    //qDebug("I am in the model");
    if (action==Qt::IgnoreAction){
    return true;
    }
    //qDebug("not ignored");
    //if (!data->hasFormat("application/vnd.text.list"))
    if (!data->hasFormat("text/plain"))
    {
        return false;
    }
    //qDebug("not wrong Format");
    if (column > 0)
    {
        return false;
    }
    //qDebug("right after first return sequences, column = 0");
    //QByteArray encodedData = data->data("application/vnd.text.list");
    QByteArray encodedData = data->data("text/plain");
    QByteArray encodedIcon = data->data("application/x-dndicondata");
    QDataStream stream2(&encodedIcon, QIODevice::ReadOnly);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList <QIcon> newIcons;
    QStringList newItems;
    int rows = 0;
    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }
    rows = 0;
    while (!stream2.atEnd()){
        QIcon icon;
        stream2 >> icon;
        newIcons << icon;
        ++rows;
    }
    int iconCounter = 0;
    int itemCounter = 0;
    foreach(QString text,newItems)
    {
        if (itemCounter%2==0)
        {
            QStandardItem *item=new QStandardItem(newItems.at(itemCounter));
            //item->setData(newItems.at(itemCounter),Qt::DisplayRole);
            item->setData(newItems.at(itemCounter+1),Qt::UserRole);
            item->setData(newIcons.at(iconCounter),Qt::DecorationRole);
            appendRow(item);
            iconCounter++;
        }
        itemCounter++;
    }
    //qDebug()<<rows;
    /*odelIndex model = parent;
    if (row != -1){
             beginRow = row;
    }
    else if (parent.isValid()){
             beginRow = parent.row();
    }
    else {
        beginRow = rowCount();
    }
    bool setDataDone = true;
    //qDebug("right before foreach loop");
    int i = 0;
    int nowRow = beginRow;
    foreach (QString text, newItems) {
        //qDebug()<<text;
        QModelIndex idx = this->index(nowRow,0);
        if(i==0){
            //setRowCount(rowCount()+1);
            QStandardItem* item = new QStandardItem(text);
            this->appendRow(item);
        }
        if (this->setData(idx,text,i))
        if (i==0){
            i=1;
        }else if (i==1){
           i=0;
        }else{
            setDataDone = false;
            break;
        }
        if(i==0)
        {
            nowRow++;
        }
    }
    nowRow = beginRow;
    foreach (QIcon icon, newIcons){
        QModelIndex idx = this->index(nowRow,0);
        if(!setData(idx,icon,Qt::DecorationRole)){
            setDataDone = false;
        }
        nowRow++;
    }
    */
    return false;
}
