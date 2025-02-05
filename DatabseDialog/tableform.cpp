#include "tableform.h"
#include "ui_tableform.h"

TableForm::TableForm(QString tblName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableForm)
    , tableName(tblName)
{
    ui->setupUi(this);
    createUI();
}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::createUI()
{
    ui->tabWidget->setCurrentIndex(0);
    db = QSqlDatabase::database("dbObj");
    ui->label->setText("Таблиця: " + tableName);

    // Вкладка Поля
    modelFields = new QSqlQueryModel(this);
    QString strSQL = QString(R"(
        SELECT
            TRIM(rf.RDB$FIELD_NAME) AS FIELD_NAME,
            TRIM(
                CASE f.RDB$FIELD_TYPE
                    WHEN 7 THEN 'SMALLINT'
                    WHEN 8 THEN 'INTEGER'
                    WHEN 9 THEN 'QUAD'
                    WHEN 10 THEN 'FLOAT'
                    WHEN 11 THEN 'D_FLOAT'
                    WHEN 12 THEN 'DATE'
                    WHEN 13 THEN 'TIME'
                    WHEN 14 THEN 'CHAR'
                    WHEN 16 THEN 'INT64'
                    WHEN 27 THEN 'DOUBLE'
                    WHEN 35 THEN 'TIMESTAMP'
                    WHEN 37 THEN 'VARCHAR'
                    WHEN 40 THEN 'CSTRING'
                    WHEN 45 THEN 'BLOB_ID'
                    WHEN 261 THEN 'BLOB'
                    ELSE 'UNKNOWN'
                END
            ) AS FIELD_TYPE,
            CASE
                WHEN f.RDB$FIELD_TYPE IN (14, 37) THEN f.RDB$FIELD_LENGTH
                ELSE ''
            END AS FIELD_LENGTH,
            TRIM(rf.RDB$FIELD_SOURCE) AS DOMAIN_NAME,
            COALESCE(TRIM(ix.INDEX_NAMES), 'NO') AS INDEX_NAMES
        FROM
            RDB$RELATION_FIELDS rf
        JOIN
            RDB$FIELDS f ON rf.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME
        LEFT JOIN (
            SELECT
                s.RDB$FIELD_NAME,
                LIST(TRIM(s.RDB$INDEX_NAME), ', ') AS INDEX_NAMES
            FROM
                RDB$INDEX_SEGMENTS s
            JOIN
                RDB$INDICES i ON s.RDB$INDEX_NAME = i.RDB$INDEX_NAME
            WHERE
                i.RDB$RELATION_NAME = '%1'
            GROUP BY
                s.RDB$FIELD_NAME
        ) ix ON rf.RDB$FIELD_NAME = ix.RDB$FIELD_NAME
        WHERE
            rf.RDB$RELATION_NAME = '%1'
        ORDER BY
            rf.RDB$FIELD_POSITION
    )").arg(tableName);

    modelFields->setQuery(strSQL, db);

    // Зміна назв стовбців
    modelFields->setHeaderData(0, Qt::Horizontal, "Назва поля");
    modelFields->setHeaderData(1, Qt::Horizontal, "Тип поля");
    modelFields->setHeaderData(2, Qt::Horizontal, "Довжина поля");
    modelFields->setHeaderData(3, Qt::Horizontal, "Назва домену");
    modelFields->setHeaderData(4, Qt::Horizontal, "Назви індексів");

    ui->tableViewFilds->setModel(modelFields);

    // Автоматичне налаштування ширини стовбців
    ui->tableViewFilds->resizeColumnsToContents();

    //Вкладка дані
    modelData = new QSqlTableModel(this, db);
    modelData->setTable(tableName);
    modelData->select();

    // Встановлення моделі для tableViewData
    ui->tableViewData->setModel(modelData);

    // Автоматичне налаштування ширини стовбців
    ui->tableViewData->resizeColumnsToContents();
}
