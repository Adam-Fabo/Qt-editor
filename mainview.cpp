/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: mainview.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: file contains definition of MainView class methods
 *
 */

#include "mainview.h"

MainView::MainView(QWidget *parent) : QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

MainView::~MainView()
{
    ;
}


void MainView::wheelEvent ( QWheelEvent * event ) {

    if(event->delta() > 0){
         scale(1.25, 1.25);
    }else{
        scale(0.8, 0.8);
    }
}



void MainView::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::MiddleButton)
    {
        // Store original position.
        m_originX = event->x();
        m_originY = event->y();
    }

    QGraphicsView::mousePressEvent(event);

}

void MainView::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::MidButton)
    {
        QPointF oldp = mapToScene(m_originX, m_originY);
        QPointF newP = mapToScene(event->pos());
        QPointF translation = newP - oldp;

        translate(translation.x(), translation.y());

        m_originX = event->x();
        m_originY = event->y();
    }
        QGraphicsView::mouseMoveEvent(event);

}



