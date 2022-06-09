/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderaddpartdial.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of BuilderAddPartDial class methods
 *
 */


#include "builderaddpartdial.h"
#include "ui_builderaddpartdial.h"

BuilderAddPartDial::BuilderAddPartDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuilderAddPartDial)
{
    ui->setupUi(this);
    setModal(true);
    model = new AppModel();

    connect(model,SIGNAL(send_filemodel(QFileSystemModel *)),this,SLOT(setDirecoryStruct(QFileSystemModel *)));
    connect(model,SIGNAL(block_file_read(QStringList,QStringList,QStringList)),this,SLOT(treeViewSlot(QStringList,QStringList,QStringList)));
    connect(model,SIGNAL(warn_saving(QString)),this,SLOT(warning(QString) ));


    model->filesystemmodel();

    setupTreeWiew();
}

BuilderAddPartDial::~BuilderAddPartDial()
{
    delete ui;
    delete model;
}


void BuilderAddPartDial::setupTreeWiew()
{
    ui->treeView->setModel(dirStruct);
    ui->treeView->setRootIndex(dirStruct->index(dirStruct->rootPath()));
}


void BuilderAddPartDial::on_cancelButton_clicked()
{
    reject();
}


void BuilderAddPartDial::on_okButton_clicked()
{

    model->open_block(ui->treeView->currentIndex());

    if(valid_selection){

        emit blockSelected(cat_name_desc_code, inputs, outputs);
        accept();
    }
    return;

}

void BuilderAddPartDial::treeViewSlot(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param)
{
    valid_selection = true;

    cat_name_desc_code = cat_name_desc_code_param;
    inputs = inputs_param;
    outputs = outputs_param;

}

void BuilderAddPartDial::warning(QString s)
{
    valid_selection = false;
}
