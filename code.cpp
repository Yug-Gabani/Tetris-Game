#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
const int BLOCK_SIZE = 4;

const int SHAPES[7][BLOCK_SIZE][BLOCK_SIZE] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // I
    {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},  // O
    {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}},  // T
    {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},  // S
    {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},  // Z
    {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}},  // J
    {{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}   // L
};

enum Color {
    CYAN = 11, YELLOW = 14, MAGENTA = 13, GREEN = 10, RED = 12, BLUE = 1, BROWN = 6,
    BOUNDARY = 9
};

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor() {
    setColor(15);
}

void gotoxy(int x, int y) {
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Tetromino {
public:
    int shape[BLOCK_SIZE][BLOCK_SIZE];
    int x, y;
    int type;
    Color color;

    Tetromino() {
        type = rand() % 7;
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                shape[i][j] = SHAPES[type][i][j];
        x = WIDTH / 2 - BLOCK_SIZE / 2;
        y = 0;
        switch (type) {
            case 0: color = CYAN; break;
            case 1: color = YELLOW; break;
            case 2: color = MAGENTA; break;
            case 3: color = GREEN; break;
            case 4: color = RED; break;
            case 5: color = BLUE; break;
            case 6: color = BROWN; break;
        }
    }

    void rotate() {
        int temp[BLOCK_SIZE][BLOCK_SIZE];
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                temp[j][BLOCK_SIZE - 1 - i] = shape[i][j];
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                shape[i][j] = temp[i][j];
    }
};

class TetrisGame {
private:
    vector<vector<int>> grid;
    vector<vector<int>> colorGrid;
    Tetromino currentPiece;
    vector<Tetromino> nextPieces;
    int score;
    bool firstDraw = true;
    bool paused = false;
    string playerName;

    bool isValidMove(int newX, int newY) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (currentPiece.shape[i][j]) {
                    int gridX = newX + j;
                    int gridY = newY + i;
                    if (gridX < 0 || gridX >= WIDTH || gridY >= HEIGHT || (gridY >= 0 && grid[gridY][gridX]))
                        return false;
                }
            }
        }
        return true;
    }

    void mergePiece() {
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                if (currentPiece.shape[i][j]) {
                    grid[currentPiece.y + i][currentPiece.x + j] = 1;
                    colorGrid[currentPiece.y + i][currentPiece.x + j] = currentPiece.color;
                }
    }

    void clearLines() {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++) {
                if (!grid[i][j]) {
                    full = false;
                    break;
                }
            }
            if (full) {
                grid.erase(grid.begin() + i);
                grid.insert(grid.begin(), vector<int>(WIDTH, 0));
                colorGrid.erase(colorGrid.begin() + i);
                colorGrid.insert(colorGrid.begin(), vector<int>(WIDTH, 0));
                score += 100;
            }
        }
    }

    void generateNextPieces() {
        while (nextPieces.size() < 3) {
            nextPieces.push_back(Tetromino());
        }
    }

public:
    TetrisGame() : grid(HEIGHT, vector<int>(WIDTH, 0)), colorGrid(HEIGHT, vector<int>(WIDTH, 0)), score(0) {
        srand(time(0));
        system("cls");
        cout << "Enter your name: ";
        getline(cin, playerName);
        cout << "\nControls:\n";
        cout << "  a - Move Left\n";
        cout << "  d - Move Right\n";
        cout << "  x - Soft Drop (faster down)\n";
        cout << "  w - Rotate Clockwise\n";
        cout << "  s - Rotate Counterclockwise\n";
        cout << "  p - Pause\n";
        cout << "  r - Resume\n";
        cout << "Press any key to start...";
        _getch();
        generateNextPieces();
    }

    void draw() {
        if (firstDraw) {
            system("cls");
            firstDraw = false;
        }
        gotoxy(0, 0);

        vector<vector<int>> tempGrid = grid;
        vector<vector<int>> tempColorGrid = colorGrid;
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                if (currentPiece.shape[i][j] && currentPiece.y + i >= 0) {
                    tempGrid[currentPiece.y + i][currentPiece.x + j] = 1;
                    tempColorGrid[currentPiece.y + i][currentPiece.x + j] = currentPiece.color;
                }

        cout << "Next Pieces:\n";
        for (int i = 0; i < 4; i++) {
            cout << "  ";
            for (int p = 0; p < 3; p++) {
                if (p < nextPieces.size()) {
                    for (int j = 0; j < 4; j++) {
                        setColor(nextPieces[p].color);
                        cout << (nextPieces[p].shape[i][j] ? "#" : " ");
                        resetColor();
                    }
                    cout << "  ";
                }
            }
            cout << endl;
        }
        cout << playerName << "'s Score: " << score << endl;

        setColor(BOUNDARY);
        cout << "+";
        for (int j = 0; j < WIDTH; j++) cout << "-";
        cout << "+\n";
        resetColor();

        for (int i = 0; i < HEIGHT; i++) {
            setColor(BOUNDARY);
            cout << "|";
            resetColor();
            for (int j = 0; j < WIDTH; j++) {
                if (tempGrid[i][j]) {
                    setColor(tempColorGrid[i][j]);
                    cout << "#";
                    resetColor();
                } else {
                    cout << " ";
                }
            }
            setColor(BOUNDARY);
            cout << "|\n";
            resetColor();
        }

        setColor(BOUNDARY);
        cout << "+";
        for (int j = 0; j < WIDTH; j++) cout << "-";
        cout << "+\n";
        resetColor();

        if (paused) {
            gotoxy(2, HEIGHT / 2 - 1);
            cout << "Paused";
            gotoxy(2, HEIGHT / 2);
            cout << "Press 'r' to Resume";
            gotoxy(2, HEIGHT / 2 + 1);
            cout << "Controls: a/d/x/w/s/p/r";
        }
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            int newX = currentPiece.x, newY = currentPiece.y;
            switch (key) {
                case 'a': newX--; break; // Move Left
                case 'd': newX++; break; // Move Right
                case 'x': newY++; break; // Soft Drop
                case 'w': { // Rotate Clockwise
                    Tetromino temp = currentPiece;
                    temp.rotate();
                    if (isValidMove(temp.x, temp.y)) {
                        currentPiece.rotate();
                    }
                    break;
                }
                case 's': { // Rotate Counterclockwise
                    Tetromino temp = currentPiece;
                    temp.rotate();
                    temp.rotate();
                    if (isValidMove(temp.x, temp.y)) {
                        currentPiece.rotate();
                        currentPiece.rotate();
                    }
                    break;
                }
                case 'p': paused = true; break; // Pause
                case 'r': paused = false; break; // Resume
            }
            if (!paused && isValidMove(newX, newY)) {
                currentPiece.x = newX;
                currentPiece.y = newY;
            }
        }
    }

    bool update() {
        if (paused) return true;
        if (isValidMove(currentPiece.x, currentPiece.y + 1)) {
            currentPiece.y++;
        } else {
            mergePiece();
            clearLines();
            if (nextPieces.empty()) {
                currentPiece = Tetromino();
                generateNextPieces();
            } else {
                currentPiece = nextPieces[0];
                nextPieces.erase(nextPieces.begin());
                generateNextPieces();
            }
            if (!isValidMove(currentPiece.x, currentPiece.y)) return false;
        }
        return true;
    }

    void run() {
        while (update()) {
            draw();
            handleInput();
            Sleep(200);
        }
        draw();
        gotoxy(0, HEIGHT + 7);
        cout << "Game Over, " << playerName << "! Final Score: " << score << endl;
    }
};

int main() {
    TetrisGame game;
    game.run();
    return 0;
}
