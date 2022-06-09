/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: mainview.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains MainView class, that is used by scene, mainly for zooming
 *
 */


#ifndef MAINVIEW_H
#define MAINVIEW_H
#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <QWheelEvent>
#include <QTimeLine>
#include <QObject>
#include <QDebug>
#include <QAbstractScrollArea>
#include <QScrollBar>


/**
 * @brief class that contains wiew for scene
 */
class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    MainView(QWidget *parent = 0);
    ~MainView();
private:
    void wheelEvent ( QWheelEvent * event );
    int _numScheduledScalings;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


    bool rightMousePressed = true;
    int  _panStartX ;
    int _panStartY;
    int m_originX ;
    int m_originY ;


};

#endif // MAINVIEW_H
