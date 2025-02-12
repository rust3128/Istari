#include "sqleditorform.h"
#include "ui_sqleditorform.h"
#include "sqlsyntaxhighlighter.h"
#include "../AppParams/loggincategories.h"
#include "../AppParams/appparams.h"


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
    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,1);
    ui->toolButtonRunSQL->setShortcut(QKeySequence("F9"));
    ui->tabWidget->setCurrentIndex(0);

    int index = ui->tabWidget->indexOf(ui->tabResult);
    if (index != -1) {
        ui->tabWidget->setTabEnabled(index, false);
    }

    ui->tableViewResultSQL->setModel(modelQuery);
    ui->tableViewHistory->setModel(historyModel);

}



void SqlEditorForm::on_toolButtonRunSQL_clicked()
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
        //      success = false;
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
