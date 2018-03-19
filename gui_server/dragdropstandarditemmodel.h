#ifndef DRAGDROPSTANDARDITEMMODEL_H
#define DRAGDROPSTANDARDITEMMODEL_H

#include <QAbstractListModel>
#include <QStandardItemModel>
#include <Qt>
class DragDropStandardItemModel : public QStandardItemModel
{
    public:
    using QStandardItemModel::QStandardItemModel;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions()const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
};

#endif // DRAGDROPSTANDARDITEMMODEL_H
