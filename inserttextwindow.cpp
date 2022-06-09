/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: inserttextwindow.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of InsertTextWindow class methods
 *
 */


#include "inserttextwindow.h"
#include "ui_inserttextwindow.h"

InsertTextWindow::InsertTextWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTextWindow)
{
    ui->setupUi(this);
    setModal(true);
}

InsertTextWindow::~InsertTextWindow()
{
    delete ui;
}


void InsertTextWindow::on_pushButton_clicked(bool checked)
{
    emit okPressed( ui->lineEdit->text());
    accept();
}
