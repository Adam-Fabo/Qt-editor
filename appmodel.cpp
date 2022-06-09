/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: appmodel.cpp
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains definition of AppModel class, file was generated through Qt but filled by hand.
 *
 */

#include "appmodel.h"
#include "compiler.h"
#include <QDebug>

AppModel::AppModel()
{
    exec_path = new QDir(QCoreApplication::applicationDirPath() + "/library"); //get path to exe file
    basic_lib_path = new QDir(exec_path->absolutePath() + "/basicblocks");
    basic_blocks_model = new QFileSystemModel;
    basic_blocks_model->setRootPath(exec_path->absolutePath());
    basic_blocks_model->setReadOnly(false);

    composite_lib_path = new QDir(exec_path->absolutePath() + "/composite");
}

AppModel::~AppModel()
{
    delete composite_lib_path;
    delete basic_blocks_model;
    delete basic_lib_path;
    delete exec_path;
}

/*
 * Emits a signal with categories(dirs) found in library
 * can also add a category specified in new_category, if thet category exists emits category_already_exists signal
 */
void AppModel::check_categories(QString new_category)
{
    if(!exec_path->exists()){
        basic_lib_path->mkpath("."); //if dir /library is not present create one
    }
    QDir tmp(basic_lib_path->absolutePath() + "/----None----");
    tmp.mkpath("."); //create None category

    basic_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
    QStringList list = basic_lib_path->entryList();

    if(new_category != ""){
        QDir tmp(basic_lib_path->absolutePath() + "/" + new_category);
        if(tmp.exists()){
            emit category_already_exists();
            return;
        }
        tmp.mkpath(".");
        list << new_category;
    }
    //qDebug() << basic_lib_path->absolutePath();
    emit send_categories(list);
}

/*
 * Emits a signal with categories(dirs) found in library
 * can also add a category specified in new_category, if thet category exists emits category_already_exists signal
 */
void AppModel::check_composite_categories(QString new_category)
{
    if(!exec_path->exists()){
        composite_lib_path->mkpath("."); //if dir /library is not present create one
    }
    QDir tmp(composite_lib_path->absolutePath() + "/----None----");
    tmp.mkpath("."); //create None category

    composite_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
    QStringList list = composite_lib_path->entryList();

    if(new_category != ""){
        QDir tmp(composite_lib_path->absolutePath() + "/" + new_category);
        if(tmp.exists()){
            emit category_already_exists();
            return;
        }
        tmp.mkpath(".");
        list << new_category;
    }

    emit send_composite_categories(list);
}

/*
 * Saves new block under category
 */
void AppModel::save_new_block(QStringList cat_name_desc_code, QStringList inputs, QStringList outputs)
{
    basic_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
    QStringList list = basic_lib_path->entryList();
    for(const auto& cat : list){ //check if block already exists
        if(basic_lib_path->exists(cat + '/' + cat_name_desc_code[1])){ //if file exist remove it
            basic_lib_path->remove(cat + '/' + cat_name_desc_code[1]);
            //emit warn_saving("This block already exists!");
            //return;
        }
    }

    QFile block_file_handle(basic_lib_path->absolutePath() + '/' + cat_name_desc_code[0] + '/' + cat_name_desc_code[1]);
    if (!block_file_handle.open(QIODevice::WriteOnly | QIODevice::Text)){ //::Text converts \r\n to \n

            emit warn_saving("Block could not be created, file can't be opened!");
            return;
    }

    QTextStream block_file(&block_file_handle);
    block_file << "#isablock\n" << cat_name_desc_code[0] << '\n' << "===\n";
    block_file << (inputs.count()/2) << '\n';
    for(int i = 0; i < inputs.count(); i++){
        if(i%2 == 0){
            block_file << "in:" << inputs[i] << '\n';
        }else{
            block_file << "iname:" << inputs[i] << '\n';
        }
    }
    block_file << "===\n";
    block_file << (outputs.count()/2) << '\n';
    for(int i = 0; i < outputs.count(); i++){
        if(i%2 == 0){
            block_file << "out:" << outputs[i] << '\n';
        }else{
            block_file << "oname:" << outputs[i] << '\n';
        }
    }
    block_file << "===\n";
    block_file << cat_name_desc_code[2] << '\n' << "===\n";
    block_file << "void " << cat_name_desc_code[1] << "(";
    for(int i = 0; i < inputs.count(); i++){
        if(i%2 == 0){
            block_file << inputs[i];
            if(i+2 != inputs.count()){
                block_file << ",";
            }else{
                if(outputs.count() == 0){
                    break;
                }
                block_file << ",";
            }
        }

    }
    for(int i = 0; i < outputs.count(); i++){
        if(i%2 == 0){
            block_file << outputs[i];
            if(i+2 != outputs.count()){
                block_file << ",";
            }else{
                break;
            }
        }
    }
    block_file << "){\n";
    block_file << cat_name_desc_code[3] << '\n';
    block_file << "return;}";


    block_file_handle.close();
    emit block_saved_succesfully(true); //finished saving
}


