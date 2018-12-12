#include "tet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

int Table[20][11] = {0};
int score = 0;
char GameOn = 1;
suseconds_t timer = 500000;

Grid current;

const Grid grid[7] = {
        {(char *[]) {(char[]) {0, 1, 1}, (char[]) {1, 1, 0}, (char[]) {0, 0, 0}},                                 3},
        {(char *[]) {(char[]) {1, 1, 0}, (char[]) {0, 1, 1}, (char[]) {0, 0, 0}},                                 3},
        {(char *[]) {(char[]) {0, 1, 0}, (char[]) {1, 1, 1}, (char[]) {0, 0, 0}},                                 3},
        {(char *[]) {(char[]) {0, 0, 1}, (char[]) {1, 1, 1}, (char[]) {0, 0, 0}},                                 3},
        {(char *[]) {(char[]) {1, 0, 0}, (char[]) {1, 1, 1}, (char[]) {0, 0, 0}},                                 3},
        {(char *[]) {(char[]) {1, 1}, (char[]) {1, 1}},                                                           2},
        {(char *[]) {(char[]) {0, 0, 0, 0}, (char[]) {1, 1, 1, 1}, (char[]) {0, 0, 0, 0}, (char[]) {0, 0, 0, 0}}, 4}
};

Grid copyG(Grid g) {
    Grid newG = g;
    char **copy = g.matrix;
    newG.matrix = (char **) malloc(newG.width * sizeof(char *));
    for (int i = 0; i < newG.width; i++) {
        newG.matrix[i] = (char *) malloc(newG.width * sizeof(char));
        for (int j = 0; j < newG.width; j++) {
            newG.matrix[i][j] = copy[i][j];
        }
    }
    return newG;
}

int checkPos(Grid g) {
    char **matrix2 = g.matrix;
    for (int i = 0; i < g.width; i++) {
        for (int j = 0; j < g.width; j++) {
            if ((g.col + j < 0 || g.col + j >= 11 || g.row + i >= 20)) {
                if (matrix2[i][j])
                    return 0;
            } else if (Table[g.row + i][g.col + j] && matrix2[i][j])
                return 0;
        }
    }
    return 1;
}

void getG() {
    Grid newG = copyG(grid[rand() % 7]);

    newG.col = rand() % (11 - newG.width + 1);
    newG.row = 0;
    current = newG;
    if (!checkPos(current)) {
        GameOn = 0;
    }
}

void rotate(Grid g) {
    Grid temp = copyG(g);
    int width = g.width;
    int k;
    for (int i = 0; i < width; i++) {
        for (int j = 0, k = width - 1; j < width; j++, k--) {
            g.matrix[i][j] = temp.matrix[k][i];
        }
    }
}

void copyT() {
    for (int i = 0; i < current.width; i++) {
        for (int j = 0; j < current.width; j++) {
            if (current.matrix[i][j])
                Table[current.row + i][current.col + j] = current.matrix[i][j];
        }
    }
}

void printTable() {
    char buf[20][11] = {0};
    for (int i = 0; i < current.width; i++) {
        for (int j = 0; j < current.width; j++) {
            if (current.matrix[i][j])
                buf[current.row + i][current.col + j] = current.matrix[i][j];
        }
    }
    clear();
    for (int i = 0; i < 20; i++) {
        printw("|");
        for (int j = 0; j < 11; j++) {
            printw("%c ", (Table[i][j] + buf[i][j]) ? '*' : ' ');
        }
        printw("|");
        printw("\n");
    }
    printw("------------------------");
}

void changeCur(int x) {
    Grid g = copyG(current);
    switch (x) {
        case 's':
            g.row++;
            if (checkPos(g))
                current.row++;
            else {
                copyT();
                getG();
            }
            break;
        case 'd':
            g.col++;
            if (checkPos(g))
                current.col++;
            break;
        case 'a':
            g.col--;
            if (checkPos(g))
                current.col--;
            break;
        case 'w':
            rotate(g);
            if (checkPos(g))
                rotate(current);
            break;
    }
    printTable();
}

struct timeval before, after;

int checkTime() {
    return ((suseconds_t) (after.tv_sec * 1000000 + after.tv_usec) -
            ((suseconds_t) before.tv_sec * 1000000 + before.tv_usec)) > timer;
}

void reset() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 11; j++) {
            Table[i][j] = 0;
        }
    }
}

void runTETRIS(void) {
    reset();
    GameOn = 1;
    srand(time(0));
    int c;
    gettimeofday(&before, NULL);
    nodelay(stdscr, TRUE);
    struct timespec ts = {0, 1000000};
    timeout(1);
    getG();
    printTable();
    noecho();
    while (GameOn) {
        if ((c = getch()) != ERR) {
            changeCur(c);
        }
        gettimeofday(&after, NULL);
        if (checkTime()) { //time difference
            changeCur('s');
            gettimeofday(&before, NULL);
        }
    }
    printw("\nGame over!\n");
    refresh();
    while (getch() != 'x') {
    }
}
