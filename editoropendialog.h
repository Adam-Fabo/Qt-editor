/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: editoropendialog.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains EditorOpenDialog class, file was generated through Qt but filled by hand.
 *
 */

#ifndef EDITOROPENDIALOG_H
#define EDITOROPENDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>


namespace Ui {
class EditorOpenDialog;
}

/**
 * @brief Class representing View of library in Editor
 */
class EditorOpenDialog : public QDialog
{
    Q_OBJECT


public:
    explicit EditorOpenDialog(QWidget *parent = nullptr, QFileSystemModel *model = nullptr);
    ~EditorOpenDialog();

public slots:
    /**
     * @brief Double-clicking on block opens it
     * @param index
     */
    void choose(QModelIndex index);
    void addcategory();
    void remove();

signals:
    /**
     * @brief Signal for model to create a new category
     * @param catname Name of category
     */
    void add_category(QString catname);
    /**
     * @brief Signal for model to remove something from library
     * @param object What to remove
     * @param catorblock Is it block or category
     */
    void remove_category_or_block(QModelIndex object, QString catorblock);
    /**
     * @brief Signal notifying model to send data about chosen block
     * @param index Which block was chosen
     */
    void opening_block(QModelIndex index);

private:
    Ui::EditorOpenDialog *ui;
};

#endif // EDITOROPENDIALOG_H
