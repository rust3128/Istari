#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QEvent>

class MdiSubWindow : public QMdiSubWindow {
    Q_OBJECT
public:
    explicit MdiSubWindow(QWidget *parent = nullptr);

signals:
    void aboutToClose(); // Сигнал перед закриттям вікна

protected:
    bool event(QEvent *event) override;
};

#endif // MDISUBWINDOW_H

