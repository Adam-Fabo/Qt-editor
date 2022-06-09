/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: compiler.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of Compiler class, file was generated through Qt but filled by hand.
 *
 */

#include "compiler.h"

Compiler::Compiler(QObject *parent, QVector<QGraphicsItem *> new_blockList, QVector<QString> new_blockListStrings, QVector<QGraphicsLineItem *> new_lineList, bool scene, QString filepath) : QObject(parent)
{
    blockList = new_blockList;
    blockListStrings = new_blockListStrings;
    lineList = new_lineList;

    fully_processed_points = {};
    visited_points = {};

    all_blocks = {};
    fully_processed_blocks = {};
    code = "";
    new_input_defined = false;
    force_defined = false;
    counter = 0;
    upstream_list = {};
    compiling_for_scene = scene;
    where_to_write = filepath;
    init_code = "";
    already_defined_functions = {};

}

Compiler::~Compiler()
{
    if(all_blocks.count() != 0) {
        all_blocks[0]->object_count = 0;
    }
    for(auto pointer: all_blocks){

        delete(pointer);
    }
    for(auto pointer: info_about_blocks){
        delete(pointer);
    }

}

// /////////////////////////////////////////
void Compiler::compile()
{

    prepare_to_compile();


    code += "// //" + already_defined_functions.join(":") + "\n"; //first line in generated code
    generate_head();


    //processing scene
    process_const_blocks();
    while(new_input_defined){
        new_input_defined = false;
        process_other_blocks();
    }

    if(fully_processed_blocks.count() != block_count()){ //handle blocks not connected to in/const
        process_rest_of_scene();
    }

    //TODO loops


    if(compiling_for_scene){
        code += "return 0;}\n";
        save_compiled_scene();
    }else{
        code += "return;}\n";
    }


    //qDebug() << code;
}


void Compiler::prepare_to_compile(){


    prepare_data_from_files();


    QRegularExpression regex("^(CONST|IN:|OUT:).*");
    for(int i = 0; i < blockListStrings.count(); i++){
        QRegularExpressionMatch match = regex.match(blockListStrings[i]);
        if((match.hasMatch())){
            BlockInScene *tmp = new BlockInScene(blockListStrings[i],blockList[i]->pos(),counter);

            all_blocks.append(tmp);

            if((blockListStrings[i].startsWith("CONST"))){
                generate_init_of_vars(tmp->my_number,true);
            }


        }
    }
    for(int i = 0; i < blockListStrings.count(); i++){
        QRegularExpressionMatch match = regex.match(blockListStrings[i]);
        if(!(match.hasMatch())){
            int index = already_defined_functions.indexOf(blockListStrings[i]);
            BlockInfoStruct * info_struct = info_about_blocks[index];


            BlockInScene *tmp = new BlockInScene(blockListStrings[i],blockList[i]->pos(),info_struct->inputs.count()/2,info_struct->outputs.count()/2,info_struct->inputs,info_struct->outputs,counter);

            all_blocks.append(tmp);
            generate_init_of_vars(tmp->my_number,false);

        }
    }
}


void Compiler::prepare_data_from_files()
{
    tmpmodel = new AppModel();
    connect(tmpmodel,SIGNAL(block_file_read(QStringList,QStringList,QStringList)),this,SLOT(file_read(QStringList,QStringList,QStringList)));
    for(int i = 0; i < blockListStrings.count(); i++){
        if(already_defined_functions.contains(blockListStrings[i]) || blockListStrings[i].startsWith("CONST") || blockListStrings[i].startsWith("IN:") || blockListStrings[i].startsWith("OUT:")){
            continue; //dont open the same file twice
        }
        tmpmodel->open_block_by_name(blockListStrings[i]);
    }




    delete(tmpmodel);
}


void Compiler::file_read(QStringList cat_name_desc_code, QStringList inputs, QStringList outputs)
{
    QString code_from_file = cat_name_desc_code[3];
    BlockInfoStruct *tmp = new BlockInfoStruct;
    tmp->cat_name_desc_code = cat_name_desc_code;
    tmp->inputs = inputs;
    tmp->outputs = outputs;
    already_defined_functions.append(cat_name_desc_code[1]);
    info_about_blocks.append(tmp);
    if(code_from_file.startsWith("// //")){ //composite
        check_composite_dependencies((code_from_file.split("\n"))[0]);
        QStringList lines = code_from_file.split("\n");
        for(int i = 0; i < lines.count(); i++){
            if(lines[i].startsWith("void " + cat_name_desc_code[1] + "(")){ //find where the function is defined
                while(lines[i] != "return;}"){
                    init_functions_code += lines[i] + "\n";
                    i++;
                }

                init_functions_code += lines[i] + "\n"; //add return
                break;
            }
        }

    }else{ //simple block
        init_functions_code += code_from_file + "\n";
    }
}