void AppModel::filesystemmodel()
{
    emit send_filemodel(basic_blocks_model);
}


void AppModel::open_remove_action(QModelIndex object, QString cat_or_block)
{
    if(cat_or_block == "cat"){ //removes category with all its content
        if(object.sibling(object.row(),0).data().toString() == "----None----"){
            //qDebug() << "cant remove this category";
            return;
        }
        if(!basic_blocks_model->rmdir(object)){
            QDir tmp(basic_lib_path->absolutePath() + "/" + object.sibling(object.row(),0).data().toString());
            tmp.removeRecursively();
        }

    }else{ // removes a single block
        basic_blocks_model->remove(object);
    }
}

void AppModel::open_block(QModelIndex object)
{
    QString pathtoblock;
    bool found_path = true;


    // vyskusat najprv atomicke bloky
    basic_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
    QStringList list = basic_lib_path->entryList();
    for(const auto& cat : list){ //check for the path of block
        if(basic_lib_path->exists(cat + '/' + object.sibling(object.row(),0).data().toString())){
            pathtoblock = cat + '/' + object.sibling(object.row(),0).data().toString();
        }
    }
    //qDebug() << "path:" <<basic_lib_path->absolutePath() + "/" + pathtoblock;
    QFile block_file_handle(basic_lib_path->absolutePath() + "/" + pathtoblock);
    if (!block_file_handle.open(QIODevice::ReadOnly | QIODevice::Text)){ //::Text converts \r\n to \n
            //qDebug() << basic_lib_path->absolutePath() + pathtoblock;
        found_path = false;
        //emit warn_saving("Block could not be read, file can't be opened!"); //warn_saving is not a great name beacause it warns when opening as well
            //return;
    }else{
        QTextStream block_file(&block_file_handle);
        QString line = "";
        QStringList cat_name_desc_code;
        QStringList inputs;
        QStringList outputs;


        line = block_file.readLine();
        if(line == "#isacompositeblock"){
            while((line = block_file.readLine()) != "#endofdesc"){
                ;
            }
            line = block_file.readLine();
        }
        if(line != "#isablock"){
            emit warn_saving("File could not be recognized as a block! (format)");
            return;
        }
        line = "";
        line = block_file.readLine();
        cat_name_desc_code << line; //cat
        cat_name_desc_code << object.sibling(object.row(),0).data().toString(); //name

        line = "";
        line = block_file.readLine();
        if(line != "==="){
            emit warn_saving("File could not be recognized as a block! (format)");
            return;
        }
        line = block_file.readLine(); //number of inputs
        line = "";
        while((line = block_file.readLine()) != "==="){ //inputs
            QStringList list = line.split(":");
            inputs << list[1];
        }
        line = block_file.readLine(); // number of outputs
        while((line = block_file.readLine()) != "==="){ //outputs
            QStringList list = line.split(":");
            outputs << list[1];
        }

        cat_name_desc_code << block_file.readLine(); //description
        line = block_file.readLine();

        //code
        line = block_file.readLine(); //skip first
        line = "";
        QString code = "";
        while((line = block_file.readLine()) != "return;}"){
            code += line;
            code += "\n";
        }
        int last_newline = code.lastIndexOf("\n");
        cat_name_desc_code << code.left(last_newline);
        block_file_handle.close();
        emit block_file_read(cat_name_desc_code, inputs, outputs);
        return;
    }

    // ak nenajde atomicky block tak skusit kompozirtne
    if(!found_path){

        found_path = true;
        composite_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
        QStringList list = composite_lib_path->entryList();
        for(const auto& cat : list){ //check for the path of block
            if(composite_lib_path->exists(cat + '/' + object.sibling(object.row(),0).data().toString())){
                pathtoblock = cat + '/' + object.sibling(object.row(),0).data().toString();
            }
        }

        QFile composite_file_handle(composite_lib_path->absolutePath() + "/" + pathtoblock);
        if (!composite_file_handle.open(QIODevice::ReadOnly | QIODevice::Text)){ //::Text converts \r\n to \n
                //qDebug() << basic_lib_path->absolutePath() + pathtoblock;
                found_path = false;
            //emit warn_saving("Block could not be read, file can't be opened!"); //warn_saving is not a great name beacause it warns when opening as well
                //return;
        }else{
            QTextStream block_file(&composite_file_handle);
            QString line = "";
            QStringList cat_name_desc_code;
            QStringList inputs;
            QStringList outputs;


            line = block_file.readLine();

            if(line == "#isacompositeblock"){

                while((line = block_file.readLine()) != "#endofdesc"){
                    ;
                }
                line = block_file.readLine();
            }

            if(line != "#isablock"){
                emit warn_saving("File could not be recognized as a block! (format)");
                return;
            }
            line = "";
            line = block_file.readLine();
            cat_name_desc_code << line; //cat
            cat_name_desc_code << object.sibling(object.row(),0).data().toString(); //name

            line = "";
            line = block_file.readLine();
            if(line != "==="){
                emit warn_saving("File could not be recognized as a block! (format)");
                return;
            }
            line = block_file.readLine(); //number of inputs
            line = "";
            while((line = block_file.readLine()) != "==="){ //inputs
                QStringList list = line.split(":");
                inputs << list[1];
            }
            line = block_file.readLine(); // number of outputs
            while((line = block_file.readLine()) != "==="){ //outputs
                QStringList list = line.split(":");
                outputs << list[1];
            }

            cat_name_desc_code << block_file.readLine(); //description
            line = block_file.readLine();

            //code
            line = block_file.readLine(); //skip first
            line = "";
            QString code = "";
            while((line = block_file.readLine()) != "return;}"){
                code += line;
                code += "\n";
            }
            int last_newline = code.lastIndexOf("\n");
            cat_name_desc_code << code.left(last_newline);
            block_file_handle.close();

            emit block_file_read(cat_name_desc_code, inputs, outputs);
            return;
        }


        if(!found_path){
            emit warn_saving("Block could not be read, file can't be opened!"); //warn_saving is not a great name beacause it warns when opening as well
            return;
        }
    }
}


