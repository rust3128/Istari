#ifndef SQLEDITORFORM_H
#define SQLEDITORFORM_H

#include <QWidget>

namespace Ui {
class SqlEditorForm;
}

class SqlEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit SqlEditorForm(QWidget *parent = nullptr);
    ~SqlEditorForm();

private:
    void createUI();
private:
    Ui::SqlEditorForm *ui;
};

#endif // SQLEDITORFORM_H
