#ifndef DATABSEDIALOG_H
#define DATABSEDIALOG_H

#include "../AppParams/databaseconfig.h"
#include "treefilterproxymodel.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace Ui {
class DatabseDialog;
}

class DatabseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabseDialog(DatabaseConfig dbC, QWidget *parent = nullptr);
    ~DatabseDialog();
private slots:
    void filterTree(const QString &text);               // Слот для фільтрації дерева
private:
    void connectToDatabase();                           //Підключення до бази даних
    void createUI();                                    //Відображення інтерфейсу форми
    void createModel();                                 //Створення моделі
    void expandAllMatchingRows();                       // Розгортання всіх елементів дерева, які відповідають фільтру
    void expandRecursively(const QModelIndex &index);   //Рекурсивно розгортає всі дочірні індекси для даного індексу
private:
    Ui::DatabseDialog *ui;
    DatabaseConfig dbAZS;                           //Параметри підключення до БД
    QSqlDatabase db;                                //База даних об'єкту
    bool isConnected;                               //Чи вдалось підключитися до бази даних
    QStandardItemModel *modelDB;                    //Модель структури бази даних
    TreeFilterProxyModel *proxyModel;              // Проксі модель для фільтрації
};

#endif // DATABSEDIALOG_H
