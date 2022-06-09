/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: insertconstcialog.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class InsertConsDialog that inherits from QDialog,
 *              in which user can insert information about constant block that is about to be inserted into scene
 *
 */



#ifndef INSERTCONSDIALOG_H
#define INSERTCONSDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class InsertConsDialog;
}

/**
 * @brief dialog window that asks user for constant and type that is selected by radiobuttons
 */
class InsertConsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertConsDialog(QWidget *parent = nullptr);
    ~InsertConsDialog();

public slots:
    /**
     * @brief gets called when ok button is pressed, accepts the dialog
     */
    void on_pushButton_OK_clicked();

    /**
     * @brief gets called when radiobutton INT gets clicked, emits constInt() signal
     * @param
     */
    void on_radioButton_int_clicked(bool checked);

    /**
     * @brief gets called when radiobutton FLOAT gets clicked, emits constFloat() signal
     * @param
     */
    void on_radioButton_float_clicked(bool checked);

    /**
     * @brief gets called when radiobutton BOOL gets clicked, emits constBool() signal
     * @param
     */
    void on_radioButton_bool_clicked(bool checked);

    /**
     * @brief gets called when radiobutton STRING gets clicked, emits constString() signal
     * @param
     */
    void on_radioButton_string_clicked(bool checked);


signals:

    /**
     * @brief emited when ok button is pressed
     * @param string from line edit, represents constant
     */
    void okPressed(QString  str);

    /**
     * @brief emited when INT radio button gets selected
     */
    void constInt();

    /**
     * @brief emited when FLOAT radio button gets selected
     */
    void constFloat();

    /**
     * @brief emited when STRING radio button gets selected
     */
    void constString();

    /**
     * @brief emited when BOOL radio button gets selected
     */
    void constBool();


private:
    Ui::InsertConsDialog *ui;

};

#endif // INSERTCONSDIALOG_H
