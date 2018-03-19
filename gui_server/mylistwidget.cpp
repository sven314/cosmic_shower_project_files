#include <mylistview.h>

bool myListView::dropMimeData(int index, const QMimeData *data, Qt::DropAction action)
{
    QList <QUrl> urlList;
    QListWidgetItem *item;
    QFileInfo info;
    QString fName;

    urlList = data->urls(); // retrieve list of urls
    foreach(QUrl url, urlList) // iterate over list
    {
        fName = url.toLocalFile();
        info.setFile( fName );
        item = new QListWidgetItem(info.fileName());
        insertItem(index, item);
        ++index; // increment index to preserve drop order
    }

    return true;
}


QStringList myListView::mimeTypes () const
{
    QStringList qstrList;
    // list of accepted mime types for drop
    qstrList.append("text/uri-list");
    qstrList.append("text/plain");
    return qstrList;
}


Qt::DropActions myListView::supportedDropActions() const
{
    // returns what actions are supported when dropping
    return Qt::CopyAction | Qt::MoveAction;
}

