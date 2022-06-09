/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: mainwindow.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains MainWindow class, file was generated through Qt but filled by hand.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editorwindow.h"
#include "builderwindow.h"
#include "appmodel.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Main window of the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    /**
     * @brief Switch window to builder
     */
    void Clicking_b();
    /**
     * @brief Switch window to editor
     */
    void Clicking_e();
    /**
     * @brief Switch window to main menu
     */
    void back_to_m();
    /**
     * @brief Dialog with credits
     */
    void show_credits();
    /**
     * @brief Dialog with help
     */
    void show_help();

private:
    Ui::MainWindow *ui;
    AppModel *model;
    BuilderWindow *builder;
    EditorWindow *editor;


};
#endif // MAINWINDOW_H
