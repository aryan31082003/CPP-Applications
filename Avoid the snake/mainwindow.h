#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>
#include <fstream>
#include <QPushButton>
using namespace std;

// ===================================================================
// == YOUR ORIGINAL GAME LOGIC (UNCHANGED, PLACED INSIDE THE HEADER) ==
// ===================================================================

const int MAX_ROWS = 50;
const int MAX_COLS = 80;
const char WALL = '#';
const char EMPTY = ' ';
const char PLAYER = '^';
const char PLAYERUP = '^';
const char PLAYERDOWN = 'v';
const char PLAYERRIGHT = '>';
const char PLAYERLEFT = '<';
const char SNAKE = '~';
const char GOAL = 'X';
const char OBSTACLE = 'O';

struct Location {
    int row;
    int col;
};

template <typename T>
struct Node {
    T data;
    Node* next;
};

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
    // We need public access to the map and state from our Qt class
    char gameMap[MAX_ROWS][MAX_COLS];
    int numRows;
    int numCols;
    Location player;
    bool gameWon = false;
    bool gameOver = false;

    inline MazeGame(const string& mapFileName);
    inline void loadMap(const string& mapFileName);
    inline bool isValidMove(int newRow, int newCol);
    inline void movePlayer(char move);
    inline void moveSnake();
    inline void undo();

private:
    int numObj;
    Location snake;
    Location goal;
    LinkedList<Location> moveHistory;
    LinkedList<Location> moveSnakeHistory;
};

inline MazeGame::MazeGame(const string& mapFileName) {
    loadMap(mapFileName);
}

inline void MazeGame::loadMap(const string& mapFileName) {
    ifstream mapFile(mapFileName);
    if (!mapFile) {
        cerr << "Error: maps.txt not found. Make sure it's in the build directory." << endl;
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
        if (code == 0) {
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

inline bool MazeGame::isValidMove(int newRow, int newCol) {
    return (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols &&
            gameMap[newRow][newCol] != OBSTACLE && gameMap[newRow][newCol] != WALL);
}

inline void MazeGame::movePlayer(char move) {
    int newRow = player.row;
    int newCol = player.col;

    if (move == 'u') newRow--;
    else if (move == 'd') newRow++;
    else if (move == 'l') newCol--;
    else if (move == 'r') newCol++;

    if (isValidMove(newRow, newCol)) {
        moveHistory.push(player);
        gameMap[player.row][player.col] = EMPTY;
        player.row = newRow;
        player.col = newCol;
        if (move == 'u') gameMap[player.row][player.col] = PLAYERUP;
        else if (move == 'd') gameMap[player.row][player.col] = PLAYERDOWN;
        else if (move == 'l') gameMap[player.row][player.col] = PLAYERLEFT;
        else if (move == 'r') gameMap[player.row][player.col] = PLAYERRIGHT;
    }

    if (player.row == goal.row && player.col == goal.col) {
        gameWon = true;
    }
}

inline void MazeGame::moveSnake() {
    int newRow = snake.row;
    int newCol = snake.col;

    if (player.row < newRow && gameMap[newRow - 1][newCol] != OBSTACLE) newRow--;
    else if (player.row > newRow && gameMap[newRow + 1][newCol] != OBSTACLE) newRow++;
    else if (player.col < newCol && gameMap[newRow][newCol - 1] != OBSTACLE) newCol--;
    else if (player.col > newCol && gameMap[newRow][newCol + 1] != OBSTACLE) newCol++;

    if (isValidMove(newRow, newCol)) {
        moveSnakeHistory.push(snake);
        gameMap[snake.row][snake.col] = EMPTY;
        snake.row = newRow;
        snake.col = newCol;
        gameMap[snake.row][snake.col] = SNAKE;
    }

    if (player.row == snake.row && player.col == snake.col) {
        gameOver = true;
    }
}

inline void MazeGame::undo() {
    if (!moveHistory.isEmpty()) {
        Location previous = moveHistory.pop();
        gameMap[player.row][player.col] = EMPTY;
        player = previous;
        gameMap[player.row][player.col] = PLAYER;
    }
    if (!moveSnakeHistory.isEmpty()) {
        Location prev = moveSnakeHistory.pop();
        gameMap[snake.row][snake.col] = EMPTY;
        snake = prev;
        gameMap[snake.row][snake.col] = SNAKE;
    }
    gameWon = false;
    gameOver = false;
}

// ===================================
// == END OF YOUR ORIGINAL GAME LOGIC ==
// ===================================


// =============================================
// == QT MAIN WINDOW DECLARATION (THE "VIEW") ==
// =============================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Functions that are triggered by events (like button clicks)
    void startGame();
    void restartGame();

private:
    // An enum to keep track of the game's current state
    enum GameState {
        MainMenu,
        Playing,
        GameOver
    };

    GameState currentState; // Variable to hold the current state
    MazeGame *game;
    const int TILE_SIZE = 20;

    // Declare pointers for our buttons
    QPushButton *startButton;
    QPushButton *restartButton;
    QPushButton *exitButton;

    void showGameOverUI();
};
#endif // MAINWINDOW_H
