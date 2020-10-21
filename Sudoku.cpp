#include "Sudoku.h"

Sudoku::Sudoku()
{
}

Sudoku& Sudoku::getInstance()
{
    static Sudoku sudoku;
    return sudoku;
}

// run
int Sudoku::run(const int argc, char **argv) const
{
    // analize arguments
    Arguments arguments;
    try {
        arguments.analyzeArguments(argc, argv);
    } catch (std::runtime_error& error) {
        std::cerr << "error: Arguments: " << error.what() << std::endl;
        return 0;
    }


    Convert convert(arguments);

    // if help was asked then
    if(arguments.getHelpAsked())
    {
        // return help message
        return arguments.printHelp();
    }

    // if version was asked then
    if(arguments.getVersionAsked())
    {
        // return program version
        return arguments.printVersion();
    }

    // if only convert then
    if(arguments.getOnlyConvert())
    {
        // convert
        try {
            convert.convertTestCase();
        } catch (std::runtime_error& error) {
            std::cerr << "error: Could not convert to " << error.what() << std::endl;
            return 0;
        }

    }
    // else if not only convert then
    else
    {
        SudokuLevel sudoku;
        try {
            convert.loadSudokuLevel(sudoku);
        } catch (std::runtime_error& error) {
            std::cerr << "error: Could not load Sudoku: " << error.what() << std::endl;
            if(sudoku.getDimension() == 0)
                convert.reportUnsolvable();
            return 0;
        }

        bool isSudokuLegal = sudoku.verify();
        if (!isSudokuLegal)
        {
            std::cout << "Sudoku is not legal" << std::endl;
            return 0;
        }

        // solve
        try {
            bool canItBeSolved = sudoku.solve(true);
            // write answer
            if (canItBeSolved)
            {
                convert.printSudoku(sudoku);
            }
            else
            {
                convert.reportUnsolvable();
            }
        } catch (std::runtime_error& error) {
            std::cerr << "error: sudoku.solve: " << error.what() << std::endl;
            return 0;
        }
    }
    return 0;
}
