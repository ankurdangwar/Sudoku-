// SudokuGame.cpp
// Simple interactive Sudoku grid using C++ and OpenGL (FreeGLUT)
// Author: Ankur Dangwar

#include <GL/freeglut.h>
#include <iostream>
#include <vector>

// ------------------- CONFIG -------------------
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 9;
const int CELL_SIZE = WINDOW_WIDTH / GRID_SIZE;

// Store Sudoku board (0 means empty cell)
int board[GRID_SIZE][GRID_SIZE] = {0};

// Track selected cell
int selectedRow = -1, selectedCol = -1;

// ------------------- GAME LOGIC -------------------
bool isValidMove(int row, int col, int val) {
    // Check row
    for (int c = 0; c < GRID_SIZE; c++)
        if (board[row][c] == val) return false;

    // Check column
    for (int r = 0; r < GRID_SIZE; r++)
        if (board[r][col] == val) return false;

    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[boxRow + r][boxCol + c] == val) return false;

    return true;
}

// ------------------- GRAPHICS -------------------
void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawGrid() {
    glColor3f(0, 0, 0); // Black grid
    glLineWidth(1);

    for (int i = 0; i <= GRID_SIZE; i++) {
        glBegin(GL_LINES);
        glVertex2f(i * CELL_SIZE, 0);
        glVertex2f(i * CELL_SIZE, WINDOW_HEIGHT);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(0, i * CELL_SIZE);
        glVertex2f(WINDOW_WIDTH, i * CELL_SIZE);
        glEnd();
    }

    // Thicker lines for 3x3 boxes
    glLineWidth(3);
    for (int i = 0; i <= GRID_SIZE; i += 3) {
        glBegin(GL_LINES);
        glVertex2f(i * CELL_SIZE, 0);
        glVertex2f(i * CELL_SIZE, WINDOW_HEIGHT);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(0, i * CELL_SIZE);
        glVertex2f(WINDOW_WIDTH, i * CELL_SIZE);
        glEnd();
    }
}

void drawNumbers() {
    glColor3f(0.1, 0.1, 0.5); // Blue numbers
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (board[r][c] != 0) {
                float x = c * CELL_SIZE + CELL_SIZE / 2 - 5;
                float y = WINDOW_HEIGHT - (r * CELL_SIZE + CELL_SIZE / 2);
                drawText(x, y, std::to_string(board[r][c]));
            }
        }
    }
}

void highlightCell() {
    if (selectedRow == -1 || selectedCol == -1) return;

    glColor3f(0.9, 0.9, 0.6); // Light yellow
    glBegin(GL_QUADS);
    glVertex2f(selectedCol * CELL_SIZE, selectedRow * CELL_SIZE);
    glVertex2f((selectedCol + 1) * CELL_SIZE, selectedRow * CELL_SIZE);
    glVertex2f((selectedCol + 1) * CELL_SIZE, (selectedRow + 1) * CELL_SIZE);
    glVertex2f(selectedCol * CELL_SIZE, (selectedRow + 1) * CELL_SIZE);
    glEnd();
}

// ------------------- EVENT HANDLERS -------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    highlightCell();
    drawGrid();
    drawNumbers();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        selectedCol = x / CELL_SIZE;
        selectedRow = (WINDOW_HEIGHT - y) / CELL_SIZE;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (selectedRow == -1 || selectedCol == -1) return;

    if (key >= '1' && key <= '9') {
        int val = key - '0';
        if (isValidMove(selectedRow, selectedCol, val)) {
            board[selectedRow][selectedCol] = val;
        } else {
            std::cout << "Invalid Move!" << std::endl;
        }
    } else if (key == '0' || key == 8) {
        board[selectedRow][selectedCol] = 0; // clear
    }
    glutPostRedisplay();
}

// ------------------- MAIN -------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Sudoku Game - C++ OpenGL");

    glClearColor(1, 1, 1, 1); // White background

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
