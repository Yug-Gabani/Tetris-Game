# Tetris-Game

<h1>➡️Introduction</h1>

![Image](https://github.com/user-attachments/assets/bb6c5ac9-5bc2-4707-83d5-fc2cbc977e06)

<br>This project is a console-based Tetris game implemented in C++. It features Tetrominoes, movement controls, rotation, scoring, and increasing difficulty as the player progresses.

<h2>Features</h2>
<br>Supports different Tetromino shapes following the original Tetris rules.
<br>Allows movement (left, right, down) and rotation of pieces.
<br>Detects collisions and line completions.
<br>Increases difficulty as the game progresses.
<br>Displays the current score and game-over conditions.

<h2>Tetromino Shapes</h2>
<br>Below are the standard Tetromino shapes used in the game:

<br>I-Shape: Four blocks in a straight line.
<br>O-Shape: Square shape.
<br>T-Shape: Three blocks in a row with one in the center.
<br>S-Shape: Zig-zag pattern.
<br>Z-Shape: Reverse of S-Shape.
<br>J-Shape: Three blocks in a row with one extending down.
<br>L-Shape: Three blocks in a row with one extending up.

<h2>Controls</h2>
<br>Left Arrow (←): Move left
<br>Right Arrow (→): Move right
<br>Down Arrow (↓): Move down faster
<br>Up Arrow (↑): Rotate the piece
<br>Spacebar: Instantly drop the piece
<br>Enter: Restart the game after Game Over
<br>Escape: Exit the game

<h2>How to Play</h2>
<br>A random Tetromino falls from the top of the board.
<br>Use the arrow keys to move and rotate the piece.
<br>Completing a full row clears it and increases your score.
<br>The game ends when the pieces stack up to the top of the board.
<br>The objective is to achieve the highest score possible.

<h2>Requirements</h2>

<br>Windows OS (due to the use of windows.h and conio.h)
A C++ compiler supporting Windows API functions
Compilation and Execution

<h2>Compile the game using g++:</h2><br>
g++ tetris.cpp -o tetris.exe<br>

Run the game:<br>
tetris.exe<br>

<h2>Notes</h2>
<br>The game runs in a console window and uses Unicode characters for better visuals.
<br>The drop speed increases as the player clears more lines.

<br>Enjoy playing Tetris!

