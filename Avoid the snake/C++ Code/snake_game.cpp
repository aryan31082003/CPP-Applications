#include <iostream>
#include <fstream>
using namespace std;

const int MAX_ROWS = 20;
const int MAX_COLS = 20;
const char WALL = '#';
const char EMPTY = ' ';
const char PLAYER = '^';
const char PLAYERUP = '^';
const char PLAYERDOWN = 'v';                                                //2K22/CO/102 Aryan Gahlaut
const char PLAYERRIGHT = '>';
const char PLAYERLEFT = '<';
const char SNAKE = '~';
const char GOAL = 'X';
const char OBSTACLE = 'O';
int moves = 0;

struct Location {
    int row;
    int col;
};

template <typename T>
struct Node {
    T data;
    Node* next;
};                                                                                             //2K22/CO/102 Aryan Gahlaut

template <typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void push(const T& value) {
        Node<T>* newNode = new Node<T>;
        newNode->data = value;
        newNode->next = head;
        head = newNode;
    }

    T pop() {
        if (head) {
            T value = head->data;
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return value;
        } else {
            
            return T(); 
        }
    }
                                                                                             //2K22/CO/102 Aryan Gahlaut
    bool isEmpty() {
        return head == nullptr;
    }

    ~LinkedList() {
        while (!isEmpty()) {
            pop();
        }
    }

private:
    Node<T>* head;
};

class MazeGame {
public:
    MazeGame(const string& mapFileName);
    void play();

private:
    char gameMap[MAX_ROWS][MAX_COLS];
    int numRows;
    int numCols;
    int numObj;

    Location player;                                                                                       //2K22/CO/102 Aryan Gahlaut
    Location snake;
    Location goal;
    LinkedList<Location> moveHistory;
    LinkedList<Location> moveSnakeHistory; 

    void loadMap(const string& mapFileName);
    void printMap();
    bool isValidMove(int newRow, int newCol);
    bool isGameOver();
    bool movePlayer(char move);
    bool moveSnake();
    void undo();
    bool isWin();
};

MazeGame::MazeGame(const string& mapFileName) {
    loadMap(mapFileName);
   
}

void MazeGame::loadMap(const string& mapFileName) {
    ifstream mapFile(mapFileName);
    if (!mapFile) {
        cerr << "Error: File not found." << endl;
        exit(1);
    }
    mapFile >> numObj >> numRows >> numCols;
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            gameMap[i][j] = EMPTY;
        }
    }
    for (int i = 0; i < numObj; i++) {
        int r, c, code;
        mapFile >> r >> c >> code;
        if (code == 0) {                                                                              //2K22/CO/102 Aryan Gahlaut
            snake.row = r;
            snake.col = c;
            gameMap[r][c] = SNAKE;
        } else if (code == 1) { 
            player.row = r;
            player.col = c;
            gameMap[r][c] = PLAYER;
        } else if (code == 2) { 
            goal.row = r;
            goal.col = c;
            gameMap[r][c] = GOAL;
        } else if (code == 3) {
            gameMap[r][c] = OBSTACLE;
            
        }
    }

    for (int i = 0; i < numRows; i++) {
        gameMap[i][0] = WALL;
        gameMap[i][numCols - 1] = WALL;
    }
    for (int i = 0; i < numCols; i++) {
        gameMap[0][i] = WALL;
        gameMap[numRows - 1][i] = WALL;
    }

    
    mapFile.close();
}

void MazeGame::printMap() {                                                                         //2K22/CO/102 Aryan Gahlaut
    system("cls");  
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            cout << gameMap[row][col] << ' ';
        }
        cout << endl;
    }
}
                    
bool MazeGame::isValidMove(int newRow, int newCol) {
    return (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols &&
            gameMap[newRow][newCol] != OBSTACLE && gameMap[newRow][newCol] != WALL);
}

bool MazeGame::movePlayer(char move) {
    int newRow = player.row;
    int newCol = player.col;

    if (move == 'u') newRow--;
    else if (move == 'd') newRow++;
    else if (move == 'l') newCol--;
    else if (move == 'r') newCol++;

    if (isValidMove(newRow, newCol)) {                                                                   //2K22/CO/102 Aryan Gahlaut
        moveHistory.push(player); 
        gameMap[player.row][player.col] = EMPTY;
        player.row = newRow;
        player.col = newCol;
        if(move=='u')gameMap[player.row][player.col] = PLAYERUP;
       else if(move=='d')gameMap[player.row][player.col] = PLAYERDOWN;
       else if(move=='l')gameMap[player.row][player.col] = PLAYERLEFT;
       else if(move=='r')gameMap[player.row][player.col] = PLAYERRIGHT;
        return true;
    }

    return false;
}

bool MazeGame::moveSnake() {
   
    int newRow = snake.row;
    int newCol = snake.col;

   
    if (player.row < newRow&&gameMap[newRow-1][newCol]!=OBSTACLE) newRow--;
    else if (player.row > newRow&&gameMap[newRow+1][newCol]!=OBSTACLE) newRow++;
    else if (player.col < newCol&&gameMap[newRow][newCol-1]!=OBSTACLE) newCol--;
    else if (player.col > newCol&&gameMap[newRow][newCol+1]!=OBSTACLE) newCol++;

  
    if (isValidMove(newRow,newCol)) {
        moveSnakeHistory.push(snake);                                                                   //2K22/CO/102 Aryan Gahlaut
        gameMap[snake.row][snake.col] = EMPTY;
        snake.row = newRow;
        snake.col = newCol;
        gameMap[snake.row][snake.col] = SNAKE;
    }

   
    if (player.row == snake.row && player.col == snake.col) {
        return true;
    } else {
        return false;
    }
}

void MazeGame::undo() {
    if (!moveHistory.isEmpty()) {
        cout<<1<<endl;
        Location previous = moveHistory.pop(); 
        gameMap[player.row][player.col] = EMPTY;
        player = previous;
        gameMap[player.row][player.col] = PLAYER;
    }
    if (!moveSnakeHistory.isEmpty()) {
        Location prev = moveSnakeHistory.pop(); 
        cout<<1<<endl; 
        gameMap[snake.row][snake.col] = EMPTY;
        snake = prev;
        gameMap[snake.row][snake.col] = SNAKE;
    }
}
                                                                                     //2K22/CO/102 Aryan Gahlaut
bool MazeGame::isWin() {
    return (player.row == goal.row && player.col == goal.col);
}

bool MazeGame::isGameOver() {
    return (player.row == snake.row && player.col == snake.col);
}

void MazeGame::play(){
    while (true) {
        printMap();

        if (isWin()) {
            cout << "Congratulations for winnig the game!" << endl;
            break;
        }    
        char move;
        cout << "Enter for move (u(up)/l(left)/d(down)/r(right)/n(undo)): ";
        cin >> move;

        if (move == 'n') {
            undo();
        } else if (move == 'u' || move == 'l' || move == 'd' || move == 'r') {
            movePlayer(move);
             if (moveSnake()) {
            cout << "Game Over! YOU LOSE." << endl;
            break;                                                                                 //2K22/CO/102 Aryan Gahlaut
        }
        }

       
    }
}

int main() {
    string mapFileName = "maps.txt";  

    MazeGame game(mapFileName); 
    game.play();

    return 0;
}
                                                                                     //2K22/CO/102 Aryan Gahlaut