void AppModel::open_block_by_name(QString object)
{
    QString pathtoblock;
    bool found_path = true;
    basic_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
    QStringList list = basic_lib_path->entryList();
    for(const auto& cat : list){ //check for the path of block
        if(basic_lib_path->exists(cat + '/' + object)){
            pathtoblock = cat + '/' + object;
        }
    }

    QFile block_file_handle(basic_lib_path->absolutePath() + "/" + pathtoblock);
    if (!block_file_handle.open(QIODevice::ReadOnly | QIODevice::Text)){ //::Text converts \r\n to \n
            found_path = false;

    }else{


        QTextStream block_file(&block_file_handle);
        QString line = "";
        QStringList cat_name_desc_code;
        QStringList inputs;
        QStringList outputs;

        line = block_file.readLine();
        if(line != "#isablock"){
            emit warn_saving("File could not be recognized as a block! (format)");
            return;
        }
        line = "";
        line = block_file.readLine();
        cat_name_desc_code << line; //cat
        cat_name_desc_code << object; //name

        line = "";
        line = block_file.readLine();
        if(line != "==="){
            emit warn_saving("File could not be recognized as a block! (format)");
            return;
        }
        line = block_file.readLine(); //number of inputs
        line = "";
        while((line = block_file.readLine()) != "==="){ //inputs
            QStringList list = line.split(":");
            inputs << list[1];
        }
        line = block_file.readLine(); // number of outputs
        while((line = block_file.readLine()) != "==="){ //outputs
            QStringList list = line.split(":");
            outputs << list[1];
        }

        cat_name_desc_code << block_file.readLine(); //description
        line = block_file.readLine(); //===
        //code
        QString code = "";
        while(!((line = block_file.readLine()).isNull())){ //while not eof read
            code += line;
            code += "\n";
        }

        int last_newline = code.lastIndexOf("\n");
        cat_name_desc_code << code.left(last_newline);
        block_file_handle.close();
        emit block_file_read(cat_name_desc_code, inputs, outputs);
        return;
    }



    if(!found_path){

        found_path = true;
        composite_lib_path->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //filter for entryList
        QStringList list = composite_lib_path->entryList();
        for(const auto& cat : list){ //check for the path of block
            if(composite_lib_path->exists(cat + '/' + object)){
                pathtoblock = cat + '/' + object;
            }
        }

        QFile composite_file_handle(composite_lib_path->absolutePath() + "/" + pathtoblock);

        if (!composite_file_handle.open(QIODevice::ReadOnly | QIODevice::Text)){ //::Text converts \r\n to \n

                found_path = false;

        }else{
            QTextStream block_file(&composite_file_handle);
            QString line = "";
            QStringList cat_name_desc_code;
            QStringList inputs;
            QStringList outputs;


            line = block_file.readLine();
            if(line == "#isacompositeblock"){

                while((line = block_file.readLine()) != "#endofdesc"){
                    ;
                }
                line = block_file.readLine();
            }

            if(line != "#isablock"){
                emit warn_saving("File could not be recognized as a block! (format)");
                return;
            }
            line = "";
            line = block_file.readLine();

            cat_name_desc_code << line; //cat
            cat_name_desc_code << object; //name

            line = "";
            line = block_file.readLine();
            if(line != "==="){
                emit warn_saving("File could not be recognized as a block! (format)");
                return;
            }
            line = block_file.readLine(); //number of inputs
            line = "";
            while((line = block_file.readLine()) != "==="){ //inputs
                QStringList list = line.split(":");
                inputs << list[1];
            }
            line = block_file.readLine(); // number of outputs
            while((line = block_file.readLine()) != "==="){ //outputs
                QStringList list = line.split(":");
                outputs << list[1];
            }

            cat_name_desc_code << block_file.readLine(); //description
            line = block_file.readLine();

            //code
            QString code = "";
            while(!((line = block_file.readLine()).isNull())){ //while not eof read
                code += line;
                code += "\n";
            }

            int last_newline = code.lastIndexOf("\n");
            cat_name_desc_code << code.left(last_newline);
            block_file_handle.close();
            emit block_file_read(cat_name_desc_code, inputs, outputs);
            return;
        }


        if(!found_path){
            emit warn_saving("Block could not be read, file can't be opened!"); //warn_saving is not a great name beacause it warns when opening as well
            return;
        }
    }
}


