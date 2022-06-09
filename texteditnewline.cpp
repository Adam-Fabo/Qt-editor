/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: texteditnewline.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of TextEditNewline class, file was generated through Qt but filled by hand.
 *
 */


#include "texteditnewline.h"
#include <QDebug>
#include <string.h>

TextEditNewline::TextEditNewline(QWidget *parent) : QTextEdit(parent)
{
    counter = 1;
}

void TextEditNewline::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter){
        counter++;
        auto num = QString::number(counter);
        //append(num + ":   ");
        insertPlainText('\n' + num + ":   "); //better than append in this scenario
        //TODO using movePosition to find out the number on this line and edit counter accordingly
    }else{
        QTextEdit::keyPressEvent(e);
    }
}

void TextEditNewline::reset_counter()
{
    counter = 1;
}

void TextEditNewline::set_counter(int number)
{
    counter = number;
}
