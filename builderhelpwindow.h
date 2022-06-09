/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderhelpwindow.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class BuilderHelpWindow that inherits from QMainWindow,
 *              which is used for showing user help tips when creating a scene
 *
 */

#ifndef BUILDERHELPWINDOW_H
#define BUILDERHELPWINDOW_H

#include <QMainWindow>


namespace Ui {
class BuilderHelpWindow;
}

/**
 * @brief pop-up window that helps user with app
 */
class BuilderHelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BuilderHelpWindow(QWidget *parent = nullptr);
    ~BuilderHelpWindow();

private:
    Ui::BuilderHelpWindow *ui;

};

#endif // BUILDERHELPWINDOW_H