void AppModel::saveSceneAs(QVector<QGraphicsItem *> blockList, QVector<QString> blockListStrings, QVector<QGraphicsItem *> commentList, QVector<QString> commentListStrings, QVector<QGraphicsLineItem *> lineList)
{

    fileName = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
                               ".",
                               tr("(*.scn) [^.]*"));


    std::ofstream dstFile;

    dstFile.open(fileName.toStdString());



    for(int i = 0; i< blockList.length();i++){

        dstFile << blockListStrings[i].toStdString() << "\n";
        dstFile << blockList[i]->pos().rx() << "\n";
        dstFile << blockList[i]->pos().ry() << "\n";

    }

    for(int i = 0; i< commentList.length();i++){

        dstFile << "#COMMENT" << "\n";
        dstFile << commentList[i]->pos().rx() << "\n";
        dstFile << commentList[i]->pos().ry() << "\n";
        dstFile << commentListStrings[i].toStdString() << "\n";

    }

    for(int i = 0; i< lineList.length();i++){

        dstFile << "#LINE" << "\n";

        dstFile << lineList[i]->line().p1().rx() << "\n";
        dstFile << lineList[i]->line().p1().ry() << "\n";

        dstFile << lineList[i]->line().p2().rx() << "\n";
        dstFile << lineList[i]->line().p2().ry() << "\n";

    }


    dstFile.close();

}

