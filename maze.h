#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

class Input
{
    public:
        Input(int argc, char *argv[]);
        int getSeed() const;
        int getRows() const;
        int getColumns() const;

    private:
        int rows;
        int columns;
        int seed;
};

class Cell
{
    public:
        Cell(int x, int y);
        Cell();
        void setVisited(bool value);
        void setValidSolPath(bool value); 
        void setTopWall(bool value);
        void setBottomWall(bool value); 
        void setRightWall(bool value); 
        void setLeftWall(bool value); 
        
        int getRow() const;
        int getColumn() const; 
        bool isVisited() const; 
        bool isValidSolPath() const;
        bool hasTopWall() const;
        bool hasBottomWall() const;
        bool hasRightWall() const;
        bool hasLeftWall() const;

    private:
        int row;
        int column;
        bool visited;
        bool validSolPath;
        bool top, bottom, right, left;
};

class Maze
{
    public:
        Maze(int rows, int columns);

        Cell getCell(int x, int y);

        void hulpGenerator(int currentX, int currentY);

        bool pathFindinghelper(int fromRow, int fromCol, int toRow, int toCol);

        void mazeGenerator();

        void pathFinding();

        void print() const;

    private:
        int numRows;
        int numColumns;
        std::vector<Cell> matrix;
};


#endif
