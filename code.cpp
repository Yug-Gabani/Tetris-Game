#include <bits/stdc++.h> // of c++
#include <conio.h>       // input
#include <windows.h>     // for windows specific functions

using namespace std;

int dropSpeed = 700;
const int WIDTH = 15;
const int HEIGHT = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = {0, 0};

// Tetromino class with rotation states
struct Tetromino {
    vector<vector<vector<int>>> rotations; // all possible rotation states
    string emoji; // emoji to represent the tetromino
    int rotationState; // current rotation state

    Tetromino(vector<vector<vector<int>>> r, string e) : rotations(r), emoji(e), rotationState(0) {}

    void rotate() {
        rotationState = (rotationState + 1) % rotations.size(); // cycle through rotations
    }

    vector<vector<int>> getShape() {
        return rotations[rotationState]; // return the shape for the current rotation state
    }
};

// Tetromino Definitions
vector<Tetromino> shape = {
    {{{{1}, {1}, {1}, {1}}, {{1, 1, 1, 1}}}, "🟦"}, // I piece
    {{{{1, 1}, {1, 1}}}, "🟨"},                     // Square piece
    {{{{1, 1, 1}, {0, 1, 0}}, 
      {{0, 1}, {1, 1}, {0, 1}}, 
      {{0, 1, 0}, {1, 1, 1}}, 
      {{1, 0}, {1, 1}, {1, 0}}}, "🟪"},             // T piece
    {{{{0, 1, 1}, {1, 1, 0}}, 
      {{1, 0}, {1, 1}, {0, 1}}}, "🟩"},             // S piece
    {{{{1, 1, 0}, {0, 1, 1}}, 
      {{0, 1}, {1, 1}, {1, 0}}}, "🟥"},             // Z piece
    {{{{0, 1}, {0, 1}, {1, 1}}, 
      {{1, 0, 0}, {1, 1, 1}}, 
      {{1, 1}, {1, 0}, {1, 0}}, 
      {{1, 1, 1}, {0, 0, 1}}}, "🟫"},               // _| piece
    {{{{1, 0}, {1, 0}, {1, 1}}, 
      {{1, 1, 1}, {1, 0, 0}}, 
      {{1, 1}, {0, 1}, {0, 1}}, 
      {{0, 0, 1}, {1, 1, 1}}}, "🟧"}                // L piece
};

vector<vector<string>> board(HEIGHT, vector<string>(WIDTH, "⬜"));
Tetromino currentPiece = shape[0]; // current tetromino (will be set in spawnPiece)
Tetromino nextPiece = shape[0];    // next tetromino (initialized to a dummy value)
int pieceX = WIDTH / 2 - 1;
int pieceY = 0;
int score = 0;

void moveCursor(int x, int y) {
    cursorPos.X = x;
    cursorPos.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPos);
}

void displayBoard() {
    system("cls");
    moveCursor(0, 0);
    
    ostringstream output;
    
    // Display the next shape.
    output << "Next Piece:\n";
    vector<vector<int>> nextShape = nextPiece.getShape();
    for (int i = 0; i < nextShape.size(); i++) {
        for (int j = 0; j < nextShape[i].size(); j++) {
            if (nextShape[i][j] == 1)
                output << nextPiece.emoji;
            else
                output << "  "; // two spaces for alignment
        }
        output << "\n";
    }
    output << "\n";
    
    // Draw the board with the current piece overlay.
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬜" &&
                pieceY <= i && i < pieceY + currentPiece.getShape().size() &&
                pieceX <= j && j < pieceX + currentPiece.getShape()[0].size() &&
                currentPiece.getShape()[i - pieceY][j - pieceX] == 1) {
                output << currentPiece.emoji;
            } else {
                output << board[i][j];
            }
        }
        output << "\n";
    }
    
    // Display score
    output << "Score: " << score << "\n";
    
    cout << output.str();
}

bool isValidMove(Tetromino &piece, int x, int y) {
    vector<vector<int>> shape = piece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                int newX = x + j, newY = y + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || board[newY][newX] != "⬜") {
                    return false;
                }
            }
        }
    }
    return true;
}

void placePiece() {
    vector<vector<int>> shape = currentPiece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                board[pieceY + i][pieceX + j] = currentPiece.emoji;
            }
        }
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (count(board[i].begin(), board[i].end(), "⬜") == 0) {
            board.erase(board.begin() + i);
            board.insert(board.begin(), vector<string>(WIDTH, "⬜"));
            score += 100;
            dropSpeed = max(100, dropSpeed - 50);
        }
    }
}

void spawnPiece() {
    // Use the pre-generated nextPiece as the current piece.
    currentPiece = nextPiece;
    pieceX = WIDTH / 2 - 1;
    pieceY = 0;
    
    // Generate the next tetromino randomly.
    nextPiece = shape[rand() % shape.size()];
    
    if (!isValidMove(currentPiece, pieceX, pieceY)) {
        int maxx = 0;
        maxx = max(maxx, score);
        cout << "Game Over! Final Score: " << score << "\n";
        cout << "Maximum Score: " << maxx << "\n";
        cout << "Press ENTER to play again or ESC to exit...";
        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 13) {
                    score = 0;
                    board.assign(HEIGHT, vector<string>(WIDTH, "⬜"));
                    spawnPiece();
                    return;
                }
                if (key == 27) {
                    exit(0);
                }
            }
        }
    }
}

void handleInput() {
    while (_kbhit()) {
        char key = _getch();
        int newX = pieceX, newY = pieceY;
        Tetromino newPiece = currentPiece;

        if (key == 75 && isValidMove(currentPiece, pieceX - 1, pieceY)) newX--; // left arrow
        if (key == 77 && isValidMove(currentPiece, pieceX + 1, pieceY)) newX++; // right arrow
        if (key == 80 && isValidMove(currentPiece, pieceX, pieceY + 1)) newY++; // down arrow
        if (key == 72) { // up arrow: rotate
            newPiece.rotate();
            if (isValidMove(newPiece, pieceX, pieceY))
                currentPiece = newPiece;
        }
        if (key == 32) // space bar: drop piece to bottom
            while (isValidMove(currentPiece, pieceX, newY + 1)) newY++;
        if (key == 27) { // escape key to exit
            exit(0);
        }
        pieceX = newX;
        pieceY = newY;
    }
}

void gameLoop() {
    while (true) {
        handleInput();
        if (isValidMove(currentPiece, pieceX, pieceY + 1)) {
            pieceY++;
        } else {
            placePiece();
            clearLines();
            spawnPiece();
        }
        displayBoard();
        Sleep(dropSpeed);
    }
}

int main() {
    srand(time(0));
    SetConsoleOutputCP(CP_UTF8);
    
    // Initialize nextPiece before starting the game.
    nextPiece = shape[rand() % shape.size()];
    
    spawnPiece();
    gameLoop();
    return 0;
}
