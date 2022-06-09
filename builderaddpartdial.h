/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: builderaddpartdial.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class BuilderAddPartDial that inherits from QDialog,
 *              which is used for letting user pick a part when he want to insert part into scene
 *
 */

#ifndef BUILDERADDPARTDIAL_H
#define BUILDERADDPARTDIAL_H

#include <QDialog>
#include <QAbstractButton>
#include <QDebug>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QFileSystemModel>

#include "appmodel.h"

namespace Ui {
class BuilderAddPartDial;
}

/**
 * @brief class responsible for opening window from which is part selected
 */
class BuilderAddPartDial : public QDialog
{
    Q_OBJECT

public:
    explicit BuilderAddPartDial(QWidget *parent = nullptr);
    ~BuilderAddPartDial();

public slots:
    //void on_buttonBox_clicked(QAbstractButton *button);
    /**
     * @brief sets up treeView that is used for selecting part
     */
    void setupTreeWiew();

    /**
     * @brief sets directory struct from which are selected parts
     * @param directory struct
     */
    void setDirecoryStruct(QFileSystemModel * dir){dirStruct = dir;}

    /**
     * @brief is called when cancel Button is clicked
     */
    void on_cancelButton_clicked();

    /**
     * @brief is called when ok Button is clicked, emits blockSelected() signal
     */
    void on_okButton_clicked();

    /**
     * @brief saves to local variables given params
     * @param
     * @param
     * @param
     */
    void treeViewSlot(QStringList cat_name_desc_code,QStringList inputs, QStringList outputs);

    /**
     * @brief slot for model signal - warn_saving()
     * @param  - not used
     */
    void warning(QString s);

signals:

    /**
     * @brief is emited when block gets selected in treeView
     * @param
     * @param
     * @param
     */
    void blockSelected(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param);

private:
    Ui::BuilderAddPartDial *ui;
    //QTreeWidgetItem * treeItem{nullptr};

    /**
     * @brief directory struct of library
     */
    QFileSystemModel * dirStruct;
    AppModel * model;

    bool valid_selection;

    QStringList cat_name_desc_code;
    QStringList inputs;
    QStringList outputs;
};

#endif // BUILDERADDPARTDIAL_H
