#include <mylistview.h>
#include <QLabel>

/*MyListView::MyListView(QWidget *parent) : QListView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}*/
void MyListView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void MyListView::dropEvent(QDropEvent *event)
{
    qDebug("I am here in list");
    event->acceptProposedAction();
}
