/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: editorwindow.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of EditorWindow class, file was generated through Qt but filled by hand.
 *
 */

#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QDebug>
#include <QDir>
#include <QRegularExpression>


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    ui->comboBox->view()->setMinimumWidth(115);
    ui->textEdit->append("1:   ");
    ui->textEdit_2->append("1:   ");
    //ui->comboBox->addItem("----None----");

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(add_category()));

    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(saving(bool)));
    connect(ui->actionBlank_Block,SIGNAL(triggered(bool)),this,SLOT(blank_block(bool)));
    connect(ui->actionHelp,SIGNAL(triggered(bool)),this,SLOT(help_popup(bool)));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SIGNAL(get_filesystemmodel()));
    //inputs = new QStringList();
}

EditorWindow::~EditorWindow()
{
    delete ui;
}




void EditorWindow::closeEvent(QCloseEvent *e)
{
    emit close_editor();
    QMainWindow::closeEvent(e); //pass event to base
}


void EditorWindow::add_category()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Action"),tr("Category name:"), QLineEdit::Normal,0,&ok);

    if (ok && !text.isEmpty()){
        emit get_categories(text);
    }
}

void EditorWindow::update_combobox(QStringList list)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
    ui->comboBox->setCurrentText("----None----");
}

void EditorWindow::warn_cat_exists()
{

    QMessageBox warning;
    warning.setWindowTitle("Warning");
    warning.setText("This category already exists!");
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
    finished_saving(false);
}

void EditorWindow::warn_block_problem(QString text)
{
    QMessageBox warning;
    warning.setWindowTitle("Warning");
    warning.setText(text);
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
    finished_saving(false);
}

QStringList EditorWindow::format_inout(QString in_or_out, int &info)
{
    QStringList type_and_name;
    info = 0;
    TextEditNewline* tmp;
    if(in_or_out == "in"){
        tmp = ui->textEdit; //input
    }else{
        tmp = ui->textEdit_2;   //output
    }

    QString text = tmp->toPlainText();
    QStringList list = text.split("\n");
    QRegularExpression regex("^(1:)*[ ]*$");  //no input/output
    QRegularExpressionMatch match = regex.match(text);
    if(match.hasMatch()){
        return type_and_name;
    }

    QStringList par_split_comma;
    QStringList par_split_space;
    QRegularExpression regex2("^([0-9]+:)?[ \n\t]*$");  //ne
    QRegularExpressionMatch match2;
    for(const auto& par : list){
        match2 = regex2.match(par);
        if(match2.hasMatch()){
            continue;
        }
        if(par.indexOf(',') != par.lastIndexOf(',') || par.indexOf(',') == -1){  //more than 1 comma or no comma at all
            //qDebug() << par;
            warn_block_problem("Incorrect format of Inputs/Outputs!");
            info = 1;
            return type_and_name;
        }
        par_split_comma = par.split(",");
        for(const auto& iter : par_split_comma){
            par_split_space = iter.split(" ");
            par_split_space.removeAll("");
            if(par_split_space.count() > 3){
                warn_block_problem("Incorrect format of Inputs/Outputs!");
                info = 1;
                return type_and_name;
            }
            if(par_split_space.count() == 3){
                par_split_space.removeAt(0);
            }
            if(in_or_out != "in"){
                type_and_name << par_split_space.join(" &");
            }else{
                type_and_name << par_split_space.join(" ");
            }
           // qDebug() << type_and_name;
        }
    }
    return type_and_name;
}


void EditorWindow::saving(bool checked)
{
    ui->statusbar->showMessage("Saving...");
    if(checked){}; //doesn't do anything
    QStringList list;
    list << ui->comboBox->currentText();  //get chosen category

    QString name = ui->lineEdit->text();
    if(name == ""){
        warn_block_problem("Missing Block name!");
        return;
    }
    if(name.contains(' ')){
        warn_block_problem("Whitespace in name not allowed!");
        return;
    }
    list << name;

    bool ok;
    QString description = QInputDialog::getText(this, tr("Action"),tr("[Optional] Add a short description:"), QLineEdit::Normal,0,&ok);
    if(!ok){
        finished_saving(false);
        return;
    }
    list << description;

    list << ui->textEdit_3->toPlainText();

    int i;
    QStringList inputs = format_inout("in", i);
    if(i == 1){
        return;
    }
    QStringList outputs = format_inout("out", i);
    if(i == 1){
        return;
    }
    emit(save_block_signal(list,inputs,outputs));

    //qDebug() << inputs;
    //qDebug() << outputs;
}

