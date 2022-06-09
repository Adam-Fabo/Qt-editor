/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: scene.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description:  file contains definition of Scene class methods
 *
 */




#include "scene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QPen>
#include <QGraphicsItem>



Scene::Scene(QObject *parent) :
    QGraphicsScene(parent), gridSize(20)
{
    Q_ASSERT(gridSize > 0);


    win = new InsertTextWindow();
    const_win = new InsertConsDialog();

    connect(win, SIGNAL(okPressed(QString)), this, SLOT(setStr(QString)));


    // connecting signals for selection of constatnt block
    connect(const_win, SIGNAL(okPressed(QString)), this, SLOT(setStr(QString)));
    connect(const_win, SIGNAL(constInt()),    this, SLOT(setConstInt()));
    connect(const_win, SIGNAL(constFloat()),  this, SLOT(setConstFloat()));
    connect(const_win, SIGNAL(constBool()),   this, SLOT(setConstBool()));
    connect(const_win, SIGNAL(constString()), this, SLOT(setConstString()));
}


Scene::~Scene()
{
    delete win;
    delete const_win;
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    // vykreslenie gridu

    QPen pen;
    painter->setPen(pen);

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);
    QVector<QPointF> points;

    for (qreal x = left; x < rect.right(); x += gridSize){
        for (qreal y = top; y < rect.bottom(); y += gridSize){
            points.append(QPointF(x,y));
        }
    }

    painter->drawPoints(points.data(), points.size());
}



void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //kreslenie ciary
    if(event->button() == Qt::LeftButton && lineDrawing){

        QPointF pos = event->scenePos();
        QPointF final;

        horizLine = new QGraphicsLineItem;
        vertiLine = new QGraphicsLineItem;

        horizLine->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        vertiLine->setFlag(QGraphicsItem::ItemSendsGeometryChanges);


        //vypocitanie suradnice tak aby ciara bola v gride
        final = getClosest(pos);

        horizLine->setPen(QPen(Qt::black,5));
        horizLine->setLine(QLineF(final.rx(),final.ry(),final.rx(),final.ry()));
        addItem(horizLine);

        vertiLine->setPen(QPen(Qt::black,5));
        vertiLine->setLine(QLineF(final.rx(),final.ry(),final.rx(),final.ry()));
        addItem(vertiLine);

    }else if(event->button() == Qt::LeftButton && deleteToggled){

        //get item
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item== 0)
            return;


        // delete item from vectors
        if(blockList.indexOf(item) != -1){
            blockListStrings.removeAt( blockList.indexOf(item) );
            blockList.removeAt( blockList.indexOf(item) );

        }else if(commentList.indexOf(item) != -1){
            commentListStrings.removeAt( commentList.indexOf(item) );
            commentList.removeAt( commentList.indexOf(item) );

        }else if( lineList.indexOf((QGraphicsLineItem *)item) != -1){
            lineList.removeAt(lineList.indexOf((QGraphicsLineItem *)item));
        }

        removeItem(item);

    }else if(event->button() == Qt::LeftButton && partInsert){

        // pridanie bloku
        QPointF pos = event->scenePos();
        pos = getClosest(pos);
        createBlock();
        block->setPos(pos);
        addItem(block);



    }else if(event->button() == Qt::LeftButton && !partInsert && !deleteToggled && !lineDrawing && selectedItems().size() != 0){
        itemMoving = true;
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // ak sa hybe s ciarou
    if(horizLine){
        QPointF pos = event->scenePos();
        QPointF horiz_end;
        QPointF closest;

        closest = getClosest(pos);

        // najblizsia suradnica na x
        horiz_end.setX(closest.rx());
        // y sa nemeni
        horiz_end.setY(horizLine->line().p1().ry());
        horizLine->setLine(QLineF(horizLine->line().p1(),horiz_end));

        QPointF vert_start = horiz_end;
        QPointF vert_end = horiz_end;

        // najblizsia suradnica na y
        vert_end.setY(closest.ry());
        vertiLine->setLine(QLineF(vert_start,vert_end));

    }else if(partInsert  && block != nullptr){

        QPointF pos = event->scenePos();
        pos = getClosest(pos);

        block->setPos(pos);

    }else if(itemMoving && !partInsert && !deleteToggled && !lineDrawing && selectedItems().size() != 0){

        QPointF currPos = event->scenePos();

        selectedItems().first()->setPos( getClosest(currPos) );
    }

    QGraphicsScene::mouseMoveEvent(event);
}


