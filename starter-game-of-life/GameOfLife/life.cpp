// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include "random.h"
using namespace std;

/**
 * The populateGrid function reads lines from the
 * already-open ifstream and populates the already-created
 * gameboard.
 */
void populateGrid(Grid<char> &gameboard, ifstream &ifs) {
    // TODO: Your code here
    string row_counts;
    string column_counts;
    getline(ifs, row_counts);
    getline(ifs, column_counts);
    int row_count = stringToInteger(row_counts);
    int column_count=stringToInteger(column_counts);
    for(int i = 0; i < row_count; i++){
        for(int j = 0; j < column_count; j++){
            int neigh_count=0;
            for(int k = -1; k <= 1; k++){
                for(int m = -1; m <= 1; m++){
                    if(gameboard.inBounds(i+k, j+m) && gameboard[i+k][j+m]=='X'){
                        neigh_count++;
                    }
                }
            }
            if(gameboard[i][j]=='X'){
                neigh_count--;
            }
            if(neigh_count<=1 || neigh_count>3){
                gameboard[i][j]='-';
            }
            else if(neigh_count==3){
                gameboard[i][j]='X';
            }
        }
    }

}

/**
 * The loadBoard function takes an already-open
 * ifstream and reads in the width and height of
 * the gameboard. Then it creates and populates
 * the gameboard and closes the ifsteram.
 *
 * Return value: A Grid<char> representing the
 * Game of Life starting game board
 */
Grid<char> loadBoard(ifstream &ifs) {
    // TODO: Your code here (you will change the return value)
    string row_counts;
    string column_counts;
    getline(ifs, row_counts);
    getline(ifs, column_counts);
    int row_count = stringToInteger(row_counts);
    int column_count=stringToInteger(column_counts);
    char ch;
    Grid<char> board(row_count, column_count);
    for(int i = 0; i < row_count; i++){
        for(int j = 0; j< column_count + 1; j++){
            ifs.get(ch);
            if(ch=='X'){
                board[i][j]=ch;
            }
            else if(ch=='-'){
                board[i][j]=ch;
            }
        }
    }
    ifs.close();
    return board;
}

/**
 * The printBoard function prints the board to the screen
 */
void printBoard(Grid<char> gameboard) {
    // TODO: Your code here
    for(int i = 0; i < gameboard.numRows(); i++){
        for(int j = 0; j < gameboard.numCols(); j++){
            cout<<gameboard[i][j];
        }
        cout<<endl;
    }
}

/**
 * countNeighbors counts the neighbors of a grid squre.
 * '-' is an unpopulated square, and 'X' is a populated square.
 * If wrap is true, then squares that are on edges potentially
 * contribute to the overall count.
 *
 * Returns the neighbor count
 */
int countNeighbors(Grid<char> &gameboard, int row, int col, bool wrap) {
    // TODO: Your code here (change the return value)
    int neigh_count=0;
    if (!wrap){
        for(int k = -1; k <= 1; k++){
            for(int m = -1; m <= 1; m++){
                if(gameboard.inBounds(row+k, col+m) && gameboard[row+k][col+m]=='X'){
                    neigh_count++;
                }
            }
        }
        if(gameboard[row][col]=='X'){
            neigh_count--;
        }
    }
    else {
        for(int k = -1; k <= 1; k++){
            for(int m = -1; m <= 1; m++){
                if(gameboard[(row+k+gameboard.numRows())%gameboard.numRows()][(col+m+gameboard.numCols())%gameboard.numCols()] == 'X'){
                    neigh_count++;
                }
            }
        }
        if(gameboard[row][col]=='X'){
            neigh_count--;
        }
    }
    return neigh_count;
}

/**
 * The tick function runs one iteration of the game of life
 * on gameboard, using wrap to determine if the board should be
 * wrapped or not
 *
 */
void tick(Grid<char>& gameboard, bool wrap) {
    int neigh_count = 0;
    Grid<char> copy = gameboard;
    for(int i = 0; i< gameboard.numRows(); i++){

        for(int j = 0; j < gameboard.numCols(); j++){
            neigh_count=countNeighbors(gameboard, i, j, wrap);
            if(neigh_count<=1 || neigh_count>3){
                copy[i][j]='-';
            }
            else if(neigh_count==2){
                copy[i][j]=gameboard[i][j];
            }
            else if(neigh_count==3){
                copy[i][j]='X';
            }
        }
    }
    gameboard=copy;
    printBoard(copy);
}

