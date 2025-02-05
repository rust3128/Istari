#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>

namespace Ui {
class TableForm;
}

class TableForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableForm(QString tblName, QWidget *parent = nullptr);
    ~TableForm();
private:
    void createUI();
private:
    Ui::TableForm *ui;
    QSqlDatabase db;
    QString tableName;                  //Ім'я таблиці
    QSqlQueryModel *modelFields;        //Модель для відображення структури таблиці
    QSqlTableModel *modelData;          //Модель для відображення даних таблиці
};
#endif // TABLEFORM_H
