#include "maze.h"

Input::Input(int argc, char *argv[])
{
    if (argc != 3 && argc != 4)
    {
        throw std::runtime_error("invalid parameter list, I need a row and col size [seed]");
    }

    rows = std::stoi(argv[1]);
    columns = std::stoi(argv[2]);

    if (rows<=0 || columns<=0){
        throw std::runtime_error("invalid parameter");
    }

    if (argc == 4)
    {
        seed = std::stoi(argv[3]);
    }
    else
    {
        seed = time(NULL);
    }
    srand(seed);
}

int Input::getSeed() const
{
    return seed;
}
int Input::getRows() const
{
    return rows;
}
int Input::getColumns() const
{
    return columns;
}

Cell::Cell(int xCor, int yCor)
{
    row = xCor;
    column = yCor;
    visited = false;
    validSolPath = false;
    top = true;
    bottom = true;
    right = true;
    left = true;
}
Cell::Cell()
{
}
void Cell::setVisited(bool value)
{
    visited = value;
}
void Cell::setValidSolPath(bool value)
{
    validSolPath = value;
}
void Cell::setTopWall(bool value)
{
    top = value;
}
void Cell::setBottomWall(bool value)
{
    bottom = value;
}
void Cell::setRightWall(bool value)
{
    right = value;
}
void Cell::setLeftWall(bool value)
{
    left = value;
}

int Cell::getRow() const
{
    return row;
}
int Cell::getColumn() const
{
    return column;
}
bool Cell::isVisited() const
{
    return visited;
}
bool Cell::isValidSolPath() const
{
    return validSolPath;
}
bool Cell::hasTopWall() const
{
    return top;
}
bool Cell::hasBottomWall() const
{
    return bottom;
}
bool Cell::hasRightWall() const
{
    return right;
}
bool Cell::hasLeftWall() const
{
    return left;
}

Maze::Maze(int rows, int columns)
{
    numRows = rows;
    numColumns = columns;

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            matrix.push_back(Cell(i, j));
        }
    }
}

Cell Maze::getCell(int x, int y)
{
    Cell temp;
    for (int i = 0; i < (numRows * numColumns); i++)
    {
        temp = matrix.at(i);
        if ((temp.getRow() == x) && (temp.getColumn() == y))
        {
            return temp;
        }
    }
    return temp;
}

void Maze::hulpGenerator(int currentRow, int currentColumn)
{
    Cell huidige = matrix.at(currentRow * numColumns + currentColumn);
    huidige.setVisited(true);

    matrix.at(currentRow * numColumns + currentColumn) = huidige;

    std::vector<Cell> notVisited;
    Cell temp;

    // top neighbour
    if (currentRow - 1 >= 0 && currentRow - 1 < numRows && currentColumn >= 0 && currentColumn < numColumns)
    {
        if (!matrix.at((currentRow - 1) * numColumns + currentColumn).isVisited())
        {
            notVisited.push_back(matrix.at((currentRow - 1) * numColumns + currentColumn));
        }
    }
    // bottom neighbour
    if (currentRow + 1 >= 0 && currentRow + 1 < numRows && currentColumn >= 0 && currentColumn < numColumns)
    {
        if (!matrix.at((currentRow + 1) * numColumns + currentColumn).isVisited())
        {
            notVisited.push_back(matrix.at((currentRow + 1) * numColumns + currentColumn));
        }
    }
    // right neigbour
    if (currentRow >= 0 && currentRow < numRows && currentColumn + 1 >= 0 && currentColumn + 1 < numColumns)
    {
        if (!matrix.at((currentRow)*numColumns + currentColumn + 1).isVisited())
        {
            notVisited.push_back(matrix.at((currentRow)*numColumns + currentColumn + 1));
        }
    }
    // left neighbour
    if (currentRow >= 0 && currentRow < numRows && currentColumn - 1 >= 0 && currentColumn - 1 < numColumns)
    {
        if (!matrix.at((currentRow)*numColumns + currentColumn - 1).isVisited())
        {
            notVisited.push_back(matrix.at((currentRow)*numColumns + currentColumn - 1));
        }
    }

    if (notVisited.size() == 0)
    {
        return;
    }
    int aantal;

    while((int)notVisited.size() != 0)
    {
        aantal = 0;
        for (int i = 0; i < (int) matrix.size(); i++){
            if(matrix.at(i).isVisited())
                aantal++;
        }
        if(aantal == (int) matrix.size()){
            aantal = 0;
            //print();
            return;
        }

        int nextNeighbor = rand() % notVisited.size();

        int neighbourRow = notVisited.at(nextNeighbor).getRow();
        int neighbourColumn = notVisited.at(nextNeighbor).getColumn();

        if(matrix.at(neighbourRow * numColumns + neighbourColumn).isVisited()){
            std::swap(notVisited.at(nextNeighbor), notVisited.at(notVisited.size() - 1));
            notVisited.pop_back();
            continue;
        }

        if (currentRow - neighbourRow != 0)
        {
            if (currentRow - neighbourRow == 1)
            {
                matrix.at(currentRow * numColumns + currentColumn).setTopWall(false);
                matrix.at(neighbourRow * numColumns + neighbourColumn).setBottomWall(false);
            }
            if (currentRow - neighbourRow == -1)
            {
                matrix.at(currentRow * numColumns + currentColumn).setBottomWall(false);
                matrix.at(neighbourRow * numColumns + neighbourColumn).setTopWall(false);
            }
        }

        if (currentColumn - neighbourColumn != 0)
        {
            if (currentColumn - neighbourColumn == 1)
            {
                matrix.at(currentRow * numColumns + currentColumn).setLeftWall(false);
                matrix.at(neighbourRow * numColumns + neighbourColumn).setRightWall(false);
            }
            if (currentColumn - neighbourColumn == -1)
            {
                matrix.at(currentRow * numColumns + currentColumn).setRightWall(false);
                matrix.at(neighbourRow * numColumns + neighbourColumn).setLeftWall(false);
            }
        }

        std::swap(notVisited.at(nextNeighbor), notVisited.at(notVisited.size() - 1));
        notVisited.pop_back();

        hulpGenerator(neighbourRow, neighbourColumn);
    }
}

