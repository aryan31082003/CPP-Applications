#include "mainwindow.h"
#include <QPainterPath>
#include <QApplication> // Needed for the exit button (qApp)

// ========================
// == CONSTRUCTOR
// ========================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize our game object
    game = new MazeGame("maps.txt");

    // Set a fixed window size based on the map dimensions and tile size
    int windowWidth = game->numCols * TILE_SIZE;
    int windowHeight = game->numRows * TILE_SIZE;
    setFixedSize(windowWidth, windowHeight);
    setWindowTitle("Qt Snake Game");

    // --- Create Buttons ---

    // Start Button
    startButton = new QPushButton("Start Game", this);
    startButton->setGeometry(windowWidth / 2 - 100, windowHeight / 2 - 25, 200, 50);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    // Restart Button
    restartButton = new QPushButton("Restart", this);
    restartButton->setGeometry(windowWidth / 2 - 100, windowHeight / 2 - 50, 200, 50);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);

    // Exit Button
    exitButton = new QPushButton("Exit", this);
    exitButton->setGeometry(windowWidth / 2 - 100, windowHeight / 2 + 10, 200, 50);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // --- Set Initial State ---
    currentState = MainMenu;
    restartButton->hide();
    exitButton->hide();
    startButton->show();
}

MainWindow::~MainWindow()
{
    delete game;
}

// ========================
// == GAME FLOW SLOTS
// ========================
void MainWindow::startGame()
{
    currentState = Playing;
    startButton->hide(); // Hide the start button
    this->setFocus();    // <-- ADD THIS LINE to grab keyboard focus
    update(); // Redraw the screen to show the game
}

void MainWindow::restartGame()
{
    restartButton->hide();
    exitButton->hide();

    // Reset the game logic
    delete game;
    game = new MazeGame("maps.txt");

    // Start playing again
    currentState = Playing;
    this->setFocus();    // <-- ALSO ADD THIS LINE here
    update(); // Redraw the screen with the new game
}

void MainWindow::showGameOverUI()
{
    currentState = GameOver;
    restartButton->show();
    exitButton->show();
}


// ========================
// == EVENT HANDLERS
// ========================

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw a solid dark background for all states
    painter.fillRect(this->rect(), QColor("#2d2d2d"));

    if (currentState == MainMenu)
    {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(this->rect(), Qt::AlignCenter | Qt::AlignTop, "Welcome to Snake Maze!");
        return; // Don't draw anything else
        }

    // --- Draw the game board (for Playing and GameOver states) ---
    for (int row = 0; row < game->numRows; ++row) {
        for (int col = 0; col < game->numCols; ++col) {
            QRectF rect(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            char currentTile = game->gameMap[row][col];

            switch (currentTile) {
            // ... (The entire switch case for drawing tiles is the same as before)
            case WALL:
                painter.fillRect(rect.adjusted(2, 2, -2, -2), QColor("#666666"));
                break;
            case SNAKE: {
                painter.setBrush(QColor("#e53935")); painter.setPen(Qt::NoPen);
                QPainterPath snakePath;
                snakePath.moveTo(rect.center().x(), rect.top()); snakePath.lineTo(rect.right(), rect.center().y());
                snakePath.lineTo(rect.center().x(), rect.bottom()); snakePath.lineTo(rect.left(), rect.center().y());
                snakePath.closeSubpath(); painter.drawPath(snakePath);
                break; }
            case GOAL: {
                painter.setBrush(QColor("#FFEB3B")); painter.setPen(Qt::NoPen);
                QPainterPath starPath;
                QPointF center = rect.center(); double outerRadius = TILE_SIZE / 2.0; double innerRadius = TILE_SIZE / 4.0;
                for (int i = 0; i < 10; ++i) {
                    double radius = (i % 2 == 0) ? outerRadius : innerRadius;
                    double angle = i * 36.0 * 3.14159 / 180.0 - (3.14159 / 2.0);
                    QPointF point(center.x() + radius * cos(angle), center.y() + radius * sin(angle));
                    if (i == 0) starPath.moveTo(point); else starPath.lineTo(point);
                }
                starPath.closeSubpath(); painter.drawPath(starPath);
                break; }
            case OBSTACLE:
                painter.fillRect(rect.adjusted(4, 4, -4, -4), QColor("#0097A7"));
                break;
            case PLAYERUP: case PLAYERDOWN: case PLAYERLEFT: case PLAYERRIGHT: {
                painter.setBrush(QColor("#4CAF50")); painter.setPen(Qt::NoPen);
                QPainterPath playerPath;
                if(currentTile == PLAYERUP) { playerPath.moveTo(rect.center().x(), rect.top()); playerPath.lineTo(rect.right(), rect.bottom()); playerPath.lineTo(rect.left(), rect.bottom()); }
                else if (currentTile == PLAYERDOWN) { playerPath.moveTo(rect.center().x(), rect.bottom()); playerPath.lineTo(rect.right(), rect.top()); playerPath.lineTo(rect.left(), rect.top()); }
                else if (currentTile == PLAYERLEFT) { playerPath.moveTo(rect.left(), rect.center().y()); playerPath.lineTo(rect.right(), rect.top()); playerPath.lineTo(rect.right(), rect.bottom()); }
                else { playerPath.moveTo(rect.right(), rect.center().y()); playerPath.lineTo(rect.left(), rect.top()); playerPath.lineTo(rect.left(), rect.bottom()); }
                playerPath.closeSubpath(); painter.drawPath(playerPath);
                break; }
            }
        }
    }

    // If the game is over, draw the message text
    if (currentState == GameOver) {
        // Semi-transparent overlay
        painter.fillRect(this->rect(), QColor(0, 0, 0, 180));
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        QString message = game->gameWon ? "YOU WIN! 🎉" : "GAME OVER! 🐍";
        painter.drawText(this->rect(), Qt::AlignCenter, message);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Only process key presses if we are in the "Playing" state
    if (currentState != Playing) {
        return;
    }

    switch (event->key()) {
    case Qt::Key_W: case Qt::Key_Up:
        game->movePlayer('u'); game->moveSnake();
        break;
    case Qt::Key_S: case Qt::Key_Down:
        game->movePlayer('d'); game->moveSnake();
        break;
    case Qt::Key_A: case Qt::Key_Left:
        game->movePlayer('l'); game->moveSnake();
        break;
    case Qt::Key_D: case Qt::Key_Right:
        game->movePlayer('r'); game->moveSnake();
        break;
    case Qt::Key_N:
        game->undo();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }

    // After a move, check if the game has ended
    if (game->gameWon || game->gameOver) {
        showGameOverUI();
    }

    update(); // Schedule a repaint
}
