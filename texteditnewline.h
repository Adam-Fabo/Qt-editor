/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: texteditnewline.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains TextEditNewline class, file was generated through Qt but filled by hand.
 *
 */




#ifndef TEXTEDITNEWLINE_H
#define TEXTEDITNEWLINE_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>

/**
 * @brief The TextEditNewline class is a subclass of QTextEdit with tweaked functionality
 */
class TextEditNewline : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditNewline(QWidget *parent = nullptr);
    /**
     * @brief Overrided method that adds a number on the beggining of line
     * @param e Event
     */
    virtual void keyPressEvent(QKeyEvent *e) override;
    /**
     * @brief Resets the counter
     */
    void reset_counter();
    /**
     * @brief Sets the counter to number
     * @param number
     */
    void set_counter(int number);

signals:


private:
    int counter; //number to be put on line

};

#endif // TEXTEDITNEWLINE_H