void AppModel::loadScene(Scene *scene)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File",
                               ".",
                               "(*.scn *)");

    QPoint pos1,pos2;

    std::ifstream input( fileName.toStdString() );

    for( std::string line; getline( input, line ); )
    {

        if(line.rfind("#isacompositeblock",0) == 0){
            continue;

        }

        if(line.rfind("#endofdesc",0) == 0){
            break;
        }


        std::string tmp;
        getline( input, tmp );
        pos1.setX(QString::fromStdString(tmp).toInt());

        getline( input, tmp );
        pos1.setY(QString::fromStdString(tmp).toInt());



        // konstanta
        if(line.find("CONST") != std::string::npos){
            scene->setBlockParamsNoPopUp({"","Constant",{},{}},{},{});

            if(line.find("INT:") != std::string::npos){
                scene->setStr(QString::fromStdString(line.erase(0,10)));
                scene->setConstInt();
            }else if(line.find("FLOAT:") != std::string::npos){
                scene->setStr(QString::fromStdString(line.erase(0,12)));
                scene->setConstFloat();
            }else if(line.find("BOOL:") != std::string::npos){
                scene->setStr(QString::fromStdString(line.erase(0,11)));
                scene->setConstBool();
            }else if(line.find("STRING:") != std::string::npos){
                scene->setStr(QString::fromStdString(line.erase(0,13)));
                scene->setConstString();
            }
            // konstanta



            scene->addBlock(pos1);

        // vstup
        }else if(line.find("IN:") != std::string::npos){
            //inputv
            scene->setBlockParamsNoPopUp({"","Input",{},{}},{},{});
            scene->setStr(QString::fromStdString(line.erase(0,3)));
            scene->addBlock(pos1);

        //vystup
        }else if(line.find("OUT:") != std::string::npos){
            //output
            scene->setBlockParamsNoPopUp({"","Output",{},{}},{},{});
            scene->setStr(QString::fromStdString(line.erase(0,4)));
            scene->addBlock(pos1);

        }else if(line.find("#COMMENT") != std::string::npos){
            //input
            scene->setBlockParamsNoPopUp({"","Comment",{},{}},{},{});

            getline( input, tmp );
            scene->setStr(QString::fromStdString(tmp));

            scene->addBlock(pos1);

       //ciara
        }else if(line.find("#LINE") != std::string::npos){

            getline( input, tmp );
            pos2.setX(QString::fromStdString(tmp).toInt());

            getline( input, tmp );
            pos2.setY(QString::fromStdString(tmp).toInt());

            QGraphicsLineItem * sceneline = new QGraphicsLineItem(pos1.rx(),pos1.ry(),pos2.rx(),pos2.ry());
            sceneline->setPen(QPen(Qt::black,5));
            scene->addItem(sceneline);
            scene->lineList.append(sceneline);

            //input
            //scene->addl
        }else{
            //normalny block
            connect(this, SIGNAL(block_file_read(QStringList,QStringList,QStringList)),scene,SLOT(setBlockParams(QStringList,QStringList,QStringList)));

            open_block_by_name(QString::fromStdString(line));
            scene->addBlock(pos1);
        }
    }
}


