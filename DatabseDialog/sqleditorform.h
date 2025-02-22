#ifndef SQLEDITORFORM_H
#define SQLEDITORFORM_H
#include "codeeditor.h"

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QCompleter>
#include <QStringListModel>

namespace Ui {
class SqlEditorForm;
}

class SqlEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit SqlEditorForm(QWidget *parent = nullptr);
    ~SqlEditorForm();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_tableViewHistory_doubleClicked(const QModelIndex &index);
    void on_pushButtonRunSQL_clicked();
    void insertCompletion(const QString &completion);

private:
    void createUI();
    void saveQueryHistory(const QString &queryText);
    void loadQueryHistory();
    void setupSqlCompleter();
    QString textUnderCursor() const;
private:
    Ui::SqlEditorForm *ui;
    CodeEditor *editor;
    QSqlDatabase db;            // База даних
    QSqlQueryModel *modelQuery;      // Модель для SELECT-запитів
    QSqlQueryModel *historyModel;   // Модель для історіі запитів
    QCompleter *sqlCompleter;


    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // SQLEDITORFORM_H
