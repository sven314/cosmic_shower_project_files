#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H
#include <QListWidget>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeType>
#include <QKeyEvent>

class MyListView : public QListView
{
public:
    using QListView::QListView;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
};

#endif // MYLISTWIDGET_H
