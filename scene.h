/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: scene.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of class Scene that inherits from QGraphicsScene,
 *              on which is user able to draw scheme
 *
 */



#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>
#include <QGraphicsItemGroup>
#include <QTreeWidgetItem>

#include "inserttextwindow.h"
#include "insertconsdialog.h"



/**
 * @brief Scene on which are parts and lines drawn
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();
    /**
     * @brief returns grid size of scene
     */
    int getGridSize() const {return this->gridSize;}

    /**
     * @brief sets scene to line drawing mode, deselects all other modes
     * @param true/false
     */
    void setLineDrawing(bool b);
    /**
     * @brief returns true/false if scene is in line drawing mode
     * @return true/false
     */
    bool getLineDrawing() { return lineDrawing;} ;

    /**
     * @brief sets scene to line deleting mode, deselects all other modes
     * @param true/false
     */
    void setDeleteToggled(bool b);
    /**
     * @brief returns true/false if scene is in line deleting mode
     * @return true/false
     */
    bool getDeleteToggled() { return deleteToggled;} ;

    /**
     * @brief sets scene to line part inserting mode, deselects all other modes
     * @param true/false
     */
    void setPartInsert(bool b);
    /**
     * @brief returns true/false if scene is in part inserting mode
     * @return true/false
     */
    bool getPartInsert() { return partInsert;} ;


    //void setBlock();
    /**
     * @brief sets positing of currently selected block
     * @param
     */
    void setBlockPos(QPoint pos){block->setPos(pos);}

    /**
     * @brief adds block to the scene at given position, calls createBlock()
     * @param pos position
     */
    void addBlock(QPoint pos);

    /**
     * @brief creates block from current params that scene has
     */
    void createBlock();

    /**
     * @brief holds all blocks(parts,iputs,outputs,constants) that are in the scene
     */
    QVector <QGraphicsItem *> blockList;
    /**
     * @brief holds all block names that are in the scene
     */
    QVector <QString>       blockListStrings;

    /**
     * @brief holds all comments(as items) that are in the scene
     */
    QVector <QGraphicsItem *> commentList;  // drzi komenty
    /**
     * @brief holds all comments strings that are in the scene
     */
    QVector <QString>       commentListStrings;

    /**
     * @brief holds all lines that are in the scene
     */
    QVector <QGraphicsLineItem * >lineList; // drzi ciary


protected:
    void drawBackground (QPainter* painter, const QRectF &rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);



private:
    void createBlockComment();
    void createBlockInput();
    void createBlockOutput();
    void createBlockConstant();

    QGraphicsLineItem *horizLine{nullptr};
    QGraphicsLineItem *vertiLine{nullptr};
    QGraphicsItemGroup *block{nullptr};
    QGraphicsItemGroup *loadedBlock{nullptr};

    QTreeWidgetItem * treeItem{nullptr};

    // QPointF movingPos;
    // QPointF itemPos;
    bool itemMoving = false;
    int gridSize;


    QPointF getClosest(QPointF pos);


    InsertTextWindow * win;

    InsertConsDialog * const_win;


    bool constInt = true;
    bool constFloat = true;
    bool constBool = true;
    bool constString = true;

    int constSelected;

    bool lineDrawing   = false;
    bool deleteToggled = false;
    bool partInsert    = false;
    bool partActive    = false;

    int inputCounter  = 0;
    int outputCounter = 0;

    //patriace k bloku
    QString name;
    QStringList cat_name_desc_code;
    QStringList inputs;
    QStringList outputs;


    QString str;    // text z lineeditu z UI
    bool strSelected = false;

public slots:
    /**
     * @brief sets string that is used in block creation
     * @param string mostly is used as name
     */
    void setStr(QString s);

    /**
     * @brief sets parameters of a block that might be later created, of block is and input, output or constant - opens dialog where user can specify other parameters
     * @param vector of category - name - description - code
     * @param
     * @param
     */
    void setBlockParams(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param);

    /**
     * @brief sets parameters of a block that might be later created, of block is and input, output or constant does not open any dialog - other params takes from given parameters
     * @param vector of category - name - description - code
     * @param
     * @param
     */
    void setBlockParamsNoPopUp(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param);


    /**
     * @brief sets constant to Int
     */
    void setConstInt()   {constInt = true;  constFloat = false; constBool = false; constString = false;}

    /**
     * @brief sets constant to Float
     */
    void setConstFloat() {constInt = false;  constFloat = true; constBool = false; constString = false;}

    /**
     * @brief sets constant to bool
     */
    void setConstBool()  {constInt = false;  constFloat = false; constBool = true; constString = false;}

    /**
     * @brief sets constant to String
     */
    void setConstString(){constInt = false;  constFloat = false; constBool = false; constString = true;}


};

#endif // SCENE_H
