#include "databsedialog.h"
#include "ui_databsedialog.h"
#include "../AppParams/loggincategories.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTreeWidgetItem>

DatabseDialog::DatabseDialog(DatabaseConfig dbC, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabseDialog)
    , dbAZS(dbC)
{
    ui->setupUi(this);
    connectToDatabase();
    createModel();
    createUI();
}

DatabseDialog::~DatabseDialog()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}


//Підключення до бази даних
void DatabseDialog::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QIBASE","dbObj");
    db.setHostName(dbAZS.getHostName());
    db.setPort(dbAZS.getPort());
    db.setDatabaseName(dbAZS.getDatabaseName());
    db.setUserName(dbAZS.getUserName());
    db.setPassword(dbAZS.getPassword());
    if(db.open()){
        qInfo(logInfo()) << "Успішне підключення до бази даних об'єкта.";
        isConnected = true;
    } else {
        isConnected = false;
        QString errMsg = db.lastError().text();
        qCritical(logCritical()) << "Помилка відкриття бази даних додатка:" << errMsg;
        MyMessage::showNotification(QMessageBox::Critical,"Критична помилка","Не можливо відкрити базу даних об'єкта!","Перевірте параметри підключення до бази даних.",errMsg);
    }
}
//Відображення інтерфейсу форми
void DatabseDialog::createUI()
{
    QString strStatus;
    if(isConnected){
        ui->labelStatusLogo->setPixmap(QPixmap(":/Resources/Images/openDatabase.png"));
        ui->labelStatusText->setStyleSheet("QLabel { color : green; }");
        strStatus = QString("Успішне підключення.");
    } else {
        ui->labelStatusLogo->setPixmap(QPixmap(":/Resources/Images/notOpenDatabase.png"));
        ui->labelStatusText->setStyleSheet("QLabel { color : red; }");
        strStatus = QString("Не вдалося підключитися.");
    }
    strStatus += "\n"+dbAZS.getHostName()+":"+QString::number(dbAZS.getPort())+" "+dbAZS.getDatabaseName();
    ui->labelStatusText->setText(strStatus);

    // Встановлення заголовка для стовпця моделі (необов'язково)
    modelDB->setHorizontalHeaderLabels({"Database Structure"});
    // Проксі модель для фільтрації
    proxyModel = new TreeFilterProxyModel(this);
    proxyModel->setSourceModel(modelDB);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(0); // Фільтрувати за першим стовпцем

    // Прив'язка проксі моделі до QTreeView
    ui->treeViewDatabase->setModel(proxyModel);

    // Підключення сигналу textChanged до слота filterTree
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &DatabseDialog::filterTree);

}

void DatabseDialog::createModel()
{
    modelDB = new QStandardItemModel(this);

    if (isConnected) {
        QStringList tables = db.tables(QSql::Tables);
        if (!tables.isEmpty()) {
            // Сортування списку таблиць за алфавітом
            std::sort(tables.begin(), tables.end());

            QStandardItem *tablesItem = new QStandardItem("Tables");
            modelDB->appendRow(tablesItem);

            for (const QString &tableName : tables) {
                QStandardItem *tableItem = new QStandardItem(tableName);
                tablesItem->appendRow(tableItem);
            }
        }
        // Отримання та сортування списку генераторів
        QStringList generators;
        QSqlQuery query("SELECT RDB$GENERATOR_NAME FROM RDB$GENERATORS WHERE RDB$SYSTEM_FLAG = 0", db);
        while (query.next()) {
            generators << query.value(0).toString().trimmed();
        }
        std::sort(generators.begin(), generators.end());

        // Додавання генераторів до моделі
        if (!generators.isEmpty()) {
            QStandardItem *generatorsItem = new QStandardItem("Generators");
            modelDB->appendRow(generatorsItem);
            for (const QString &generatorName : generators) {
                QStandardItem *generatorItem = new QStandardItem(generatorName);
                generatorsItem->appendRow(generatorItem);
            }
        }
    }


}

// Слот для фільтрації дерева
void DatabseDialog::filterTree(const QString &text)
{
    // Створення регулярного виразу для фільтрації з початку рядка
    QRegularExpression regex("^" + QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);

    // Розгортання всіх елементів дерева, які відповідають фільтру
    expandAllMatchingRows();
}



void DatabseDialog::expandAllMatchingRows()
{
    // Отримуємо кількість рядків у кореневому індексі
    int rowCount = proxyModel->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QModelIndex index = proxyModel->index(i, 0);
        expandRecursively(index);
    }
}

void DatabseDialog::expandRecursively(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    ui->treeViewDatabase->expand(index);

    int rowCount = proxyModel->rowCount(index);
    for (int i = 0; i < rowCount; ++i) {
        QModelIndex childIndex = proxyModel->index(i, 0, index);
        expandRecursively(childIndex);
    }
}