void Maze::mazeGenerator()
{
    int startRow = rand() % numRows;
    int startColumn = rand() % numColumns;

    hulpGenerator(startRow, startColumn);

    for (int i = 0; i < (int) matrix.size(); i++)
    {
        matrix.at(i).setVisited(false);
    }
}

void Maze::pathFinding()
{
    bool succes = pathFindinghelper(0, 0, numRows - 1, numColumns - 1);

    if (!succes)
    {
         print();
         throw std::runtime_error("No path possible");
    }
}

bool Maze::pathFindinghelper(int fromRow, int fromCol, int toRow, int toCol)
{
    matrix.at(fromRow * numColumns + fromCol).setVisited(true);
    matrix.at(fromRow * numColumns + fromCol).setValidSolPath(true);

    if (fromRow == toRow && fromCol == toCol)
    {
        return true;
    }
    std::vector<Cell> neighors;

    // top neighbour
    if (fromRow - 1 >= 0 && fromRow - 1 < numRows && fromCol >= 0 && fromCol < numColumns)
    {
        if (!matrix.at((fromRow - 1) * numColumns + fromCol).isVisited())
        {
            if (!matrix.at((fromRow) * numColumns + fromCol).hasTopWall())
            {
            neighors.push_back(matrix.at((fromRow - 1) * numColumns + fromCol));
            }
        }
    }
    // bottom neighbour
    if (fromRow + 1 >= 0 && fromRow + 1 < numRows && fromCol >= 0 && fromCol < numColumns)
    {
        if (!matrix.at((fromRow + 1) * numColumns + fromCol).isVisited())
        {
            if (!matrix.at((fromRow) * numColumns + fromCol).hasBottomWall())
            {
            neighors.push_back(matrix.at((fromRow + 1) * numColumns + fromCol));
            }
        }
    }
    // right neigbour
    if (fromRow >= 0 && fromRow < numRows && fromCol + 1 >= 0 && fromCol + 1 < numColumns)
    {
        if (!matrix.at((fromRow)*numColumns + fromCol + 1).isVisited())
        {
            if (!matrix.at((fromRow) * numColumns + fromCol).hasRightWall())
            {
            neighors.push_back(matrix.at((fromRow)*numColumns + fromCol + 1));
            }
        }
    }
    // left neighbour
    if (fromRow >= 0 && fromRow < numRows && fromCol - 1 >= 0 && fromCol - 1 < numColumns)
    {
        if (!matrix.at((fromRow)*numColumns + fromCol - 1).isVisited())
        {
            if (!matrix.at((fromRow) * numColumns + fromCol).hasLeftWall())
            {
            neighors.push_back(matrix.at((fromRow)*numColumns + fromCol - 1));
            }
        }
    }

    int newFromRow, newFromCol;

    for (int i = 0; i < (int) neighors.size(); i++)
    {
        newFromRow = neighors.at(i).getRow();
        newFromCol = neighors.at(i).getColumn();
        if(matrix.at(newFromRow * numColumns + newFromCol).isVisited())
        {
                continue;
        }

        if (pathFindinghelper(newFromRow, newFromCol, toRow, toCol))
        {
            return true;
        }
    }
    matrix.at(fromRow * numColumns + fromCol).setVisited(false);
    matrix.at(fromRow * numColumns + fromCol).setValidSolPath(false);


    return false;
}

void Maze::print() const
{
    for (int row = 0; row < numRows; row++)
    {
        for (int col = 0; col < numColumns; col++)
        {
            if (matrix.at(row * numColumns + col).hasTopWall())
            {
                std::cout << "+---";
            }
            else
            {
                std::cout << "+   ";
            }
        }
        std::cout << "+" << std::endl;
        std::cout << "|";
        for (int col = 0; col < numColumns; col++)
        {
            /*
            if (matrix.at(row * numColumns + col).hasLeftWall())
            {
                std::cout << "|";
            }
            else
            {
                std::cout << " ";
            }
            */
            if (matrix.at(row * numColumns + col).isValidSolPath())
            {
                std::cout << " . ";
            }
            else
            {
                std::cout << "   ";
            }

            if (matrix.at(row * numColumns + col).hasRightWall())
            {
                std::cout << "|";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    for (int col = 0; col < numColumns; col++)
    {
        if (matrix.at((numRows - 1) * numColumns + col).hasBottomWall())
        {
            std::cout << "+---";
        }
        else
        {
            std::cout << "+   ";
        }
    }
    std::cout << "+" << std::endl;
}

int main(int argc, char *argv[])
{
    try
    {
        Input input = Input(argc, argv);

        Maze maze = Maze(input.getRows(), input.getColumns());
        maze.mazeGenerator();
        maze.pathFinding();
        maze.print();

    }
    catch (std::runtime_error &excpt)
    {
        std::cerr << excpt.what() << std::endl;
    }

    return 0;
}
