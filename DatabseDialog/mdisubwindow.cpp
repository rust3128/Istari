#include "mdisubwindow.h"

MdiSubWindow::MdiSubWindow(QWidget *parent)
    : QMdiSubWindow(parent) {}

bool MdiSubWindow::event(QEvent *event) {
    if (event->type() == QEvent::Close) {
        emit aboutToClose(); // Викликаємо сигнал перед закриттям
    }
    return QMdiSubWindow::event(event);
}
