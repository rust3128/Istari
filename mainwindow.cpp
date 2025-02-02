#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AppParams/databaseconfig.h"
#include "version.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Зажаемо параметри підключення до БД АЗС
    DatabaseConfig dbAZS("localhost",3050,"D:/Develop/DataBase/prolux1001.GDB","SYSDBA","masterkey");

    dbDlg = new DatabseDialog(dbAZS);
    setCentralWidget(dbDlg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menu_action_About_Istari_triggered()
{
    QString versionInfo = QString("Version: %1.%2.%3\nBuild Date: %4")
    .arg(PROJECT_VERSION_MAJOR)
        .arg(PROJECT_VERSION_MINOR)
        .arg(PROJECT_VERSION_PATCH)
        .arg(PROJECT_BUILD_DATE);
    QMessageBox::about(this, "Про Istari", versionInfo);
}

