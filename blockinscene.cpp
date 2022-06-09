/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: blockinscene.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of BlockInScene class, file was generated through Qt but filled by hand.
 *
 */

#include "blockinscene.h"
#include <QDebug>

int BlockInScene::object_count = 0;

BlockInScene::BlockInScene(QString group_name, QPointF position, int in_count, int out_count, QStringList inputs, QStringList outputs, int &var_names_from):
    my_position{position},
    name_of_group{group_name},
    input_count{in_count},
    output_count{out_count}

{
    am_i_special_block = false;
    am_i_out = false;
    am_i_in = false;
    am_i_defined = false;
    my_number = object_count;
    object_count++;
    in_out_types = {};
    in_out_var_name = {};
    initialize_positions(false);
    initialize_inouttypes(inputs,outputs);
    already_processed_inputs = {};
    already_processed_outputs = {};
    other_block = {};
    other_block_input = {};
    connected_output = {};
    connected_to_input = {};
    my_call_code = "";
    initiallize_varnames_and_call(var_names_from);

    //initialize_myname();
}


void BlockInScene::initialize_positions(bool special)
{

    for(int i = 0; i < input_count; i++){
        if(special){
            QPointF tmp(my_position.x(), my_position.y() + TOPIN * (i+1));
            input_positions << tmp;
            continue;
        }
        QPointF tmp(my_position.x() - TOPIN, my_position.y() + TOPIN * (i+1));
        input_positions << tmp;
    }
    for(int i = 0; i < output_count; i++){
        if(special){
            QPointF tmp(my_position.x() + CONSTBLOCKSWIDTH + TOPIN, my_position.y() + TOPIN * (i+1));
            output_positions << tmp;
            continue;
        }
        QPointF tmp(my_position.x() + BLOCKWIDTH + TOPIN, my_position.y() + TOPIN * (i+1));
        output_positions << tmp;
    }

    //qDebug() << input_positions;
    //qDebug() << output_positions;
}


void BlockInScene::initialize_inouttypes(QStringList inputs, QStringList outputs)
{
    for(int i = 0; i < inputs.count(); i = i+2){
        QStringList tmp = inputs[i].split(" ");
        in_out_types << tmp[0]; //get the types
    }
    for(int i = 0; i < outputs.count(); i = i+2){
        QStringList tmp = outputs[i].split(" "); //e.g.: int a -> int,a
        in_out_types << tmp[0]; //get the types
    }
    //qDebug() << in_out_types;
}

void BlockInScene::initiallize_varnames_and_call(int &from)
{
    //initializes all var names
    for(int i = 0; i < input_count; i++){
        QVariant tmp(from);
        in_out_var_name.insert(("_" + tmp.toString()),input_positions[i]);
        from++;
    }
    for(int i = 0; i < output_count; i++){
        QVariant tmp(from);
        in_out_var_name.insert(("_" + tmp.toString()),output_positions[i]);
        from++;
    }
    //initializes call string
    my_call_code += name_of_group + "(";
    for(int i = 0; i < in_out_var_name.size(); i++){
        if(i < input_count){
            my_call_code += in_out_var_name.key(input_positions[i]);
        }else{
            my_call_code += in_out_var_name.key(output_positions[i-input_count]);
        }

        if(!((i+1) == in_out_var_name.size())){
            my_call_code += ",";
        }
    }
    my_call_code += ")";
    //qDebug() << my_call_code;
}
// //////////////////////////////////

BlockInScene::BlockInScene(QString group_name, QPointF position, int &var_names_from):
    my_position{position},
    name_of_group{group_name}
{
    am_i_special_block = true;
    am_i_defined = false;
    am_i_in = false;
    my_number = object_count;
    object_count++;
    in_out_types = {};
    in_out_var_name = {};
    initialize_special_block(var_names_from);
    already_processed_inputs = {};
    already_processed_outputs = {};
    other_block = {};
    other_block_input = {};
    connected_output = {};
    connected_to_input = {};
}

void BlockInScene::initialize_special_block(int &from)
{
    QRegularExpression regex1("^CONST.*:");  //no input/output
    QRegularExpressionMatch match1 = regex1.match(name_of_group);
    if(match1.hasMatch()){
        output_count = 1;
        input_count = 0;
        am_i_defined = true; //im not sure about this line same with IN

        QStringList tmplist = name_of_group.split(":");
        tmplist = tmplist[0].split(" ");
        in_out_types.append(tmplist[1].toLower());
        initialize_positions(true);
        QVariant tmp(from);

        in_out_var_name.insert(("_" + tmp.toString()),output_positions[0]);
        from++;
        return;
    }

    QRegularExpression regex2("^IN:");  //no input/output
    QRegularExpressionMatch match2 = regex2.match(name_of_group);
    if(match2.hasMatch()){
        output_count = 1;
        input_count = 0;
        am_i_defined = true;
        am_i_in = true;

        QStringList tmplist = name_of_group.split(":");
        tmplist = tmplist[1].split(" ");
        in_out_types.append(tmplist[0]);
        initialize_positions(true);
        //QVariant tmp(from);

        //in_out_var_name.insert(("_" + tmp.toString()),output_positions[0]);
        in_out_var_name.insert(((name_of_group.split(":")[1]).split(",")[0]).split(" ")[1],output_positions[0]);
        //from++;
        return;
    }
    //OUT
    output_count = 0;
    input_count = 1;
    am_i_defined = false;
    am_i_out = true;

    QStringList tmplist = name_of_group.split(":");
    tmplist = tmplist[1].split(" ");
    in_out_types.append(tmplist[0]);
    initialize_positions(true); //true/false does not matter here
    //QVariant tmp(from);

    in_out_var_name.insert(((name_of_group.split(":")[1]).split(",")[0]).split(" ")[1],input_positions[0]);
    //from++;

}

// /////////////////////////////////////

void BlockInScene::set_input_defined(QPointF input)
{
    already_processed_inputs.append(input);
    if(already_processed_inputs.count() == input_count){
        am_i_defined = true;
    }
}

void BlockInScene::add_connection(QPointF out_pin, QPointF to_input_pin, int block)
{
    other_block.append(block);//in sync with other_block_input
    other_block_input.append(to_input_pin);
    connected_output.append(out_pin); //in sync with connected_to_input
    connected_to_input.append(to_input_pin);
}
