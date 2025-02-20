#include "sqleditorform.h"
#include "ui_sqleditorform.h"
#include "sqlsyntaxhighlighter.h"
#include "../AppParams/loggincategories.h"
#include "../AppParams/appparams.h"

#include <QScrollBar>

SqlEditorForm::SqlEditorForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SqlEditorForm)
{
    ui->setupUi(this);

    db = QSqlDatabase::database("dbObj");
    modelQuery = new QSqlQueryModel(this);
    historyModel = new QSqlQueryModel(this);
    createUI();
}

SqlEditorForm::~SqlEditorForm()
{
    delete ui;
}

void SqlEditorForm::createUI()
{
    qInfo(logInfo()) << "CreateUI";
    new SqlSyntaxHighlighter(ui->plainTextEditSQL->document());
    // Налаштовуємо автодоповнення
    setupSqlCompleter();

    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,1);
    ui->pushButtonRunSQL->setShortcut(QKeySequence("F9"));
    ui->tabWidget->setCurrentIndex(0);

    int index = ui->tabWidget->indexOf(ui->tabResult);
    if (index != -1) {
        ui->tabWidget->setTabEnabled(index, false);
    }

    ui->tableViewResultSQL->setModel(modelQuery);
    ui->tableViewHistory->setModel(historyModel);

}

void SqlEditorForm::saveQueryHistory(const QString &queryText)
{
    QSqlQuery query;
    query.prepare("INSERT INTO QUERY_HISTORY (QUERY_TEXT, USER_ID) VALUES (?, ?)");
    query.addBindValue(queryText);
    query.addBindValue(AppParams::instance().getParameter("userID").toInt());

    if (!query.exec()) {
        qWarning() << "Помилка запису в історію: " << query.lastError().text();
    }
}



void SqlEditorForm::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->widget(index) == ui->tabHistory) {
        loadQueryHistory();
    }
}

void SqlEditorForm::loadQueryHistory() {
    historyModel->setQuery(
        "SELECT QUERY_TEXT, EXEC_TIME, u.user_fio "
        "FROM QUERY_HISTORY h "
        "LEFT JOIN users u ON u.user_id = h.user_ID "
        "ORDER BY EXEC_TIME DESC");

    if (historyModel->lastError().isValid()) {
        qWarning() << "Помилка завантаження історії: " << historyModel->lastError().text();
        return;
    }

    // Встановлення заголовків
    historyModel->setHeaderData(0, Qt::Horizontal, "Текст запиту");
    historyModel->setHeaderData(1, Qt::Horizontal, "Час");
    historyModel->setHeaderData(2, Qt::Horizontal, "Користувач");

    // Прив'язуємо модель до tableView
    ui->tableViewHistory->setModel(historyModel);

    // Налаштування розміру колонок
    ui->tableViewHistory->setColumnWidth(0, 300); // "Текст запиту" – 100 пікселів
    ui->tableViewHistory->resizeColumnToContents(1); // "Час" – автоматичне визначення ширини
    ui->tableViewHistory->resizeColumnToContents(2); // "Користувач" – автоматичне визначення ширини
}



void SqlEditorForm::on_tableViewHistory_doubleClicked(const QModelIndex &index)
{
    QString historySQL = index.sibling(index.row(),0).data().toString();
    ui->plainTextEditSQL->clear();
    ui->plainTextEditSQL->setPlainText(historySQL);

    int tabIndex = ui->tabWidget->indexOf(ui->tabEdit);
    if (tabIndex != -1) {
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
}


void SqlEditorForm::on_pushButtonRunSQL_clicked()
{
    QString sqlQuery = ui->plainTextEditSQL->toPlainText().trimmed();
    if (sqlQuery.isEmpty()) {
        ui->textEditMessage->setText("Запит порожній!");
        return;
    }

    QSqlQuery query(db);
    bool success = true;
    modelQuery->clear();

    if (sqlQuery.toUpper().startsWith("SELECT")) {
        // Виконуємо SELECT-запит
        modelQuery->setQuery(sqlQuery, db);

        if (modelQuery->lastError().isValid()) {
            ui->textEditMessage->setText("Помилка: " + modelQuery->lastError().text());
            success = false;
        } else {
            ui->textEditMessage->clear(); // Очищаємо повідомлення про помилки
            ui->tableViewResultSQL->resizeColumnsToContents();
            // Переключаємо вкладку на tabResult
            int index = ui->tabWidget->indexOf(ui->tabResult);
            if (index != -1) {
                ui->tabWidget->setTabEnabled(index, true);
                ui->tabWidget->setCurrentIndex(index);
            }
        }
    } else {
        ui->textEditMessage->setText("На поточний час доступне виконня тільки SELECT запитів.");
        // // Виконуємо будь-який інший запит (INSERT, UPDATE, DELETE, тощо)
        // if (!query.exec(sqlQuery)) {
        success = false;
        //     ui->textEditMessage->setText("Помилка: " + query.lastError().text());
        // } else {
        //     ui->textEditMessage->setText("Запит виконано успішно.");
        // }
    }
    // Зберігаємо тільки успішні запити
    if (success) {
        saveQueryHistory(sqlQuery);
    }
}

void SqlEditorForm::setupSqlCompleter()
{
    QStringList sqlKeywords = {
        "SELECT", "FROM", "WHERE", "INSERT", "UPDATE", "DELETE",
        "JOIN", "LEFT JOIN", "RIGHT JOIN", "INNER JOIN", "ORDER BY",
        "GROUP BY", "HAVING", "LIMIT", "OFFSET", "CREATE TABLE",
        "ALTER TABLE", "DROP TABLE", "PRIMARY KEY", "FOREIGN KEY",
        "NOT NULL", "DEFAULT", "UNIQUE", "CHECK", "INDEX"
    };

    sqlCompleter = new QCompleter(sqlKeywords, this);
    sqlCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    sqlCompleter->setCompletionMode(QCompleter::PopupCompletion);
    sqlCompleter->setWidget(ui->plainTextEditSQL);

    connect(sqlCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}


void SqlEditorForm::insertCompletion(const QString &completion)
{
    QTextCursor tc = ui->plainTextEditSQL->textCursor();
    tc.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    tc.insertText(completion);
    ui->plainTextEditSQL->setTextCursor(tc);
}



void SqlEditorForm::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e); // Викликаємо стандартну обробку

    // Ігноруємо клавіші, які не впливають на текст
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter ||
        e->key() == Qt::Key_Escape || e->key() == Qt::Key_Tab) {
        return;
    }

    // Отримуємо поточне слово під курсором
    QString prefix = textUnderCursor();
    if (prefix.isEmpty()) {
        sqlCompleter->popup()->hide();
        return;
    }

    // Передаємо в `QCompleter`
    sqlCompleter->setCompletionPrefix(prefix);
    QRect cr = ui->plainTextEditSQL->cursorRect();
    cr.setWidth(sqlCompleter->popup()->sizeHintForColumn(0) + 20);
    sqlCompleter->complete(cr);
}

QString SqlEditorForm::textUnderCursor() const
{
    QTextCursor tc = ui->plainTextEditSQL->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
