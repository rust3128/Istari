#include "tableform.h"
#include "ui_tableform.h"

TableForm::TableForm(QString tblName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableForm)
    , tableName(tblName)
{
    ui->setupUi(this);
    createUI();
}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::createUI()
{
    ui->label->setText(tableName);
}
