Editor and interpret of structured functional blocks (ICP)

This app is a project (variant 2) for subject ICP at VUT FIT Brno.

Authors:
Stanislav Gabriš 		<xgabri18(at)fit.vutbr.cz>
Adam Fabo (project leader)  	<xfaboa00(at)fit.vutbr.cz>


------- GENERAL --------
Move provided library to folder with executable, as that is the folder that will be searched!

------- Description: ---------

Editor:

Editor allows the user to create and edit atomic blocks as specified by the assignment.
-User can choose name and category of the atomic block as well as add new category.
-User can specify Inputs/Outputs in format: [type] [variable name],[pin name] where pin name is not mandatory (syntax here is controlled).
-User can specify logic of the block, where it is also possible to use Inputs/Outputs specified before (or after) (syntax is not controlled).
-User can save a block and add a Description.
-Editing is done through button OPEN (which opens the library of atomic blocks), where user chooses a block by double-clicking on it. Double-clicking here on category allows user to edit category name.
-In the OPEN window it is also possible to add categories or remove blocks/categories (removing a category removes all blocks in the category).

Summary of implemented features: creating/editing atomic blocks, creating/removing/renaming categories, library 
Not implemented: sub-categories, description is not visible anywhere 


----------------------------------------------------------------------------------

Builder:
    Here user can create new, save or open scene and  create new, save or open composite block

    For adding new block press [B] or click at Block button and library window will open
        Here you can select part, confirdm selection by pressing at OK
        Now you are in block inserting mode, whenever you press Left mouse button new block will appear. While you keep
        Left mouse button pressed you can move with block. After Left mouse button is released block is added to scene.

        Note: even after adding one block you are still in block inserting mode, so you can insert multiple objects.
        For exiting block inserting mode press ESC or [B] or Blcok button.
        (functionality should be simmiliar to Eagle[scheme drawing program])

    For adding new line press [A] or click at Line button.
        Now you are in line adding mode. You can just press Left mouse button and draw lines
        For exiting line insert mode press ESC or [A] or Line button.

        Note: lines are connected only when line starts at point where andother line ends. Lines are not connected
              when line starts at middle of another line.

    For deleting part in scene press [D] or click at Delete button.
        Now you are in line deleting mode .After clicking at object in the scene, object should disappear and is
        removed from a scene.
        For exiting line deleting mode press ESC or [D] or Delete button.

    Zooming in and out in scene
        You can zoom in and out naturaly with scroll wheel.

    Navigating at the scene.
        You can move at the scene by pressing middle mouse button and moving with the mouse.
        Scene is saved with suffix .scn

    For saving scene you can press Ctrl+S or click at Save button.
        Note - scene cant have special in and out block to be clasified as a scene. In and Out blocks are purely for
               composite blocks

    For opening already saved scene press [O] ore click at Open button.

    For creating new scene press [N] or click at New button

    For compiling scene press [C] or compile button. You will be asked to save scene, and final compiled scene will be
        saved in directory which was selected when saving scene

    For showing help window press [H] or Help button and this text will be shown

    *******************************
    Composite blocks

    Composite blocks is in its nature just a scene with Input/Output blocks

    For creating new composite block draw scene as usual and after that add some desired Input/Output blocks

    When adding Input/Output block you will be asked to write port name.
        Port name should be formatted as [data type] [variable name], [pin name] e.g. "int a,IN_01".
        Make sure that variable name and pin name are unique among other Input/Output blocks.

    For saving composite block click at File->Save composite and you will be asked to insert name and description and
        to select category. By clicking at [+] button you can add new category. When adding new category/composite block
        make sure that your name is unique among other categories and names - including atomic.

        After saving you can add composite block to the scene by clicking at Add part and selecting created block

    For editing created composite block press [O] or click at Open button and select from library composite block.



----------------------------------------------------------------------------------

Compilation:

Compilation happens when (in Builder) user presses compile or when save composite is pressed.
-When compiling a scene (not saving a composite) a file is created beggining with "compiled_" and the scene name where the code is generated with a main function, this code is ready to be built and ran (note that generated code does not contain any #include).
-When saving a composite block, the compilation also takes place but the generated code is stored alongside other informations about the composite and therefore no main function is created.

Notes: It is important that, when compiling and using composite blocks, all blocks the composite blocks are dependent on exist in library !


Compilation Steps:
     * ---------------------------------------------------
     * 1.Map all IN/CONST blocks
     * 2.Process all blocks that can be processed(have all inputs defined and have > 0 inputs)
     * 3.Process all blocks with 0 inputs
     * 4.Process rest of blocks (until all blocks are in fully_processed_blocks)
     * ----------------------------------------------------
     * 1./2.
     * -compiler starts at some IN or CONST block
     * -it follows output line going from block and into other blocks and maps these connections
     * -when all IN/CONST blocks are done it continues onto regular blocks
     * -it tries to map outputs of defined blocks again
     * -this is done in loop until no connections happen
     *
     * 3.
     * -compiler now easily processes blocks with no inputs
     * -does so by defining such a block and doing step 2.
     *
     * 4.
     * -compiler starts at some ("random") undefined block 
     * -picks first undefined input and calls go_upstream() on it (tries to find dependencies)
     * -method go_upstream() finds a block connected to input of previous block 
     * -(if no block is found this input is force defined and step 2. is done again)
     * -if block is found we pick this block by calling process_undefined_block() and continue with go_upstream()
     * -this is repeated until no previous block is found(so current block is processed)
     * -or if compiler finds a loop (we visit the same block more than once)
     *   -> in this case the repeating block is processed
     * -whole step is repeated until there are no unprocessed blocks

From this:
-Blocks CONST/IN have the highest priority, then defined blocks, then blocks with 0 inputs, then undefined blocks.
-Defined block -> block with all inputs defined, Processed block -> block with all outputs processed
-When more than 1 output is connected to an input, only one will be chosen ("randomly")
-"randomness" is dependant on type of block and order of placement
-line is defined by 2 points, to join lines, they must connect at these points

Not implemented: Loops are not processed as loops, instead the scene is taken as a snapshot -> everything is checked only once by the compiler
Not implemented: No validation is done on IN/OUT blocks, or if dependencies of composite block exist
