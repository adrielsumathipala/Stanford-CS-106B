/*
 * Assignment 1: Game of Life
 * Stanford CPSC 106B
 * By: Adriel Sumathipala
*/

//Importing libraries
//Libraries in <> brackets are standard libraries
//Libraries in "" quotes are Stanford C++ Libraries
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
#include "lifegui.h" //GUI for game
using namespace std;

//Function Prototypes:
void printGrid(Grid<char>);
void iterateNeighbor(int &neighbor, char gridPoint);
char addCell(int neighbors, char currentValue);
int wrapNumNeighbors(Grid<char> &grid, int r, int c);
int noWrapNumNeighbors(Grid<char> &grid, int r, int c);
void wGeneration(Grid<char> &grid, Grid<char> &gridNew);
void nwGeneration(Grid<char> &grid, Grid<char> &gridNew);
int advance(Grid<char> grid, bool wrapBool);

int main() {
    //PART 1: Read in data from input grid file:
    string row;
    string column;
    string fileName;
    ifstream stream;

    //Check if file exists:
    fileName = getLine("Grid input file name? ");
    while(!openFile(stream, fileName)){
        cout << "Unable to open that file. Try again." << endl;
        fileName = getLine("Grid input file name? ");
    }

    //Reading in number of row/columns:
    getLine(stream, row);
    getLine(stream, column);
    int numRow = stringToInteger(row);
    int numCol = stringToInteger(column);

    //Make and Fill in Game Board:
    Grid<char> grid(numRow, numCol);
    for(int i = 0; i < numRow; i++){
        string line;
        getLine(stream, line);
        for(size_t j = 0; j < numCol; j++){
            grid[i][j] = line[j];
        }
    }

    //Close stream:
    stream.close();

    //Asking user if board should be wrapped:
    string wrapString = getLine("Should the simulation wrap around the grid (y/n)? ");
    bool wrapBool = wrapString == "y";

    //Printing initial state of game board:
    printGrid(grid);

    //Quits game or advances the game by single generation or by animation.
    int exit = advance(grid, wrapBool);
    cout << "Have a nice Life!" << endl;
    return exit;
}

//Function to print grid:
void printGrid(Grid<char> grid){
    string line;
    for(int i = 0; i < grid.numRows(); i++){
        for(int j = 0; j < grid.numCols(); j++){
            cout << grid[i][j];
        }
       cout << endl;
    }
}

//Function: Advances game by one generation
// in the case WITHOUT WRAPPING
void nwGeneration(Grid<char> &grid, Grid<char> &gridNew){
    int neighbors;
    for(int r = 0; r < grid.numRows(); r++){
        for(int c = 0; c < grid.numCols(); c++){
            neighbors = noWrapNumNeighbors(grid, r, c);
            gridNew[r][c] = addCell(neighbors, grid[r][c]);
        }
    }
}

//Function:Count number of neighbors at grid point (r,c)
//WITHOUT wrapping
int noWrapNumNeighbors(Grid<char> &grid, int r, int c){
    //Variables to keep track of neighbors
    int neighbors = 0;
    int rowNeighbor, colNeighbor;

    //Count number of neighbors surrounding cell
    for(int rowDiff = -1; rowDiff < 2; rowDiff++){
        for(int colDiff = -1; colDiff < 2; colDiff++ ){
            //Find grid point of neighbor:
            rowNeighbor = r + rowDiff;
            colNeighbor = c + colDiff;

            //Don't count cell itself as a neighbor
            if(rowDiff == 0 && colDiff == 0){
                continue;
            }
            //If gridpoint is inbounds:
            else if(grid.inBounds(rowNeighbor, colNeighbor)){
                iterateNeighbor(neighbors, grid[rowNeighbor][colNeighbor]);
            }
        }
    }
    return neighbors;
}

