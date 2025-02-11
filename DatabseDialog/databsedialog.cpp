#include "databsedialog.h"
#include "DatabseDialog/ui_databsedialog.h"
#include "ui_databsedialog.h"
#include "../AppParams/loggincategories.h"
#include "../AppParams/appparams.h"
#include "tableform.h"
#include "sqleditorform.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTreeWidgetItem>
#include <QMdiSubWindow>
#include <QFileInfo>

DatabseDialog::DatabseDialog(DatabaseConfig dbC, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabseDialog)
    , dbAZS(dbC)
{
    ui->setupUi(this);
    // Збереження початкового кольору тексту
    originalTextColor = ui->lineEditFilter->palette().color(QPalette::Text);
    connectToDatabase();
    createModel();
    createUI();
    // Перевірка назви виконуваного файлу
    QFileInfo exeFile(QCoreApplication::applicationFilePath());
    if (exeFile.fileName() != "Istari.exe") {
        ui->toolButtonFullScreen->show();
    }
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
        emit noConnDB();
    }
}
//Відображення інтерфейсу форми
void DatabseDialog::createUI()
{
    ui->toolButtonFullScreen->hide();
    ui->toolButtonSQLEditor->setShortcut(QKeySequence("F12"));
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
    strStatus = dbAZS.getHostName()+"/"+QString::number(dbAZS.getPort())+":"+dbAZS.getDatabaseName() + " " + strStatus;
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

    // Підключення сигналу textChanged до слота filterTree та highlightNonLatinCharacters
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &DatabseDialog::filterTree);
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &DatabseDialog::highlightNonLatinCharacters);

}

void DatabseDialog::createModel()
{
    modelDB = new QStandardItemModel(this);

    if (isConnected) {
        QStringList tables = db.tables(QSql::Tables);
        if (!tables.isEmpty()) {
            // Сортування списку таблиць за алфавітом
            std::sort(tables.begin(), tables.end());

            QStandardItem *tablesItem = new QStandardItem(AppParams::instance().DATABASE_ITEM_HEADS.value(1));
            modelDB->appendRow(tablesItem);

            for (const QString &tableName : tables) {
                QStandardItem *tableItem = new QStandardItem(tableName);
                tablesItem->appendRow(tableItem);
            }
        }
        // // Отримання та сортування списку генераторів
        // QStringList generators;
        // QSqlQuery query("SELECT RDB$GENERATOR_NAME FROM RDB$GENERATORS WHERE RDB$SYSTEM_FLAG = 0", db);
        // while (query.next()) {
        //     generators << query.value(0).toString().trimmed();
        // }
        // std::sort(generators.begin(), generators.end());

        // // Додавання генераторів до моделі
        // if (!generators.isEmpty()) {
        //     QStandardItem *generatorsItem = new QStandardItem(AppParams::instance().DATABASE_ITEM_HEADS.value(2));
        //     modelDB->appendRow(generatorsItem);
        //     for (const QString &generatorName : generators) {
        //         QStandardItem *generatorItem = new QStandardItem(generatorName);
        //         generatorsItem->appendRow(generatorItem);
        //     }
        // }
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

void DatabseDialog::highlightNonLatinCharacters(const QString &text)
{
    QPalette palette = ui->lineEditFilter->palette();

    // Перевірка на наявність нелатинських символів
    QRegularExpression nonLatinRegex("[^a-zA-Z]");
    QRegularExpressionMatch match = nonLatinRegex.match(text);

    if (match.hasMatch()) {
        // Якщо знайдені нелатинські символи, змінюємо колір тексту на червоний
        palette.setColor(QPalette::Text, Qt::red);
    } else {
        // Якщо всі символи латинські, змінюємо колір тексту на початковий
        palette.setColor(QPalette::Text, originalTextColor);
    }

    ui->lineEditFilter->setPalette(palette);
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


void DatabseDialog::on_treeViewDatabase_doubleClicked(const QModelIndex &index)
{
    // Перевіряємо, чи індекс валідний
    if (!index.isValid())
        return;

    // Отримуємо назву елемента
    QString itemName = index.data(Qt::DisplayRole).toString();

    // Отримуємо батьківський індекс
    QModelIndex parentIndex = index.parent();

    // Перевіряємо, чи батьківський індекс валідний
    QString parentName;
    if (parentIndex.isValid())
    {
        parentName = parentIndex.data(Qt::DisplayRole).toString();
    }
    else
    {
        parentName = "Root"; // або інше значення, якщо індекс не має батька
        return;
    }

    // Виводимо назву елемента та його батьківського елемента
    // qDebug() << "Double-clicked item name:" << itemName;
    // qDebug() << "Parent item name:" << parentName;

    // Перевіряємо, чи підвікно з такою назвою вже існує
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        if (window->windowTitle() == itemName)
        {
            // Якщо підвікно вже існує, активуємо його
            ui->mdiArea->setActiveSubWindow(window);
            return;
        }
    }

    // Якщо підвікно не існує, створюємо нове підвікно
    QMdiSubWindow *subWindow = new QMdiSubWindow;

    TableForm *tblWidget = new TableForm (itemName, subWindow);

    subWindow->setWidget(tblWidget);

    // Встановлюємо заголовок підвікна
    subWindow->setWindowTitle(itemName);

    // Додаємо підвікно до mdiArea
    ui->mdiArea->addSubWindow(subWindow);

    // Відображаємо підвікно
    subWindow->show();
}


void DatabseDialog::on_toolButtonFullScreen_clicked()
{
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}


void DatabseDialog::on_toolButtonSQLEditor_clicked()
{
    QMdiSubWindow *subWindow = new QMdiSubWindow;

    SqlEditorForm *sqlEdit = new SqlEditorForm(subWindow);
    subWindow->setWidget(sqlEdit);
     // Встановлюємо заголовок підвікна
     subWindow->setWindowTitle("SQL Editor");
    // Додаємо підвікно до mdiArea
    ui->mdiArea->addSubWindow(subWindow);

    // Відображаємо підвікно
    subWindow->show();
}

