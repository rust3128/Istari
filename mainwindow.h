#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DatabseDialog/databsedialog.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menu_action_About_Istari_triggered();

private:
    Ui::MainWindow *ui;
    DatabseDialog *dbDlg;
};
#endif // MAINWINDOW_H
