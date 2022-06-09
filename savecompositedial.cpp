/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: savecompositedial.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description:  file contains definition of SaveCompositeDial class methods
 *
 */


#include "savecompositedial.h"
#include "ui_savecompositedial.h"

SaveCompositeDial::SaveCompositeDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveCompositeDial)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle("Save Composite Block");
    model = new AppModel();
    connect(model,SIGNAL(send_composite_categories(QStringList)),this,SLOT(setCategories(QStringList)));

    //ziskanie kategorii
    model->check_composite_categories("");
}


SaveCompositeDial::~SaveCompositeDial()
{
    delete model;
    delete ui;

}


void SaveCompositeDial::on_okButton_clicked()
{
    emit saveCompositeSignal(ui->nameLineEdit->text(),ui->categorycomboBox->currentText(),ui->descLineEdit->text());
    accept();
}


void SaveCompositeDial::on_cancelButton_clicked()
{
    reject();
}


void SaveCompositeDial::on_addCategoryButton_clicked()
{
    bool ok;
    //ziskanie mena
    QString text = QInputDialog::getText(this, tr("Category name"),
                                         tr("Category name:"), QLineEdit::Normal,
                                         "", &ok);
    model->check_composite_categories(text);
    ui->categorycomboBox->setCurrentText(text);
}


void SaveCompositeDial::setCategories(QStringList list)
{
    ui->categorycomboBox->clear();
    ui->categorycomboBox->addItems(list);
    ui->categorycomboBox->setCurrentText("----None----");
}