void Compiler::check_composite_dependencies(QString using_blocks)
{
    using_blocks = using_blocks.split("// //")[1];
    QStringList using_individual_blocks = using_blocks.split(":");
    for(const auto name: using_individual_blocks){
        if(name == "" || already_defined_functions.contains(name)){
            continue;
        }
        tmpmodel->open_block_by_name(name);
    }


}







void Compiler::generate_init_of_vars(int block, bool is_const)
{
    //my_number of a block should corespond to its position in all_blocks
    if(is_const){ //e.g: int _0 = 22;
        init_code += all_blocks[block]->in_out_types[0] + " " + all_blocks[block]->in_out_var_name.key(all_blocks[block]->output_positions[0]) + "=" + (all_blocks[block]->name_of_group.split(":")[1]) + ";\n";
    }else{ //e.g: int _0;
        for(int i = 0; i < all_blocks[block]->input_count; i++){
            init_code += all_blocks[block]->in_out_types[i] + " " + all_blocks[block]->in_out_var_name.key(all_blocks[block]->input_positions[i]) + ";\n";
        }

        for(int i = 0; i < all_blocks[block]->output_count; i++){
            init_code += all_blocks[block]->in_out_types[i+all_blocks[block]->input_count] + " " + all_blocks[block]->in_out_var_name.key(all_blocks[block]->output_positions[i]) + ";\n";
        }

    }

}















void Compiler::process_const_blocks()
{
    for(auto i: all_blocks){
       if(i->am_i_special_block && !(i->am_i_out)){ //process IN and CONST
          // qDebug() << "const";
           process_outputs(i);
           fully_processed_blocks.append(i->my_number);
       }
    }
}

void Compiler::process_other_blocks()
{
    for(auto i: all_blocks){
       if(!(i->am_i_special_block) && !(i->am_i_out)){ //process normal blocks
           if(i->am_i_defined && !(fully_processed_blocks.contains(i->my_number))){
             process_outputs(i);
             fully_processed_blocks.append(i->my_number);
           }
       }
    }
}


void Compiler::process_rest_of_scene()
{
    bool in = false;
    for(auto i: all_blocks){ //first process blocks with no input
       if(!(i->am_i_special_block) && !(i->am_i_out)){ //process IN and CONST
           if(!(i->am_i_defined) && !(fully_processed_blocks.contains(i->my_number))){
             if(i->input_count == 0){
                in = true;
                process_undefined_block(i);

             }
           }
       }
       while(new_input_defined){
           new_input_defined = false;
           process_other_blocks();
       }
    } //first process blocks with no input




    while(fully_processed_blocks.count() != block_count()){ //and now process rest
        for(auto i: all_blocks){
           if(!(i->am_i_special_block) && !(i->am_i_out)){ //process IN and CONST
               if(!(i->am_i_defined) && !(fully_processed_blocks.contains(i->my_number))){
                 in = true;
                 process_undefined_block(i);
                 upstream_list = {};
                 force_defined = false;
                 break;
               }
           }
        }
        if(!in){ //most likely because only unconected OUTs remain
            for(auto i: all_blocks){
                if(i->am_i_out){
                    if(!(fully_processed_blocks.contains(i->my_number))){
                      fully_processed_blocks.append(i->my_number);
                    }
                }
            }
        }
        in = false;
        while(new_input_defined){
            new_input_defined = false;
            process_other_blocks();
        }
    }
}

void Compiler::process_undefined_block(BlockInScene *block)
{
    if(block->input_count == 0){ //IN has a bigger value than block without inputs -> IN will be connected first
        generate_code_call_block(block->call_code());
        if(block->output_count == 0){
            fully_processed_blocks.append(block->my_number);
            return;
        }
        block->am_i_defined = true;
        new_input_defined = true;
        return;
    }

    for(int i = 0; i < block->input_count; i++){
        if(!(block->already_processed_inputs.contains(block->input_positions[i]))){
            if(upstream_list.contains(block->my_number)){ //loop
                force_define_block(block,block->input_positions[i]);
                return;
            }
            upstream_list.append(block->my_number);

            visited_points = {};
            go_upstream(block,block->input_positions[i],block->input_positions[i]);
            if(force_defined == false){ //if recursion returns and it didnt find any dependency , define this block
                force_define_block(block,block->input_positions[i]);
            }
            visited_points = {};
            return; //process one undefined input
        }
    }

}

void Compiler::force_define_block(BlockInScene *block, QPointF input_pos)
{
    new_input_defined = true;
    force_defined = true;

    block->set_input_defined(input_pos);
    if(block->am_i_defined && (block->am_i_out == false)){
        generate_code_call_block(block->call_code());
    }
}

void Compiler::process_outputs(BlockInScene *block)
{
    for(int i = 0; i < block->output_count; i++){
        process_line(block,block->output_positions[i],block->output_positions[i]);
        visited_points = {};
    }
}

