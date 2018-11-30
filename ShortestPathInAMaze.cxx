#include <iostream>
#include <vector>
#include <climits>
#include <stack>


using namespace std;

class Cell {
public:
    const int row;
    const int col;
    char data;
    bool isVisited;
    int distance;
    Cell* previous;

    Cell(const int row_, const int col_)
     : row(row_),
       col(col_),
       data('_'),
       isVisited(false),
       distance(INT_MAX),
       previous(NULL){}

     bool isBomb() {
       return data == '*';
     }

     void setBomb() {
       data = '*';
     }
};

class Maze {
public:
  const int m;
  const int n;
  vector<vector<Cell> > maze;

  Maze(const int m_, const int n_)
   : m(m_),
     n(n_) {
    maze.resize(m);

    for(int rowIndex = 0; rowIndex < m; ++rowIndex) {
      vector<Cell>& eachRow = maze[rowIndex];
      for (int colIndex = 0; colIndex < n; ++colIndex) {
        eachRow.push_back(Cell(rowIndex, colIndex));
      }
    }
  }

  Cell& getCell(const int row, const int col) {
    return maze[row][col];
  }

  void setBomb(const int row, const int col) {
    const bool isInputValid = row >=0 && row < m && col >=0 && col < n;

    if (!isInputValid) return;
    maze[row][col].setBomb();
  }

  vector<Cell*> GetNeighbours(Cell& source) {
      const int row = source.row;
      const int col = source.col;
      vector<Cell*> neighbours;

      if ( (col - 1) >= 0) {
          neighbours.push_back(&maze[row][col-1]);
      }
      if ( (col + 1) < n) {
          neighbours.push_back(&maze[row][col+1]);
      }
      if ( (row - 1) >= 0) {
          neighbours.push_back(&maze[row - 1][col]);
      }
      if ( (row + 1) < m) {
          neighbours.push_back(&maze[row + 1][col]);
      }

      return neighbours;
  }

  Cell* GetMinDistanceCell() {
    Cell* minDistanceCell = NULL;

    for(int rowIndex = 0; rowIndex < m; rowIndex++) {
      for (int colIndex = 0; colIndex < n; colIndex++) {
        if (maze[rowIndex][colIndex].isVisited) continue;
        if (maze[rowIndex][colIndex].isBomb()) continue;

        if (minDistanceCell == NULL) {
          minDistanceCell = &maze[rowIndex][colIndex];
        } else if (maze[rowIndex][colIndex].distance < minDistanceCell->distance) {
          minDistanceCell = &maze[rowIndex][colIndex];
        }
      }
    }

    return minDistanceCell;
  }

  int findShortestPathUtil(Cell& source, Cell& destination) {
    const bool isInputValid = source.row >= 0 && source.row < m &&
                              source.col >= 0 && source.col < n &&
                              !source.isBomb() && !destination.isBomb();
    if (!isInputValid) return INT_MAX;
    source.isVisited = true;

    if (source.row == destination.row && source.col == destination.col) {
      return destination.distance;
    }

    const vector<Cell*> neighbours = GetNeighbours(source);

    if (neighbours.empty()) return INT_MAX;

    for(int index = 0; index < neighbours.size(); ++index) {
      Cell* currentNeighbour = neighbours[index];
      const int distanceToThisNeighbour = source.distance + 1;

      if (distanceToThisNeighbour < currentNeighbour->distance) {
        currentNeighbour->distance = distanceToThisNeighbour;
        currentNeighbour->previous = &source;
      }
    }

    Cell* minDistanceCell = GetMinDistanceCell();

    if (minDistanceCell == NULL) return INT_MAX;

    return findShortestPathUtil(*minDistanceCell, destination);
  }

  int findShortestPath(Cell& source, Cell& destination) {
    const bool isInputValid = source.row >= 0 && source.row < m &&
                              source.col >= 0 && source.col < n &&
                              !source.isBomb() && !destination.isBomb();

    if (!isInputValid) return INT_MAX;

    source.distance = 0;

    return findShortestPathUtil(source, destination);
  }
};


int main(int argc, char** argv)  {
  Maze maze(4, 4);
  maze.setBomb(1, 1);
  maze.setBomb(1, 3);
  maze.setBomb(2, 2);
  const int distance = maze.findShortestPath(maze.getCell(0, 0), maze.getCell(3, 3));
  cout << "Shortest Path: " << distance << endl;
  std::stack<Cell*> shortestpath;
  Cell* currentCell = &maze.getCell(3, 3);

  while(currentCell != NULL) {
    shortestpath.push(currentCell);
    currentCell = currentCell->previous;
  }

  while(!shortestpath.empty()) {
    Cell* eachCell = shortestpath.top();
    shortestpath.pop();
    cout << "(" << eachCell->row << ", " << eachCell->col << ")" << endl;
  }

  return 0;
}
