#include "treefilterproxymodel.h"

TreeFilterProxyModel::TreeFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool TreeFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);

    // Перевірити, чи поточний елемент відповідає фільтру
    if (sourceModel()->data(index).toString().contains(filterRegularExpression())) {
        return true;
    }

    // Перевірити дочірні елементи
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index)) {
            return true;
        }
    }

    // Перевірити батьківський елемент
    QModelIndex parent = source_parent;
    while (parent.isValid()) {
        if (sourceModel()->data(parent).toString().contains(filterRegularExpression())) {
            return true;
        }
        parent = parent.parent();
    }

    // Якщо ні поточний елемент, ні його дочірні елементи, ні батьківський елемент не відповідають фільтру
    return false;
}
