/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderwindow.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class BuilderWindow that inherits from QMainWindow,
 *              which is used for displaying a scene where user can build scene
 *
 */



#ifndef BUILDERWINDOW_H
#define BUILDERWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QTreeWidgetItem>

#include "ui_builderwindow.h"
#include "scene.h"
#include "builderhelpwindow.h"
#include "builderaddpartdial.h"
#include "mainview.h"
#include "appmodel.h"

#include "savecompositedial.h"
#include "compiler.h"


namespace Ui {
class BuilderWindow;
}

/**
 * @brief class that takes care of Builder winndow that is responsible for drawing parts
 */
class BuilderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BuilderWindow(QWidget *parent = nullptr);
    ~BuilderWindow();

    virtual void closeEvent(QCloseEvent *e);

public slots:

    /**
     * @brief sets scene to drawing line mode
     * @param
     */
    void on_addLine_toggled(bool arg1);

    /**
     * @brief sets scene to deleting lines/parts
     * @param
     */
    void on_delLine_toggled(bool arg1);

    /**
     * @brief sets scene to adding part mode
     * @param
     */
    void on_addBlock_toggled(bool arg1);

    /**
     * @brief shows help window
     * @param
     */
    void on_actionHelp_triggered() { builderHelp->show(); };

    /**
     * @brief slot that gets notified when library window gets closed (by esc or cancel)
     */
    void qdialog_ended();

    /**
     * @brief gets called when save as button gets clicked on, saves scene a
     */
    void on_actionSave_as_triggered();

    /**
     * @brief gets called when compile button gets clicked on, saves scene and compiles it
     */
    void on_actionCompile_triggered(bool checked);

    /**
     * @brief gets called when new button gets clicked on, delete scene
     */
    void on_actionNew_triggered();

    /**
     * @brief gets called when open button gets clicked on, opens scene that user picks
     */
    void on_actionOpen_triggered();

    /**
     * @brief gets called when save composite button gets clicked on, saves and compiles composite block
     */
    void on_actionSave_Composite_triggered();

public slots:
    void saveComposite(QString name,QString categ,QString desc);

private:
    Ui::BuilderWindow *ui;

    BuilderHelpWindow * builderHelp;
    BuilderAddPartDial * builderAddPart;

    MainView * mainView;

    SaveCompositeDial * dial;

    /**
     * @brief scene of which are parts drawn
     */
    Scene * scene;
    void keyPressEvent(QKeyEvent *event);


signals:
    /**
     * @brief emited when builder window is closed
     */
    void close_builder();
};

#endif // BUILDERWINDOW_H
