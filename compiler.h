/*
 * project: Editor and interpret of structured functional blocks (2)
 * subject: ICP at VUT FIT Brno
 * file: compiler.h
 * date: 6.5.2021
 *
 * authors: Stanislav Gabriš <xgabri18(at)fit.vutbr.cz>
 *          Adam Fabo        <xfaboa00(at)fit.vutbr.cz>
 *
 * description: This file contains Compiler class, file was generated through Qt but filled by hand.
 *
 */


#ifndef COMPILER_H
#define COMPILER_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QStack>
#include "blockinscene.h"
#include "appmodel.h"
/**
 * @brief The Compiler class represents a compiler that is able to, just by correct initialization and calling of method compile(), analyze the scene and create code out of it
 */
class Compiler : public QObject
{
    Q_OBJECT

public:
#define BLOCKWIDTH 80 //+-20 to in/out pins
#define TOPIN 20
#define CONSTBLOCKSWIDTH 140 //+20 to pin when in,const; 0 when out

private:
    bool    compiling_for_scene; //will it be in main or no
    QString where_to_write; //filepath
    int     counter; //keeps track of pin amount


    AppModel *tmpmodel;
    QList<QString>          already_defined_functions; //during initialization functions are defined

    typedef struct block_info{
        QStringList cat_name_desc_code;
        QStringList inputs;
        QStringList outputs;
    }BlockInfoStruct;
    QVector<BlockInfoStruct*> info_about_blocks; //so we dont need to open the same file

    //  /////////////////
    QVector <QGraphicsItem *> blockList;  //blocks, in, out, konstanty gained from outside
    QVector <QString>         blockListStrings;

    QVector <QGraphicsLineItem *> lineList; // holds lines linelist[0].line.p1 ->beggining

    /*
     * Line travesring:
     * ---------------------------------------------------
     * 1.Map all IN/CONST blocks
     * 2.Process all blocks that can be processed(have all inputs defined and have > 0 inputs)
     * 3.Process all blocks with 0 inputs
     * 4.Process rest of blocks (until all blocks are in fully_processed_blocks)
     * ---------
     * Not yet implemented:
     * 5.Check for loops and process them (creates while(1) in code)
     * ----------------------------------------------------
     * 1./2.
     * -compiler starts at some input or constant
     * -it follows line going from block and into other blocks and maps these connections
     * -when al IN/CONST blocks are done it continues onto regular blocks
     * -it tries to map outputs of defined blocks again
     * -this is done in loop until no connections happen
     *
     * 3.
     * -compiler now easily processes blocks with no inputs
     * -does so by defining such a block and doing step 2.
     *
     * 4.
     * -compiler starts at some undefined block
     * -picks first undefined input and call go_upstream() on it
     * -method go_upstream() finds a block connected to input we started on
     * -(if no block is found this input is processed and we try step 2. again)
     * -if block is found we pick this block by calling process_undefined_block() and continue with go_upstream()
     * -this is repeated until no previous block is found(so current block is processed)
     * -or if compiler finds a loop (we visit the same block more than once)
     *   -> in this case the repeating block is processed
     *
     * ---------------------------------------------------
     * Notes:
     * -go_upstream() is essentialy similar to process_line() as both are trying to search for connections, but the former is used to process undefined inputs while the latter is used to map outputs to inputs
     * -step 3. is done to guarantee that IN/CONST have priority
     * -if two outputs go into 1 input, only one connection will be mapped while the other will be discarded
     * -order in which blocks and lines are placed matters considering previous point(defines order in which objects are processed)
     *
     */

    QList   <QPointF>    fully_processed_points; //point is fully processed if all lines from it are checked
    QList   <QPointF>    visited_points;  //when a point is visited in process_line(), so there is not an infinite loop


    QList   <BlockInScene *>  all_blocks;
    QList   <int>             fully_processed_blocks; //when all outputs of block are processed (different from block.am_i_defined)
    //QList   <int>             functions_written_for;

    QString                   code; //code that is being generated
    QString                   init_code; //initializations of variables, will connect to code
    QString                   init_functions_code; //which blocks have function definined in code


