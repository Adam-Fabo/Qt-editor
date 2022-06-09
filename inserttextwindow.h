/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: inserttextwindow.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class InsertTextWindow that inherits from QDialog,
 *              in which user can insert information about INPUT/OUTPUT block that is about to be inserted into scene
 *
 */


#ifndef INSERTTEXTWINDOW_H
#define INSERTTEXTWINDOW_H

#include <QDialog>
#include <QAbstractButton>
#include <QDebug>
#include <QString>
namespace Ui {
class InsertTextWindow;
}

/**
 * @brief dialog window that asks user for string
 */
class InsertTextWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTextWindow(QWidget *parent = nullptr);
    ~InsertTextWindow();

public slots:
    /**
     * @brief gets called when ok button is pressed, emits okPressed() signal
     * @param
     */
    void on_pushButton_clicked(bool checked);

signals:
    /**
     * @brief gets emmited when ok button is pressed
     * @param string from lineEdit
     */
    void okPressed(QString  str);

private:
    Ui::InsertTextWindow *ui;
};

#endif // INSERTTEXTWINDOW_H
