#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "version.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

