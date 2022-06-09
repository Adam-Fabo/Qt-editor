/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: insertconstcialog.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of InsertConsDialog class methods
 *
 */


#include "insertconsdialog.h"
#include "ui_insertconsdialog.h"

InsertConsDialog::InsertConsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertConsDialog)
{
    ui->setupUi(this);
     setModal(true);
     ui->radioButton_int->setChecked(true);
}

InsertConsDialog::~InsertConsDialog()
{
    delete ui;
}



void InsertConsDialog::on_pushButton_OK_clicked()
{
    emit okPressed( ui->lineEdit->text());

    accept();
}

void InsertConsDialog::on_radioButton_int_clicked(bool checked)
{
    if(checked)
        emit constInt();
}

void InsertConsDialog::on_radioButton_float_clicked(bool checked)
{
    if(checked)
        emit constFloat();
}

void InsertConsDialog::on_radioButton_bool_clicked(bool checked)
{
    if(checked)
        emit constBool();
}

void InsertConsDialog::on_radioButton_string_clicked(bool checked)
{
    if(checked)
        emit constString();
}
