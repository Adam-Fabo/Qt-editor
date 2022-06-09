/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderwindow.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of BuilderWindow class methods
 *
 */


#include "builderwindow.h"
#include "ui_builderwindow.h"
#include "scene.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QTreeWidgetItem>

BuilderWindow::BuilderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BuilderWindow)
{

    mainView = new MainView();
    ui->setupUi(this);
    setWindowTitle("Builder");

    setCentralWidget(mainView);

    scene = new Scene();
    mainView->setScene(scene);

    // pre moznost pohybovania sceny
    mainView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    builderHelp = new BuilderHelpWindow();
    builderAddPart = new BuilderAddPartDial();

    dial = new SaveCompositeDial();


    connect(builderAddPart, SIGNAL(rejected()), this, SLOT(qdialog_ended()));
    connect(builderAddPart, SIGNAL(blockSelected(QStringList,QStringList,QStringList)),scene,SLOT(setBlockParams(QStringList,QStringList,QStringList)));

    //ak uzivatel kline na ok pri okne na ulozenie kompozitneho
    connect(dial,SIGNAL(saveCompositeSignal(QString , QString , QString )),this,SLOT(saveComposite(QString , QString , QString )));


}

BuilderWindow::~BuilderWindow()
{
    delete ui;
    delete scene;
    delete mainView;
    delete builderHelp;
    delete builderAddPart;

    delete dial;

}

void BuilderWindow::closeEvent(QCloseEvent *e)
{
    emit close_builder();
    QMainWindow::closeEvent(e); //pass event to base
}



void BuilderWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        // ak su nejake zapnute a stlaci sa ESC tak sa vypnu
        if(  ui->addLine->isChecked() )
             ui->addLine->toggle();


        if(  ui->addBlock->isChecked() ){
              builderAddPart->show();
        }

        if(  ui->delLine->isChecked() )
             ui->delLine->toggle();
    }
}



void BuilderWindow::on_addLine_toggled(bool arg1)
{

    scene->setLineDrawing(arg1);

    if(arg1 == true){
        if(ui->delLine->isChecked())
            ui->delLine->toggle();

        if(ui->addBlock->isChecked())
            ui->addBlock->toggle();
    }
}


void BuilderWindow::on_delLine_toggled(bool arg1)
{
    scene->setDeleteToggled(arg1);

    if(arg1 == true){
        if(ui->addLine->isChecked())
            ui->addLine->toggle();

        if(ui->addBlock->isChecked())
            ui->addBlock->toggle();
    }

}

void BuilderWindow::on_addBlock_toggled(bool arg1)
{
    scene->setPartInsert(arg1);

    if(arg1 == true){
         builderAddPart->show();

         if(arg1 == true){
             if(ui->addLine->isChecked())
                 ui->addLine->toggle();

             if(ui->delLine->isChecked())
                 ui->delLine->toggle();
         }
    }
}


void BuilderWindow::qdialog_ended()
{
    on_addBlock_toggled(false);
    ui->addBlock->setChecked(false);
}


void BuilderWindow::on_actionCompile_triggered(bool checked)
{


  if(checked){};

  AppModel * model = new AppModel();
  model->saveSceneAs(scene->blockList, scene->blockListStrings, scene->commentList,scene->commentListStrings,scene->lineList);


  Compiler *kompiler = new Compiler(nullptr,scene->blockList,scene->blockListStrings,scene->lineList,true,model->getFileName());
  kompiler->compile();

  delete model;
  delete(kompiler);
}


void BuilderWindow::on_actionSave_as_triggered()
{
    for(int i=0; i <scene->blockListStrings.count();i++){
        if(scene->blockListStrings[i].startsWith("IN:") || scene->blockListStrings[i].startsWith("OUT:")){
            QMessageBox msgBox;
            msgBox.setText("Scene has IN/OUT blocks that are only supposed to be in composite blocks");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }
    }
    AppModel * model = new AppModel();
    model->saveSceneAs(scene->blockList, scene->blockListStrings, scene->commentList,scene->commentListStrings,scene->lineList);
    delete model;

}


void BuilderWindow::on_actionNew_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to proceed without saving?");

    msgBox.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setText("Do you wish to save first?");
    msgBox.setInformativeText("Not doing so will discard all changes!");
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Save:
          on_actionSave_as_triggered();
          break;
      case QMessageBox::Discard:
        break;
      case QMessageBox::Cancel:
          return;
          break;
    }



    //todo mozno okno v ktorom sa spyta ci ulozit
    scene->clear();
    scene->lineList = {};

    scene->commentList = {};
    scene->commentListStrings = {};
    scene->blockList = {};
    scene->blockListStrings = {};
}

void BuilderWindow::on_actionOpen_triggered()
{
    AppModel * model = new AppModel();
    model->loadScene(scene);
    delete model;
}


void BuilderWindow::on_actionSave_Composite_triggered()
{
    dial->show();
}


void BuilderWindow::saveComposite(QString name, QString categ, QString desc)
{
    AppModel * model = new AppModel();
    model->saveComposite(name,categ,desc,scene->blockList,scene->blockListStrings,scene->commentList,scene->commentListStrings,scene->lineList);
    delete model;
}
