#ifndef DRAGDROPFILESYSTEMMODEL_H
#define DRAGDROPFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <Qt>
class DragDropFileSystemModel : public QFileSystemModel
{
public:
    using QFileSystemModel::QFileSystemModel;
    //DragDropFileSystemModel():QFileSystemModel(){};
    Qt::DropActions supportedDropActions()const{
        return Qt::CopyAction | Qt::MoveAction;
    }
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
};

#endif // DRAGDROPFILESYSTEMMODEL_H