void AppModel::saveComposite(QString name, QString category,QString desc ,QVector<QGraphicsItem *> blockList, QVector<QString> blockListStrings, QVector<QGraphicsItem *> commentList, QVector<QString> commentListStrings, QVector<QGraphicsLineItem *> lineList)
{
    // otvorenie suboru
    QFile block_file_handle(composite_lib_path->absolutePath() + '/' + category + '/' + name);
    if (!block_file_handle.open(QIODevice::ReadWrite| QIODevice::Text)){ //::Text converts \r\n to \n
            emit warn_saving("Block could not be created, file can't be opened!");
            return;
    }


    QTextStream block_file(&block_file_handle);

    block_file << "#isacompositeblock\n";

    // zapis dat do suboru -podobne ako u sceny
    for(int i = 0; i< blockList.length();i++){

        block_file << blockListStrings[i] << "\n";
        block_file << blockList[i]->pos().rx() << "\n";
        block_file << blockList[i]->pos().ry() << "\n";

    }

    for(int i = 0; i< commentList.length();i++){

        block_file << "#COMMENT" << "\n";

        block_file << commentList[i]->pos().rx() << "\n";
        block_file << commentList[i]->pos().ry() << "\n";
        block_file << commentListStrings[i] << "\n";

    }

    for(int i = 0; i< lineList.length();i++){
        block_file << "#LINE" << "\n";

        block_file << lineList[i]->line().p1().rx() << "\n";
        block_file << lineList[i]->line().p1().ry() << "\n";

        block_file << lineList[i]->line().p2().rx() << "\n";
        block_file << lineList[i]->line().p2().ry() << "\n";
    }

    block_file << "#endofdesc\n";


    QStringList inputs;
    QStringList outputs;
    QStringList cat_name_desc_code;
    int info;


    for(int i = 0; i<blockListStrings.count(); i++){
        if(blockListStrings[i].startsWith("IN:")){
            QString tmp = blockListStrings[i];
            inputs.append(tmp.remove(0,3));

        }else if(blockListStrings[i].startsWith("OUT:")){
            QString tmp = blockListStrings[i];
            outputs.append(tmp.remove(0,4));
        }
    }

    cat_name_desc_code.append(category);
    cat_name_desc_code.append(name);
    cat_name_desc_code.append(desc);
    cat_name_desc_code.append("");

    inputs = format_inout("in",inputs,info);
    if(info == 1)
        return;

    outputs = format_inout("out",outputs,info);

    if(info == 1)
        return;


    // zapis dat do suboru - rovnako ako pri atomickom bloku
    block_file << "#isablock\n" << cat_name_desc_code[0] << '\n' << "===\n";
    block_file << (inputs.count()/2) << '\n';
    for(int i = 0; i < inputs.count(); i++){
        if(i%2 == 0){
            block_file << "in:" << inputs[i] << '\n';
        }else{
            block_file << "iname:" << inputs[i] << '\n';
        }
    }

    block_file << "===\n";
    block_file << (outputs.count()/2) << '\n';

    for(int i = 0; i < outputs.count(); i++){
        if(i%2 == 0){
            block_file << "out:" << outputs[i] << '\n';
        }else{
            block_file << "oname:" << outputs[i] << '\n';
        }
    }

    block_file << "===\n";
    block_file << cat_name_desc_code[2] << '\n' << "===\n";

    Compiler *kompiler = new Compiler(nullptr,blockList,blockListStrings,lineList,false,composite_lib_path->absolutePath() + '/' + category + '/' + name);
    kompiler->compile();

    block_file << kompiler->generated_code();
    delete(kompiler);

    emit block_saved_succesfully(true); //finished saving

    block_file_handle.close();

}


QStringList AppModel::format_inout(QString in_or_out,QStringList list, int &info)
{
    QStringList type_and_name;
    info = 0;

    QStringList par_split_comma;
    QStringList par_split_space;
    QRegularExpression regex2("^([0-9]+:)?[ \n\t]*$");  //ne
    QRegularExpressionMatch match2;

    for(const auto& par : list){
        match2 = regex2.match(par);
        if(match2.hasMatch()){
            continue;
        }
        if(par.indexOf(',') != par.lastIndexOf(',') || par.indexOf(',') == -1){  //more than 1 comma or no comma at all

            QMessageBox warning;
            warning.setWindowTitle("Warning");
            warning.setText("This category already exists!");
            warning.setIcon(QMessageBox::Warning);
            warning.exec();
            info = 1;
            return type_and_name;
        }

        par_split_comma = par.split(",");
        for(const auto& iter : par_split_comma){
            par_split_space = iter.split(" ");
            par_split_space.removeAll("");
            if(par_split_space.count() > 3){
                QMessageBox warning;
                warning.setWindowTitle("Warning");
                warning.setText("Incorrect format of Inputs/Outputs!");
                warning.setIcon(QMessageBox::Warning);
                warning.exec();
                info = 1;
                return type_and_name;
            }
            if(par_split_space.count() == 3){
                par_split_space.removeAt(0);
            }
            if(in_or_out != "in"){
                type_and_name << par_split_space.join(" &");
            }else{
                type_and_name << par_split_space.join(" ");
            }
        }
    }
    return type_and_name;
}

