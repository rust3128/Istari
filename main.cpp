#include "mainwindow.h"
#include "version.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString versionInfo = QString("Version: %1.%2.%3\nBuild Date: %4")
                              .arg(PROJECT_VERSION_MAJOR)
                              .arg(PROJECT_VERSION_MINOR)
                              .arg(PROJECT_VERSION_PATCH)
                              .arg(PROJECT_BUILD_DATE);

    qDebug() << versionInfo;

    MainWindow w;
    w.show();
    return a.exec();
}
