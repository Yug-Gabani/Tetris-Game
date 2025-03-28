#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

int dropSpeed = 700;//Controls the speed at which the tetromino falls (milliseconds per step).
const int WIDTH = 15;
const int HEIGHT = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//Gets a handle to the console output for manipulating the screen.
COORD cursorPos = {0, 0};//Stores the cursor position for updating the console screen efficiently.

struct Tetromino {
    vector<vector<vector<int>>> rotations;//A list of all possible rotations of the piece.
    string emoji;//The Unicode character used to represent this piece.
    int rotationState;//Tracks the current rotation state (index in the rotations vector).

    Tetromino(vector<vector<vector<int>>> r, string e) : rotations(r), emoji(e), rotationState(0) {}

    void rotate() {
        rotationState = (rotationState + 1) % rotations.size();
    }

    vector<vector<int>> getShape() {
        return rotations[rotationState];
    }
};

vector<Tetromino> tetrominoes = {
    {{{{1}, {1}, {1}, {1}}, {{1, 1, 1, 1}}}, "🟦"},
    {{{{1, 1}, {1, 1}}}, "🟨"},
    {{{{1, 1, 1}, {0, 1, 0}}, {{0, 1}, {1, 1}, {0, 1}}, {{0, 1, 0}, {1, 1, 1}}, {{1, 0}, {1, 1}, {1, 0}}}, "🟪"},
    {{{{0, 1, 1}, {1, 1, 0}}, {{1, 0}, {1, 1}, {0, 1}}}, "🟩"},
    {{{{1, 1, 0}, {0, 1, 1}}, {{0, 1}, {1, 1}, {1, 0}}}, "🟥"},
    {{{{0, 1}, {0, 1}, {1, 1}}, {{1, 0, 0}, {1, 1, 1}}, {{1, 1}, {1, 0}, {1, 0}}, {{1, 1, 1}, {0, 0, 1}}}, "🟫"},
    {{{{1, 0}, {1, 0}, {1, 1}}, {{1, 1, 1}, {1, 0, 0}}, {{1, 1}, {0, 1}, {0, 1}}, {{0, 0, 1}, {1, 1, 1}}}, "🟧"}
};//This initializes all 7 Tetromino types with their possible rotations and assigns a Unicode block to each.

vector<vector<string>> board(HEIGHT, vector<string>(WIDTH, "⬜"));
Tetromino currentPiece = tetrominoes[0];
int pieceX = WIDTH / 2 - 1;
int pieceY = 0;
int score = 0;

void moveCursor(int x, int y) {
    cursorPos.X = x;
    cursorPos.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPos);
}

void displayBoard() {
    system("clear");
    moveCursor(0, 0);
    ostringstream output;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬜" && pieceY <= i && i < pieceY + currentPiece.getShape().size() && // this is for print a tetrimino which is running now.
                pieceX <= j && j < pieceX + currentPiece.getShape()[0].size() &&
                currentPiece.getShape()[i - pieceY][j - pieceX] == 1) {
                output << currentPiece.emoji;
            } else {    // this condition for old page which is print direct.
                output << board[i][j];
            }
        }
        output << "\n";
    }
    output << "Score: " << score << endl;
    cout << output.str();
}

bool isValidMove(Tetromino &piece, int x, int y) {
    vector<vector<int>> shape = piece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                int newX = x + j, newY = y + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || board[newY][newX] != "⬜") {  // if tetrimino going outside the displayboard then stop it. otherwise apply above condition.
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
            if (shape[i][j] == 1) {   // we made tetrimino like that if 1 then colouring box present and if 0 then absent.
                board[pieceY + i][pieceX + j] = currentPiece.emoji;
            }
        }
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (count(board[i].begin(), board[i].end(), "⬜") == 0) {  // count white box if zero then erase last row. and insert a row at top.
            board.erase(board.begin() + i);
            board.insert(board.begin(), vector<string>(WIDTH, "⬜"));
            score += 100;
            dropSpeed = max(100, dropSpeed - 50);
        }
    }
}

void spawnPiece() {
    currentPiece = tetrominoes[rand() % 7];  // tetrimino generator
    pieceX = WIDTH / 2 - 1;
    pieceY = 0;
    if (!isValidMove(currentPiece, pieceX, pieceY)) {
        int maxx = 0;
        maxx = max(maxx, score);
        cout << "Game Over! Final Score: " << score << endl;
        cout << "Maximum Score: " << maxx << endl;
        cout << "Press ENTER to play again or ESC to exit...";
        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 13) {
                    score = 0;  // enter
                    board.assign(HEIGHT, vector<string>(WIDTH, "⬜"));
                    spawnPiece();
                    return;
                }
                if (key == 27) {
                    exit(0);   // escape
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

        if (key == 75 && isValidMove(currentPiece, pieceX - 1, pieceY)) newX--;  //<--
        if (key == 77 && isValidMove(currentPiece, pieceX + 1, pieceY)) newX++;  //-->
        if (key == 80 && isValidMove(currentPiece, pieceX, pieceY + 1)) newY++;  // down
        if (key == 72) {       
            newPiece.rotate();
            if (isValidMove(newPiece, pieceX, pieceY)) currentPiece = newPiece; // rotate
        }
        if (key == 32) while (isValidMove(currentPiece, pieceX, newY + 1)) newY++;   // spacebar

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
    spawnPiece();
    gameLoop();
    return 0;
}
