/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: savecompositedial.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class SaveCompositeDial that inherits from QDialog,
 *              which lets user specify information about composite block that is about to be saved
 *
 */



#ifndef SAVECOMPOSITEDIAL_H
#define SAVECOMPOSITEDIAL_H

#include <QDialog>
#include <QInputDialog>
#include <QStringList>
#include "appmodel.h"

namespace Ui {
class SaveCompositeDial;
}

/**
 * @brief dialog window that lets user insert name, description and type of composite block that is about to be saved
 */
class SaveCompositeDial : public QDialog
{
    Q_OBJECT

public:
    explicit SaveCompositeDial(QWidget *parent = nullptr);
    ~SaveCompositeDial();

public slots:
    /**
     * @brief gets called when ok button is pressed, emits saveCompositeSignal() and accepts the dialog
     */
    void on_okButton_clicked();

    /**
     * @brief gets called when cancel button is pressed, rejects dialog
     */
    void on_cancelButton_clicked();

    /**
     * @brief gets called when add category [+] button is pressed, opens window where user can write catogory name
     */
    void on_addCategoryButton_clicked();

    /**
     * @brief slot for setting categories than will be shown to user
     * @param list of categories
     */
    void setCategories(QStringList list);

signals:
    /**
     * @brief gets emited when ok button is pressed
     */
    void saveCompositeSignal(QString name,QString categ,QString desc);

private:
    Ui::SaveCompositeDial *ui;
    AppModel * model;
};

#endif // SAVECOMPOSITEDIAL_H
