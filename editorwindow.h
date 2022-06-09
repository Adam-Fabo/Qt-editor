/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: editorwindow.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains EditorWindow class, file was generated through Qt but filled by hand.
 *
 */

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QInputDialog>
#include <QMessageBox>
#include "editoropendialog.h"



namespace Ui {
class EditorWindow;
}
/**
 * @brief The EditorWindow class represents the Editor part of the app
 */
class EditorWindow : public QMainWindow
{
    Q_OBJECT

signals:
    /**
     * @brief Notify main window about close event
     */
    void close_editor();
    /**
     * @brief Ask for existing categories and save new category
     * @param new_category
     */
    void get_categories(QString new_category);
    /**
     * @brief Tell model to save new block
     * @param cat_name_desc_code
     * @param inputs
     * @param outputs
     */
    void save_block_signal(QStringList cat_name_desc_code,QStringList inputs, QStringList outputs);
    /**
     * @brief Asks model for filesystemmodel
     */
    void get_filesystemmodel();
    /**
     * @brief Removing category or block
     * @param object
     * @param catorblock
     */
    void open_window_remove_signal(QModelIndex object, QString catorblock);
    /**
     * @brief A block was selected to be read
     * @param index
     */
    void open_window_block_selected(QModelIndex index);

public:
    explicit EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

    virtual void closeEvent(QCloseEvent *e);
    /**
     * @brief Formats text from textEdit widgets to required format
     * @param in_or_out
     * @param info
     * @return
     */
    QStringList format_inout(QString in_or_out, int &info);
    /**
     * @brief Popup window after pressing blank block
     * @return
     */
    int popup_blank();
    /**
     * @brief Removes & from variable
     * @param variable
     * @return
     */
    QString readjust_out(QString variable);

public slots:
    /**
     * @brief Asks user to provide new category and emits get_categories with new category
     */
    void add_category();
    /**
     * @brief Refreshes category combobox to list
     * @param list Items to be in combobox
     */
    void update_combobox(QStringList list);
    /**
     * @brief Popup warning
     */
    void warn_cat_exists();
    /**
     * @brief Popup warning with text
     * @param text
     */
    void warn_block_problem(QString text);
    /**
     * @brief Popup help with informations about editor
     * @param checked
     */
    void help_popup(bool checked);
    /**
     * @brief Changes statusbar
     * @param succes
     */
    void finished_saving(bool succes);
    /**
     * @brief Prepares data providided by user to be stored by model and emits them
     * @param checked
     */
    void saving(bool checked);
    /**
     * @brief Creates blank worksheet
     * @param checked
     */
    void blank_block(bool checked);
    /**
     * @brief Shows library of basic blocks
     * @param model
     */
    void open_show_library(QFileSystemModel *model);
    /**
     * @brief Fills worksheet with data from block chosen by user
     * @param cat_name_desc_code
     * @param inputs
     * @param outputs
     */
    void filldata_after_read(QStringList cat_name_desc_code,QStringList inputs,QStringList outputs);

private:
    Ui::EditorWindow *ui;
    EditorOpenDialog *open_dial;
};

#endif // EDITORWINDOW_H
