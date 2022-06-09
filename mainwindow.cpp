/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: mainwindow.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of MainWindow class, file was generated through Qt but filled by hand.
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    model = new AppModel();
    editor = new EditorWindow();  //create new top-level window
    builder = new BuilderWindow();

    //mainwindow connections
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Clicking_b()));
    connect(builder, SIGNAL(close_builder()),this, SLOT(back_to_m()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Clicking_e()));
    connect(editor, SIGNAL(close_editor()),this, SLOT(back_to_m()));

    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(show_credits()));//credits
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(show_help()));//help

    //editor-model connections
    connect(editor,SIGNAL(get_categories(QString)),model,SLOT(check_categories(QString)));
    connect(model,SIGNAL(send_categories(QStringList)),editor,SLOT(update_combobox(QStringList)));
    emit editor->get_categories(""); //initialize editors combobox
    connect(model,SIGNAL(category_already_exists()),editor,SLOT(warn_cat_exists()));
    connect(editor,SIGNAL(save_block_signal(QStringList,QStringList,QStringList)),model,SLOT(save_new_block(QStringList,QStringList,QStringList)));
    connect(model,SIGNAL(warn_saving(QString)),editor,SLOT(warn_block_problem(QString)));
    connect(model,SIGNAL(block_saved_succesfully(bool)),editor,SLOT(finished_saving(bool)));
    connect(editor,SIGNAL(get_filesystemmodel()),model,SLOT(filesystemmodel()));
    connect(model,SIGNAL(send_filemodel(QFileSystemModel*)),editor,SLOT(open_show_library(QFileSystemModel*)));
    connect(editor,SIGNAL(open_window_remove_signal(QModelIndex, QString)),model,SLOT(open_remove_action(QModelIndex, QString)));
    connect(editor,SIGNAL(open_window_block_selected(QModelIndex)),model,SLOT(open_block(QModelIndex)));
    connect(model,SIGNAL(block_file_read(QStringList,QStringList,QStringList)),editor,SLOT(filldata_after_read(QStringList,QStringList,QStringList)));
}

MainWindow::~MainWindow()
{
    delete editor;
    delete builder;
    delete model;
    delete ui;
}






void MainWindow::Clicking_b()
{
    hide();
    builder->show();
}

void MainWindow::Clicking_e()
{
    hide();
    editor->show();
}

void MainWindow::back_to_m()
{
    show();
}


void MainWindow::show_credits()
{
    QMessageBox help;
    help.setText("Adam Fabo (project leader) - Builder\n"
                 "Stanislav Gabriš - Editor and Compilation");
    help.exec();
}

void MainWindow::show_help()
{
    QMessageBox help;
    help.setText("Welcome to our app!\n\n"
                 "Pressing Editor brings you to the editor part of the app\n"
                 "-you can edit/create basic blocks there (more information in help of Editor)\n\n"
                 "Pressing Builder brings you to the builder part of the app\n"
                 "-you can create/edit scenes and compisite blocks as well as compile (more in help of Builder)");
    help.exec();
}



