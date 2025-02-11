#include "sqleditorform.h"
#include "ui_sqleditorform.h"

SqlEditorForm::SqlEditorForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SqlEditorForm)
{
    ui->setupUi(this);
    createUI();
}

SqlEditorForm::~SqlEditorForm()
{
    delete ui;
}

void SqlEditorForm::createUI()
{
    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,1);

}
