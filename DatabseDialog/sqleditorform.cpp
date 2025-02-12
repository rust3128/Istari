#include "sqleditorform.h"
#include "ui_sqleditorform.h"
#include "sqlsyntaxhighlighter.h"
#include "../AppParams/loggincategories.h"


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
    new SqlSyntaxHighlighter(ui->plainTextEditSQL->document());
    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,1);
    ui->toolButtonRunSQL->setShortcut(QKeySequence("F9"));
    ui->tabWidget->setCurrentIndex(0);

    int index = ui->tabWidget->indexOf(ui->tabResult);
    if (index != -1) {
        ui->tabWidget->setTabEnabled(index, false);
    }

}

void SqlEditorForm::on_toolButtonRunSQL_clicked()
{
    qInfo(logInfo()) << "Run SQL";
    int index = ui->tabWidget->indexOf(ui->tabResult);
    if (index != -1) {
        ui->tabWidget->setTabEnabled(index, true);
        ui->tabWidget->setCurrentIndex(index);
    }
}

