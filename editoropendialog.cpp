/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: editoropendialog.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of EditorOpenDialog class, file was generated through Qt but filled by hand.
 *
 */

#include "editoropendialog.h"
#include "ui_editoropendialog.h"
#include <QDebug>

EditorOpenDialog::EditorOpenDialog(QWidget *parent, QFileSystemModel *model) :
    QDialog(parent),
    ui(new Ui::EditorOpenDialog)
{
    ui->setupUi(this);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(model->rootPath() + "/basicblocks"));


    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(choose(QModelIndex)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addcategory()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(remove()));

}

EditorOpenDialog::~EditorOpenDialog()
{
    delete ui;
}


void EditorOpenDialog::choose(QModelIndex index)
{
    if(index.parent().operator==(ui->treeView->rootIndex())){
        //qDebug() << "Is category";
        return;
    }

    emit opening_block(index);
    done(0);
}

void EditorOpenDialog::addcategory()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Action"),tr("Category name:"), QLineEdit::Normal,0,&ok);

    if (ok && !text.isEmpty()){
        emit add_category(text);
    }

}

void EditorOpenDialog::remove()
{
    if(ui->treeView->currentIndex().parent().operator==(ui->treeView->rootIndex())){
        QMessageBox popup;
        popup.setText("Removing a category removes all of its content!");
        popup.setInformativeText("Proceed anyway?");
        popup.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        popup.setDefaultButton(QMessageBox::Ok);
        popup.setIcon(QMessageBox::Warning);
        int ret = popup.exec();
        switch(ret){
        case QMessageBox::Ok:
            emit remove_category_or_block(ui->treeView->currentIndex(), "cat");
            break;
        default: //cancel
            break;
        }

        return;
    }else{
        QMessageBox popup;
        popup.setText("Are you sure you wish to delete this block?");
        popup.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        popup.setDefaultButton(QMessageBox::Ok);
        popup.setIcon(QMessageBox::Question);
        int ret = popup.exec();
        switch(ret){
        case QMessageBox::Ok:
            emit remove_category_or_block(ui->treeView->currentIndex(), "block");
            break;
        default: //cancel
            break;
        }
        //qDebug() << ui->treeView->currentIndex().sibling(ui->treeView->currentIndex().row(),0).data().toString(); //gives file/folder name everytime
    }

}