// You should have more functions here

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    ifstream input;
    string filename=getLine("Enter file name ");
    input.open(filename);
    while(input.fail()){
        cout<<"Unable to open that file.  Try again."<<endl;
        filename=getLine("Enter file name ");
        input.open(filename);
    }
    Grid<char> gameboard=loadBoard(input);
    bool wrap = getYesOrNo("Should the simulation wrap around the grid (y/n)? n");
    string choice = getLine("a)nimate, t)ick, q)uit?");
    toLowerCase(choice);
    while(choice!="q"){
        if(choice=="t"){
            tick(gameboard, wrap);
            choice = getLine("a)nimate, t)ick, q)uit?");
            toLowerCase(choice);
        }
        else if(choice=="a"){
            int frames=getInteger("How many frames?");
            for(int i = 0; i < frames; i++){
                clearConsole();
                tick(gameboard,wrap);
                pause(50);
            }
            choice = getLine("a)nimate, t)ick, q)uit?");
            toLowerCase(choice);
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

// used for creating tests
void saveGame(Grid<char> gameboard, string filename) {
    ofstream ofs;
    ofs.open(filename, std::fstream::out);
    ofs << gameboard.numRows() << endl;
    ofs << gameboard.numCols() << endl;
    for (int row = 0; row < gameboard.numRows(); row++) {
        for (int col = 0; col < gameboard.numCols(); col++) {
            ofs << gameboard[row][col];
        }
        ofs << endl;
    }
    ofs.close();
}

PROVIDED_TEST("Test loadBoard function on simple.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', 'X', 'X', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/simple.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test loadBoard function on format.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', 'X', 'X', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/format.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test loadBoard function on stableplateau.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X'},
        {'-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-'},
        {'-', '-', '-', '-', 'X', '-', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', 'X', '-', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/stableplateau.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test simple.txt for one iteration") {
    ifstream ifs;
    openFile(ifs, "boards/simple.txt");
    Grid<char> gameboard = loadBoard(ifs);

    ifstream ifs2;
    openFile(ifs2, "boards-after-one-iteration/simple-after1.txt");
    Grid<char> gameboardAfterOne = loadBoard(ifs2);

    tick(gameboard, false);
    EXPECT_EQUAL(gameboard, gameboardAfterOne);
}

PROVIDED_TEST("Test all boards for one iteration, no wrapping") {
    Vector<string> files = {
        "diehard", "dinner-table", "fish", "flower",
        "format", "glider-explosion", "glider-gun",
        "glider", "quilt-square", "rpent", "seeds",
        "simple", "simple-edge", "snowflake", "spiral",
        "stableplateau", "tictactoe"
    };

    for (string &filename : files) {
        string boardFilename = "boards/" + filename + ".txt";
        string boardAfter1Filename =
                "boards-after-one-iteration/" +  filename + "-after1.txt";
        cout << boardFilename << endl;
        cout << boardAfter1Filename << endl;
        ifstream ifs;
        openFile(ifs, boardFilename);
        Grid<char> gameboard = loadBoard(ifs);
        tick(gameboard, false);

        ifstream ifs2;
        openFile(ifs2, boardAfter1Filename);
        Grid<char> gameboardAfter1 = loadBoard(ifs2);

        EXPECT_EQUAL(gameboard, gameboardAfter1);
//        saveGame(gameboard, boardAfter1Filename);
    }
}

PROVIDED_TEST("Test simple-edge for one iteration, with wrapping") {
    ifstream ifs;
    openFile(ifs, "boards/simple-edge.txt");
    Grid<char> gameboard = loadBoard(ifs);

    ifstream ifs2;
    openFile(ifs2, "boards-after-one-iteration/simple-edge-after1-wrap.txt");
    Grid<char> gameboardAfterOne = loadBoard(ifs2);

    tick(gameboard, true);
    EXPECT_EQUAL(gameboard, gameboardAfterOne);
}
