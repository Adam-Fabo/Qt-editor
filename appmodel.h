/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: appmodel.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains AppModel class, file was generated through Qt but filled by hand.
 *
 */

#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QDir>
#include <QFile> //filehandle
#include <QCoreApplication>
#include <QTextStream> //for read/write
#include <QFileSystemModel>
#include <QTimer>
#include <QGraphicsItem>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QString>
#include <QMessageBox>
#include "scene.h"

/**
 * @brief Model of the aplication, works with aplication data
 */
class AppModel : public QObject
{
    Q_OBJECT
public:
    AppModel();
    ~AppModel();

public slots:
    /**
     * @brief Finds out all categories(basicblocks) and emits them with send_catefories()
     * @param new_category if not "" creates new category with name specified here
     */
    void check_categories(QString new_category);
    void check_composite_categories(QString new_category);
    /**
     * @brief Saves/Overwrites block
     * @param cat_name_desc_code
     * @param inputs
     * @param outputs
     */
    void save_new_block(QStringList cat_name_desc_code,QStringList inputs, QStringList outputs);
    /**
     * @brief emits signal send_filemodel() with model of basicblocks library
     */
    void filesystemmodel();
    /**
     * @brief In Editor Open a remove button was pressed
     * @param object what is going to be removed
     * @param cat_or_block is it category or block (when category removes all of its content as well)
     */
    void open_remove_action(QModelIndex object, QString cat_or_block);
    /**
     * @brief In Editor Open read a block and emit block_file_read()
     * @param object block to be read
     */
    void open_block(QModelIndex object);

    /**
     * @brief In Editor Open read a block and emit block_file_read()
     * @param block name to be read
     */
    void open_block_by_name(QString object);

    /**
     * @brief Saves scene to file which is selected by user, params are vectors of blocks in a scene
     * @param
     * @param
     * @param
     * @param
     * @param
     */
    void saveSceneAs( QVector <QGraphicsItem *> blockList, QVector <QString> blockListStrings,QVector <QGraphicsItem *> commentList,
                      QVector <QString> commentListStrings, QVector <QGraphicsLineItem * >lineList);

    /**
     * @brief Loads items from a file into a scene
     * @param scene which will get loaded items into
     */
    void loadScene(Scene * scene);

    /**
     * @brief Saves composite block to file which is selected by user, similar to saveSceneAs
     * @param
     * @param
     * @param
     * @param
     * @param
     * @param
     * @param
     * @param
     */
    void saveComposite(QString name, QString category,QString desc, QVector <QGraphicsItem *> blockList, QVector <QString> blockListStrings,
                       QVector <QGraphicsItem *> commentList, QVector <QString> commentListStrings, QVector <QGraphicsLineItem * >lineList);


    /**
     * @brief Formats inputs or outputs in format "[data type] [var name], [pin name]" into "[data type] [var name]" , "pin name"
     * @param
     * @param
     * @param
     * @return
     */
    QStringList format_inout(QString in_or_out, QStringList list,int &info);

    /**
     * @brief returns filename
     * @return
     */
    QString getFileName(){return fileName;}

signals:
    /**
     * @brief Emits existing categories
     * @param list
     */
    void send_categories(QStringList list);
    void send_composite_categories(QStringList list);
    /**
     * @brief Signal to notify that a category already exists
     */
    void category_already_exists();
    /**
     * @brief Sends warning message
     * @param text
     */
    void warn_saving(QString text); //warn_saving is not a great name beacause it warns when opening as well
    /**
     * @brief Notify of succesfull saving
     * @param succes
     */
    void block_saved_succesfully(bool succes);

    void send_filemodel(QFileSystemModel *filemodel);
    /**
     * @brief Send information gained from file
     * @param cat_name_desc_code
     * @param inputs
     * @param outputs
     */
    void block_file_read(QStringList cat_name_desc_code,QStringList inputs, QStringList outputs);



private:
    //QString exec_path;
    QDir*   exec_path; //path to main /library
    QDir*   basic_lib_path; //path to /library/basicblocks
    QFileSystemModel *basic_blocks_model; //model for basic parts library

    QDir*   composite_lib_path; //path to /library/composite
    QString fileName;

};

#endif // APPMODEL_H
