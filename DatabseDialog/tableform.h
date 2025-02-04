#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QWidget>

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
    QString tableName;                  //Ім'я таблиці
};
#endif // TABLEFORM_H