void EditorWindow::finished_saving(bool succes){
    ui->statusbar->clearMessage();
    if(succes){
        ui->statusbar->showMessage("Saved succesfully!",2000);
    }
}

int EditorWindow::popup_blank()
{
    QMessageBox popup;
    popup.setText("Do you wish to save first?");
    popup.setInformativeText("Not doing so will discard all changes!");
    popup.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    popup.setDefaultButton(QMessageBox::Save);
    popup.setIcon(QMessageBox::Question);
    int ret = popup.exec();
    return ret;
}

void EditorWindow::blank_block(bool checked)
{
    if(checked){}; //doesn't do anything
    int optn = popup_blank();
    switch(optn){
        case QMessageBox::Save:
            emit ui->actionSave->triggered(false);
            break;
        case QMessageBox::Discard:
            ui->lineEdit->setText("");
            ui->textEdit->setPlainText("");
            ui->textEdit->append("1:   ");
            ui->textEdit_2->setPlainText("");
            ui->textEdit_2->append("1:   ");
            ui->textEdit_3->setPlainText("");
            ui->textEdit->reset_counter();
            ui->textEdit_2->reset_counter();
            break;
        default: //cancel do nothing
            break;
    }
}

void EditorWindow::help_popup(bool checked)
{
    if(checked){}; //doesn't do anything
    QMessageBox help;
    help.setText("Welcome to Atomic block Editor!\nYou can implement new block or edit existing blocks here.\n"
                 "Specify what your block name is and the category under which it will be listed\n"
                 "Specify input pins and output pins (hovering over them will provide more info)\n"
                 "Specify the block logic open_show_librarywith c/c++ code\n"
                 "Have fun!");
    help.exec();
}


void EditorWindow::open_show_library(QFileSystemModel *model)
{
    open_dial = new EditorOpenDialog(this,model);
    connect(open_dial,SIGNAL(add_category(QString)),this,SIGNAL(get_categories(QString)));
    connect(open_dial,SIGNAL(remove_category_or_block(QModelIndex, QString)),this,SIGNAL(open_window_remove_signal(QModelIndex, QString)));
    connect(open_dial,SIGNAL(opening_block(QModelIndex)),this,SIGNAL(open_window_block_selected(QModelIndex)));
    open_dial->exec();
    //qDebug() << "end";
    disconnect(open_dial,nullptr,this,nullptr);
    //emit get_categories(""); //refresh combobox (for possible changes)
    delete(open_dial);
}


QString EditorWindow::readjust_out(QString variable)
{
    QStringList list = variable.split("&");
    return list.join("");
}


void EditorWindow::filldata_after_read(QStringList cat_name_desc_code,QStringList inputs,QStringList outputs)
{
    ui->comboBox->setCurrentText(cat_name_desc_code[0]);
    ui->lineEdit->setText(cat_name_desc_code[1]);
    ui->textEdit_3->setPlainText(cat_name_desc_code[3]);

    if(inputs.count() != 0){
        ui->textEdit->setPlainText("");
    }else{
        ui->textEdit->setPlainText("1:   ");
    }
    if(outputs.count() != 0){
        ui->textEdit_2->setPlainText("");
    }else{
        ui->textEdit_2->setPlainText("1:   ");
    }

    int counter = 0;
    for(int i = 0; i < inputs.count();i = i+2){
        counter++;
        ui->textEdit->insertPlainText(QString::number(counter));
        ui->textEdit->insertPlainText(":   " + inputs[i] + "," + inputs[i+1] + "\n");
    }
    ui->textEdit->set_counter(counter);
    counter = 0;
    for(int i = 0; i < outputs.count();i = i+2){
        counter++;
        ui->textEdit_2->insertPlainText(QString::number(counter));
        ui->textEdit_2->insertPlainText(":   " + readjust_out(outputs[i]) + "," + outputs[i+1] + "\n");
    }
    ui->textEdit_2->set_counter(counter);
    counter = 0;

}