    bool                      new_input_defined;
    bool                      force_defined; //something was force defined
    //bool                      was_recursion;
    QList   <int>             upstream_list; //when going upstream store which blocks were vidited
    //some list with loop participants or somthing like that


public:
    explicit Compiler(QObject *parent = nullptr, QVector<QGraphicsItem *> new_blockList = {}, QVector<QString> new_blockListStrings = {},QVector<QGraphicsLineItem *> new_lineList ={} , bool scene=false, QString filepath="");
    ~Compiler();

    /**
     * @brief This method should return the code that was generated after callinng compile()
     * @return Generated code
     */
    QString generated_code() {return code;};
    /**
     * @brief Returns number of all blocks in scene
     * @return Number of blocks
     */
    int     block_count() {return blockListStrings.count();};

    /**
     * @brief Main compiler method that uses Compiler class members to generate code
     */
    void    compile();
    /**
     * @brief This method is responsible for, call other preparation methods, prepare all the necessary data and mainly initialize BlockInScene objects, for further compilation
     */
    void    prepare_to_compile();
    /**
     * @brief Method that begins the data collection from files about each Block, used befor compilation
     */
    void    prepare_data_from_files();
    /**
     * @brief This method chceks wheter the loaded block needs other blocks to functions and can cause recursion through model->open_block_by_name
     * @param using_blocks
     */
    void    check_composite_dependencies(QString using_blocks);
    /**
     * @brief In the generated code creates initialization of variables
     * @param block Specific block whose variables will be initialized
     * @param is_const Whether block is CONST
     */
    void    generate_init_of_vars(int block, bool is_const);
    /**
     * @brief Processes all connections from CONST/IN blocks
     */
    void    process_const_blocks();
    /**
     * @brief Processes other blocks dependant only on CONST/IN
     */
    void    process_other_blocks();
    /**
     * @brief Processes rest of scene
     */
    void    process_rest_of_scene();

    /**
     * @brief Processes all outputs of block using process_line()
     * @param block Which block to process
     */
    void    process_outputs(BlockInScene *block);
    /**
     * @brief This method is called to map an output to inputs -> processes path from output on output_pos -> does so recursively
     * @param block To which block does the output belong
     * @param output_pos Position of output to be mapped
     * @param point Point along the path from output
     */
    void    process_line(BlockInScene *block, QPointF output_pos, QPointF point);
    /**
     * @brief Tries to define a block
     * @param block At which block does the path begin
     */
    void    process_undefined_block(BlockInScene *block);

    /**
     * @brief Find dependencies -> try to define next block -> searches for an output of another block -> does so recursively
     * @param block To which block does the input belong
     * @param input_pos Position of input that is not defined
     * @param point Point along the path
     */
    void    go_upstream(BlockInScene *block, QPointF input_pos, QPointF point);
    /**
     * @brief This method is used when the program decides which block needs to be processed first in 4. step of program and defines one input
     * @param block Which block
     * @param input_pos Input to be defined
     */
    void    force_define_block(BlockInScene *block, QPointF input_pos);



    /**
     * @brief Generate a line of code like -> in = out;
     * @param in_block
     * @param out_block
     * @param input
     * @param output
     */
    void    generate_code_for_connection(BlockInScene *in_block, BlockInScene *out_block, QPointF input, QPointF output);
    /**
     * @brief Create a function call -> ADD(var1,var2);
     * @param call
     */
    void    generate_code_call_block(QString call);
    /**
     * @brief Adds function head to code (either void main() or if composite creates more sophisticated head)
     */
    void    generate_head();
    /**
     * @brief Adds & for out_variable
     * @param out_variable
     * @return
     */
    QString adjust_out(QString out_variable);

    /**
     * @brief When compiling scene, creates new file with just the compiled code
     */
    void    save_compiled_scene();


public slots:
    /**
     * @brief Slot that is invoked when data from file are sent by model
     * @param cat_name_desc_code In order -> category,name,description,code
     * @param inputs Inputs of block (int a,IN,...)
     * @param outputs Outputs of block (int o,OUT,...)
     */
    void file_read(QStringList cat_name_desc_code, QStringList inputs, QStringList outputs);

signals:

};

#endif // COMPILER_H
