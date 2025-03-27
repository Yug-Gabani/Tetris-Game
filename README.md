# Tetris-Game

<h1>Introduction</h1>

This project is a console-based Tetris game implemented in C++. It features Tetrominoes, movement controls, rotation, scoring, and increasing difficulty as the player progresses.

Features

Supports different Tetromino shapes following the original Tetris rules.

Allows movement (left, right, down) and rotation of pieces.

Detects collisions and line completions.

Increases difficulty as the game progresses.

Displays the current score and game-over conditions.

Tetromino Shapes

Below are the standard Tetromino shapes used in the game:

I-Shape: Four blocks in a straight line.

O-Shape: Square shape.

T-Shape: Three blocks in a row with one in the center.

S-Shape: Zig-zag pattern.

Z-Shape: Reverse of S-Shape.

J-Shape: Three blocks in a row with one extending down.

L-Shape: Three blocks in a row with one extending up.

Controls

Left Arrow (←): Move left

Right Arrow (→): Move right

Down Arrow (↓): Move down faster

Up Arrow (↑): Rotate the piece

Spacebar: Instantly drop the piece

Enter: Restart the game after Game Over

Escape: Exit the game

How to Play

A random Tetromino falls from the top of the board.

Use the arrow keys to move and rotate the piece.

Completing a full row clears it and increases your score.

The game ends when the pieces stack up to the top of the board.

The objective is to achieve the highest score possible.

Requirements

Windows OS (due to the use of windows.h and conio.h)

A C++ compiler supporting Windows API functions

Compilation and Execution

Compile the game using g++:

g++ tetris.cpp -o tetris.exe

Run the game:

tetris.exe

Notes

The game runs in a console window and uses Unicode characters for better visuals.

The drop speed increases as the player clears more lines.

Enjoy playing Tetris!