//Function: Advances game by one generation
// in the case WITH WRAPPING
void wGeneration(Grid<char> &grid, Grid<char> &gridNew){
    //Variable to keep track of number of neighbors
    int neighbors;
    for(int r = 0; r < grid.numRows(); r++){
        for(int c = 0; c < grid.numCols(); c++){
            neighbors = wrapNumNeighbors(grid, r, c);
            gridNew[r][c] = addCell(neighbors, grid[r][c]);
        }
    }
}

//Function:Count number of neighbors at grid point (r,c)
//with wrapping
int wrapNumNeighbors(Grid<char> &grid, int r, int c){
    //Variables to keep track of neighbors
    int neighbors = 0;
    int rowNeighbor, colNeighbor;

    //Variables for handling wrapping case:
    int wrapRow, wrapCol;

    //Count number of neighbors surrounding cell
    for(int rowDiff = -1; rowDiff < 2; rowDiff++){
        for(int colDiff = -1; colDiff < 2; colDiff++ ){
            //Find grid point of neighbor:
            rowNeighbor = r + rowDiff;
            colNeighbor = c + colDiff;

            //Don't count cell itself as a neighbor
            if(rowDiff == 0 && colDiff == 0){
                continue;
            }
            //If gridpoint is inbounds:
            else if(grid.inBounds(rowNeighbor, colNeighbor)){
                iterateNeighbor(neighbors, grid[rowNeighbor][colNeighbor]);
            }
            //If gridpoint is out of bounds:
            else if(!grid.inBounds(rowNeighbor, colNeighbor)){
                //Increase rowNeighbor by 1 because indexing from 0
                wrapRow = (rowNeighbor + grid.numRows()) % grid.numRows();
                wrapCol = (colNeighbor + grid.numCols()) % grid.numCols();
                iterateNeighbor(neighbors, grid[wrapRow][wrapCol]);
            }
        }
    }
    return neighbors;
}

//Function: Increases neighbor counter by one if
//gridpoint = "X"
void iterateNeighbor(int &neighbor, char gridPoint){
    if(gridPoint == 'X'){
        neighbor++;
    }
}

//Function: Adds a cell to gridpoint if conditions met:
//char addCell(int neighbors, char currentValue){
//    if(neighbors < 2){
//        return '1';
//    } else if(neighbors < 3){
//        return '2';
//    } else if(neighbors < 4){
//        return '3';
//    } else {
//        return '4';
//    }
//}

char addCell(int neighbors, char currentValue){
    if(neighbors < 2){
        return '-';
    }
    else if(neighbors < 3){
        return currentValue;
    }
    else if(neighbors < 4){
        return 'X';
    }
    else {
        return '-';
    }
}

int advance(Grid<char> grid, bool wrapBool) {
    Grid<char> gridNew  = grid;
    string option = getLine("a)nimate, t)ick, q)uit? " );
    string optionLower= toLowerCase(option);
    if(optionLower== "a"){
        string numGenString = getLine("How many frames? ");
        int numGen = stringToInteger(numGenString);
        if(wrapBool){
            for(int gen = 0; gen < numGen; gen++){
                wGeneration(grid, gridNew);
                printGrid(gridNew);
                grid = gridNew;
                pause(50);
                clearConsole();
            }
            advance(grid, wrapBool);
        } else{
            for(int gen = 0; gen < numGen; gen++){
                nwGeneration(grid, gridNew);
                printGrid(gridNew);
                grid = gridNew;
                pause(50);
                clearConsole();
            }
            advance(grid, wrapBool);
        }

    } else if(optionLower == "t"){
        if(wrapBool){
            wGeneration(grid, gridNew);
            printGrid(gridNew);
            grid  = gridNew;
            advance(grid, wrapBool);
        } else {
            nwGeneration(grid, gridNew);
            printGrid(gridNew);
            grid  = gridNew;
            advance(grid, wrapBool);
        }
    } else if(optionLower == "q"){
        return 0;
    } else{
        cout << "Please enter a, q, or t" << endl;
        advance(grid, wrapBool);
    }
    return 0;
}