void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if(horizLine && event->button() == Qt::LeftButton){
        // pridavanie ciary

        auto x = horizLine->line().dx();

         // ak by ciara mala byt dlha 0 tak sa ani neprida do sceny
        if(x ==0)
            removeItem(horizLine);
        else
            lineList.append(horizLine);


        auto y = vertiLine->line().dy();

        // ak by ciara mala byt dlha 0 tak sa ani neprida do sceny
        if(y ==0)
            removeItem(vertiLine);
        else
            lineList.append(vertiLine);


        horizLine = nullptr;
        vertiLine = nullptr;
        clearSelection();

        QRectF rec = itemsBoundingRect();
        rec.adjust(-300,-300,300,300);
        setSceneRect(rec);

    }else if(partInsert && block != nullptr){
        //pridavanie bloku

        block = nullptr;
        clearSelection();
        QRectF rec = itemsBoundingRect();
        rec.adjust(-300,-300,300,300);
        setSceneRect(rec);

    }else if(event->button() == Qt::LeftButton && itemMoving && !partInsert && !deleteToggled && !lineDrawing && selectedItems().size() != 0){
        // ak je mod hybania itemu

        itemMoving = true;
        clearSelection();
        QRectF rec = itemsBoundingRect();
        rec.adjust(-300,-300,300,300);
        setSceneRect(rec);

    }

    QGraphicsScene::mouseReleaseEvent(event);
}


//vrati najblizsiu poziciu v gride ku zadanej
QPointF Scene::getClosest(QPointF pos)
{
    int x = int(pos.rx());
    int y = int(pos.ry());

    QPointF final;
    bool negative = false;

     // x
    if(x < 0) negative = true;
    x = abs(x);

    if(x %20 <10) x = x - x % 20;
    else          x = x + (20 - x % 20);

    if(negative) x = x * -1;


    // y
    negative= false;
    if(y < 0)negative = true;
    y = abs(y);

    if(y %20 <10) y = y - y % 20;
    else          y = y + (20 - y % 20);

    if(negative) y = y * -1;

    final.rx() = x;
    final.ry() = y;

    return final;
}

void Scene::setStr(QString s)
{
    str = s;
    strSelected = true;
}


void Scene::setBlockParams(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param)
{

    name = cat_name_desc_code_param[1];
    cat_name_desc_code = cat_name_desc_code_param;
    inputs = inputs_param;
    outputs = outputs_param;

    if(QString::compare(name,"Input") == 0){
         win->show();
    }else if(QString::compare(name,"Output") == 0){
         win->show();
    }else if(QString::compare(name,"Comment") == 0){
        win->show();
    }else if(QString::compare(name,"Constant") == 0){
        const_win->show();
    }else{
        ;
    }
}

void Scene::setBlockParamsNoPopUp(QStringList cat_name_desc_code_param, QStringList inputs_param, QStringList outputs_param)
{
    name = cat_name_desc_code_param[1];
    cat_name_desc_code = cat_name_desc_code_param;
    inputs = inputs_param;
    outputs = outputs_param;
}



void Scene::setLineDrawing(bool b)
{

    if(b == false){
        if(horizLine != nullptr)
            removeItem(horizLine);
        if(vertiLine != nullptr)
            removeItem(vertiLine);

        lineDrawing = false;
    }else{
        lineDrawing = true;
    }


}

void Scene::setDeleteToggled(bool b)
{
    deleteToggled = b;
}

void Scene::setPartInsert(bool b)
{

    if(b == false){
        if(block != nullptr)
            removeItem(block);

        partInsert = false;
    }else{
        partInsert = true;
    }
}


void Scene::addBlock(QPoint pos)
{


    createBlock();
    block->setPos(pos);
    addItem(block);
    block = NULL;
}