void Compiler::process_line(BlockInScene *block, QPointF output_pos, QPointF point)
{
    if(fully_processed_points.contains(point)){ //if this point was procesed, we dont need to go through it
        return;
    }
    if(visited_points.contains(point)){ //so no infinite loop happens
        return;
    }
    visited_points.append(point);
    for(int i = 0; i < all_blocks.count(); i++){ //check for input connections
        for(int j = 0; j < all_blocks[i]->input_count; j++){
            if(point == all_blocks[i]->input_positions[j]){ //match
                if(!(all_blocks[i]->already_processed_inputs.contains(all_blocks[i]->input_positions[j]))){ //only if the input is not yet connected connect it
                    block->add_connection(output_pos, point, all_blocks[i]->my_number); //maps the connection
                    all_blocks[i]->set_input_defined(point);
                    new_input_defined = true;
                    generate_code_for_connection(all_blocks[i],block,point,output_pos);

                    if(all_blocks[i]->am_i_defined && (all_blocks[i]->am_i_out == false)){
                        generate_code_call_block(all_blocks[i]->call_code());

                    }else if(all_blocks[i]->am_i_out){
                        fully_processed_blocks.append(all_blocks[i]->my_number); //add OUT block to processed blocks
                    }
                    break; //one block cant have multiple inputs with same position
                }
            }
        }
    }

    for(int i = 0; i < lineList.count(); i++){ //go through lines
        if(lineList[i]->line().p1() == lineList[i]->line().p2()){ //such a line(point) has no value
            continue;
        }
        if(point == lineList[i]->line().p1()){
            process_line(block, output_pos, lineList[i]->line().p2()); //look at the other side of a line
        }
        if(point == lineList[i]->line().p2()){
            process_line(block, output_pos, lineList[i]->line().p1());
        }
    }

    fully_processed_points.append(point);
}


void Compiler::go_upstream(BlockInScene *block, QPointF input_pos, QPointF point)
{


    if(visited_points.contains(point)){ //so no infinite loop happens
        return;
    }
    visited_points.append(point);

    for(int i = 0; i < all_blocks.count(); i++){ //are we on output
        for(int j = 0; j < all_blocks[i]->output_count; j++){
            if(point == all_blocks[i]->output_positions[j]){
                process_undefined_block(all_blocks[i]); //continue with previous block
                return;
            }
        }
    }


    for(int i = 0; i < lineList.count(); i++){ //go through lines

        if(lineList[i]->line().p1() == lineList[i]->line().p2()){ //such a line(point) has no value
            continue;
        }
        if(point == lineList[i]->line().p1()){

            go_upstream(block, input_pos, lineList[i]->line().p2()); //look at the other side of a line
            if(force_defined){ //no need to look further
                return;
            }
            continue;
        }
        if(point == lineList[i]->line().p2()){

            go_upstream(block, input_pos, lineList[i]->line().p1());
            if(force_defined){ //no need to look further
                return;
            }
            continue;
        }
    }

}






void Compiler::generate_code_for_connection(BlockInScene *in_block, BlockInScene *out_block, QPointF input, QPointF output)
{
    QString tmpin = in_block->in_out_var_name.key(input);
    QString tmpout = out_block->in_out_var_name.key(output);

    code += tmpin + " = " + tmpout + ";\n";

}


void Compiler::generate_code_call_block(QString call)
{

    code += call + ";\n";
}

void Compiler::generate_head()
{
    code += init_functions_code;
    if(compiling_for_scene){
        code += "int main(){\n";
        code += init_code;
        return;
    }else{
        code += "void " + where_to_write.split("/").last() + "(";
        QStringList inputs = {};
        QStringList outputs = {};
        for(int i = 0 ; i < all_blocks.count();i++){
            if(all_blocks[i]->name_of_group.startsWith("IN:")){
                inputs.append((all_blocks[i]->name_of_group.split(":")[1]).split(",")[0]);
            }else if(all_blocks[i]->name_of_group.startsWith("OUT:")){
                outputs.append((all_blocks[i]->name_of_group.split(":")[1]).split(",")[0]);
            }
        }

        for(int i = 0; i < inputs.count(); i++){

             code += inputs[i];
             if(i+1 != inputs.count()){
                 code += ",";
             }else{
                 if(outputs.count() == 0){
                     break;
                 }
                 code += ",";
             }


        }
        for(int i = 0; i < outputs.count(); i++){

              code += adjust_out(outputs[i]);
             if(i+1 != outputs.count()){
                 code += ",";
             }else{
                 break;
             }

        }
        code += "){\n";
        code += init_code;
    }
}

QString Compiler::adjust_out(QString out_variable)
{
    QStringList list = out_variable.split(" ");
    return list.join(" &");
}



void Compiler::save_compiled_scene()
{
    //qDebug() << where_to_write;
    QStringList path = where_to_write.split("/");
    path.last().prepend("compiled_");


    QFile block_file_handle(path.join("/"));
    if (!block_file_handle.open(QIODevice::WriteOnly| QIODevice::Text)){ //::Text converts \r\n to \n
            qDebug("Could not open file when compiling!");
            return;
    }


    QTextStream scene_file(&block_file_handle);
    scene_file << code;
    block_file_handle.close();
}
