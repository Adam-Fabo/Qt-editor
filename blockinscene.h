/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: blockinscene.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains BlockInScene class, file was generated through Qt but filled by hand.
 *
 */

#ifndef BLOCKINSCENE_H
#define BLOCKINSCENE_H

#include <QString>
#include <QPointF>
#include <QGraphicsItemGroup>
#include <QStack>
#include <QMap>
#include <QRegularExpression>
#include <QPair>
/**
 * @brief The BlockInScene class represents a Block in scene and all necessary information about it
 */
class BlockInScene
{

public:
#define BLOCKWIDTH 80 //+-20 to in/out pins
#define TOPIN 20
#define CONSTBLOCKSWIDTH 140 //+20 to pin when in,const; 0 when out

public:
    bool            am_i_special_block;
    bool            am_i_out;
    bool            am_i_in;


    static int      object_count; //each blockinscene takes object_count and puts it into my_umber after which this is incremented, at the end holds number of all objects
    int             my_number; //a number of this object (unique for every instance)

    QPointF         my_position; //position in scene

    QString         name_of_group; //name that all objects in group share, e.g.:ADD
    QList<QString>  in_out_types; //group specific var types
    QString         my_call_code; //will be initialized together with var names

    bool            am_i_defined; //when all inputs are defined
    int             input_count;
    int             output_count;
    QList<QPointF>  input_positions;
    QList<QPointF>  output_positions;
    QList<QPointF>  already_processed_inputs; //inputs that were checked (for example are connected)
    QList<QPointF>  already_processed_outputs; //outputs that were checked

    //QVector<QPointF,int>       inputs_of_other_blocks; //map key value of connections -> input to a block(int is the specific number)
    QVector<int>            other_block; //other blocks unique number
    QVector<QPointF>        other_block_input; //other blocks inputs
    //QMap<QPointF,QPointF>   connections; //map of connections from this block to another ((200,50):(0,10)), int is specific number of object
    QVector<QPointF>        connected_output; //one output can go to multiple ins
    QVector<QPointF>        connected_to_input; //one input can have only one output -> this must be controlled from outside this class

    QMap<QString,QPointF>   in_out_var_name; //pins mapped to variables in generated code ((0,10):varname), varname is created by compiler

public:
    BlockInScene(QString group_name, QPointF position, int in_count, int out_count, QStringList inputs, QStringList outputs, int &var_names_from); //regular initialization
    BlockInScene(QString group_name, QPointF position, int &var_names_from); //special block-> CONST,IN,OUT
    /**
     * @brief Method that calculates input/output pin positions and stores the calculated QPointF into lists
     * @param special
     */
    void initialize_positions(bool special); //input/output positions
    /**
     * @brief Fills in_out_var_name with input/output pin types (Inputs and outputs sould be in order of pins from top to bottom)
     * @param inputs All inputs
     * @param outputs All outputs
     */
    void initialize_inouttypes(QStringList inputs, QStringList outputs);
    /**
     * @brief Method that creates unique variable names for each input/output
     * @param from The name is just a number prepended with "_", from holds the information about what number it should be
     */
    void initiallize_varnames_and_call(int &from);
    /**
     * @brief CONST/IN/OUT blocks are initialized a bit differently
     * @param from The name is just a number prepended with "_", from holds the information about what number it should be
     */
    void initialize_special_block(int &from);


    /**
     * @brief Adds connection on this blocks out_pin to another blocks to_input_pin
     * @param out_pin
     * @param to_input_pin
     * @param block
     */
    void    add_connection(QPointF out_pin, QPointF to_input_pin, int block);
    /**
     * @brief Defines input of a block, and when all inputs are defined sets am_i_defined to true
     * @param input Input that was defined
     */
    void    set_input_defined(QPointF input);
    /**
     * @brief Returns call code
     * @return Returns call code
     */
    QString call_code() {return my_call_code;};



};

#endif // BLOCKINSCENE_H