void Scene::createBlock()
{
    QFont serifFont("Times", 14, QFont::Bold);

    if(QString::compare(name,"Input") == 0){
          createBlockInput();
    }else if(QString::compare(name,"Output") == 0){
        createBlockOutput();
    }else if(QString::compare(name,"Comment") == 0){
        createBlockComment();
    }else if(QString::compare(name,"Constant") == 0){
        createBlockConstant();
    }else{

        block = new QGraphicsItemGroup();
        block->setFlags(QGraphicsItem::ItemIsSelectable);


        int inputsNum  = inputs.count();
        int outputsNum = outputs.count();

        int horizLen = 80;
        int vertiLen = 0;
        int pinLen = 20;

        int tmp_max = std::max(inputsNum/2,outputsNum/2);

        if(tmp_max>4){
            vertiLen = (tmp_max + 1) * 20;
        }else{
            vertiLen = 100;
        }

        block = new QGraphicsItemGroup();
        block->setFlags(QGraphicsItem::ItemIsSelectable);



        //nazov
        auto * tmpTxt = new QGraphicsTextItem();
        tmpTxt->setPlainText(name);
        tmpTxt->setFont(serifFont);
        tmpTxt->setPos(20,-30);
        block->addToGroup(tmpTxt);

        blockList.append(block);
        blockListStrings.append(name);
        //blockListStrings.append(name);



        //horna horiz ciara
        auto * tmp = new QGraphicsLineItem(0,0,horizLen,0);
        tmp->setPen(QPen(Qt::black,5));
        block->addToGroup(tmp);

        //spodna horiz ciara
        tmp = new QGraphicsLineItem(0,vertiLen,horizLen,vertiLen);
        tmp->setPen(QPen(Qt::black,5));
        block->addToGroup(tmp);

        //lava verti ciara
        tmp = new QGraphicsLineItem(0,0,0,vertiLen);
        tmp->setPen(QPen(Qt::black,5));
        block->addToGroup(tmp);

        //prava verti ciara
        tmp = new QGraphicsLineItem(horizLen,0,horizLen,vertiLen);
        tmp->setPen(QPen(Qt::black,5));
        block->addToGroup(tmp);


        QFont fnt("Times", 8);
        //nakreslenie inputov - nalavo
        for(int i = 0; i< inputsNum; i++){
            //nakreslenie ciary
            if(i%2 != 0){
                tmp = new QGraphicsLineItem(-pinLen,20*(i+1)/2,0,20*(i+1)/2);
                tmp->setPen(QPen(Qt::black,5));
                block->addToGroup(tmp);

                //nazov pinu
                tmpTxt = new QGraphicsTextItem();
                tmpTxt->setPlainText(inputs[i]);
                tmpTxt->setFont(fnt);
                tmpTxt->setPos(5,20*(i+1)/2-15);
                block->addToGroup(tmpTxt);
            }

        }

        //nakreslenie outputov - napravo
        for(int i = 0; i< outputsNum; i++){
            //nakreslenie ciary
            if(i%2 != 0){
                tmp = new QGraphicsLineItem(horizLen,20*(i+1)/2,horizLen + pinLen,20*(i+1)/2);
                tmp->setPen(QPen(Qt::black,5));
                block->addToGroup(tmp);

                //nazov pinu
                tmpTxt = new QGraphicsTextItem();
                tmpTxt->setPlainText(outputs[i]);
                tmpTxt->setFont(fnt);
                tmpTxt->setPos(40,20*(i+1)/2-15);
                block->addToGroup(tmpTxt);
            }

        }
    }

    //qDebug() << blockListStrings;
}

void Scene::createBlockComment()
{
    // komenty sa budu dat vyfiltrovat tak ze grupa obsahuje iba jeden prvok
    QFont serifFont("Times", 14, QFont::Bold);
    block = new QGraphicsItemGroup();
    block->setFlags(QGraphicsItem::ItemIsSelectable);

    auto * tmpTxt = new QGraphicsTextItem();
    tmpTxt->setPlainText(str);
    tmpTxt->setFont(serifFont);
    tmpTxt->setPos(0,-20);
    block->addToGroup(tmpTxt);

    commentList.append(block);
    commentListStrings.append(str);

}

