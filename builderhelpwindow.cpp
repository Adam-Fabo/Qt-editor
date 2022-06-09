/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderhelpwindow.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of BuilderHelpWindow class methods
 *
 */

#include "builderhelpwindow.h"
#include "ui_builderhelpwindow.h"

BuilderHelpWindow::BuilderHelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BuilderHelpWindow)
{
    ui->setupUi(this);
}

BuilderHelpWindow::~BuilderHelpWindow()
{
    delete ui;
}
