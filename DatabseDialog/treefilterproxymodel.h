#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit TreeFilterProxyModel(QObject *parent = nullptr);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // TREEFILTERPROXYMODEL_H