void Scene::createBlockInput()
{
    QFont serifFont("Times", 14, QFont::Bold);

    block = new QGraphicsItemGroup();
    block->setFlags(QGraphicsItem::ItemIsSelectable);


    block = new QGraphicsItemGroup();
    block->setFlags(QGraphicsItem::ItemIsSelectable);

    //horna horiz ciara
    auto * tmp = new QGraphicsLineItem(0,0,120,0);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);

    //spodna horiz ciara
    tmp = new QGraphicsLineItem(0,40,120,40);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);

    //lava verti ciara
    tmp = new QGraphicsLineItem(0,0,0,40);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);

    //horna diag ciara
    tmp = new QGraphicsLineItem(120,0,140,20);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);

    //spodna diag ciara
    tmp = new QGraphicsLineItem(120,40,140,20);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);

    //posledna ciarka
    tmp = new QGraphicsLineItem(140,20,160,20);
    tmp->setPen(QPen(Qt::black,5));
    //items.append(tmp);
    block->addToGroup(tmp);


    //text
    auto * tmpTxt = new QGraphicsTextItem();
    tmpTxt->setPlainText("IN:" + str);
    tmpTxt->setFont(serifFont);
    tmpTxt->setPos(0,-30);
    block->addToGroup(tmpTxt);

    //block = createItemGroup(items);

    blockList.append(block);
    blockListStrings.append(tmpTxt->toPlainText());


}

void Scene::createBlockOutput()
{
    QFont serifFont("Times", 14, QFont::Bold);

    block = new QGraphicsItemGroup();
    block->setFlags(QGraphicsItem::ItemIsSelectable);

    //horna horiz ciara
    auto * tmp = new QGraphicsLineItem(40,0,160,0);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    //spodna horiz ciara
    tmp = new QGraphicsLineItem(40,40,160,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    //prva verti ciara
    tmp = new QGraphicsLineItem(160,0,160,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);


    //horna diag ciara
    tmp = new QGraphicsLineItem(20,20,40,0);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    //spodna diag ciara
    tmp = new QGraphicsLineItem(20,20,40,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    //posledna ciarka
    tmp = new QGraphicsLineItem(0,20,20,20);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);


    //text
    auto * tmpTxt = new QGraphicsTextItem();
    tmpTxt->setPlainText("OUT:" + str);
    tmpTxt->setFont(serifFont);
    tmpTxt->setPos(40,-30);
    block->addToGroup(tmpTxt);

    blockList.append(block);
    blockListStrings.append(tmpTxt->toPlainText());

}



void Scene::createBlockConstant()
{

    QFont serifFont("Times", 14, QFont::Bold);

    block = new QGraphicsItemGroup();
    block->setFlags(QGraphicsItem::ItemIsSelectable);

    auto * tmpTxt = new QGraphicsTextItem();
    if(constInt)
        tmpTxt->setPlainText("CONST INT:" + str);
    else if(constFloat)
        tmpTxt->setPlainText("CONST FLOAT:" + str);
    else if(constBool)
        tmpTxt->setPlainText("CONST BOOL:" + str);
    else if(constString)
        tmpTxt->setPlainText("CONST STRING:" + str);

    tmpTxt->setFont(serifFont);
    tmpTxt->setPos(0,-30);
    block->addToGroup(tmpTxt);

    // vrchna vodorovna
    auto * tmp = new QGraphicsLineItem(0,0,140,0);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    // spodna vodorovna
    tmp = new QGraphicsLineItem(0,40,140,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    // lava zvisla
    tmp = new QGraphicsLineItem(0,0,0,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    // prava zvisla
    tmp = new QGraphicsLineItem(140,0,140,40);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    // konektor
    tmp = new QGraphicsLineItem(140,20,160,20);
    tmp->setPen(QPen(Qt::black,5));
    block->addToGroup(tmp);

    blockList.append(block);
    blockListStrings.append(tmpTxt->toPlainText());
}



