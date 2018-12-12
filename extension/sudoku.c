#include "sudoku.h"

char initial[9][9] = {
        {' ', ' ', ' ', '2', '6', ' ', '7', ' ', '1'},
        {'6', '8', ' ', ' ', '7', ' ', ' ', '9', ' '},
        {'1', '9', ' ', ' ', ' ', '4', '5', ' ', ' '},
        {'8', '2', ' ', '1', ' ', ' ', ' ', '4', ' '},
        {' ', ' ', '4', '6', ' ', '2', '9', ' ', ' '},
        {' ', '5', ' ', ' ', ' ', '3', ' ', '2', '8'},
        {' ', ' ', '9', '3', ' ', ' ', ' ', '7', '4'},
        {' ', '4', ' ', ' ', '5', ' ', ' ', '3', '6'},
        {'7', ' ', '3', ' ', '1', '8', ' ', ' ', ' '}
};

char solution[9][9] = {
        {'4', '3', '5', '2', '6', '9', '7', '8', '1'},
        {'6', '8', '2', '5', '7', '1', '4', '9', '3'},
        {'1', '9', '7', '8', '3', '4', '5', '6', '2'},
        {'8', '2', '6', '1', '9', '5', '3', '4', '7'},
        {'3', '7', '4', '6', '8', '2', '9', '1', '5'},
        {'9', '5', '1', '7', '4', '3', '6', '2', '8'},
        {'5', '1', '9', '3', '2', '6', '8', '7', '4'},
        {'2', '4', '8', '9', '5', '7', '1', '3', '6'},
        {'7', '6', '3', '4', '1', '8', '2', '5', '9'}
};

int checkSol(char matrix[][9]){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (matrix[i][j] != solution[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void printGrid(char matrix[][9]) {
    for(int i = 0; i < 9; i++){
        printw("|");
        for(int j = 0; j < 9; j++){
            char value = matrix[i][j];
            printw("%c", value);
            printw("|");
        }
        printw("\n");
    }
}

void copyBoard(char grid[][9], char new_grid[][9]) {
    //Copy over values from one grid to another without altering addresses
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            new_grid[i][j] = grid[i][j];
        }
    }
}

int runSudoku(void) {
    char current[9][9];
    copyBoard(initial, current);
    while (1) {
        //Take initial dimensions
        printw("Press c to make a move or press x to quit: ");
        refresh();

        noecho();
        char c = 'a';
        while (c != 'c' && c != 'x') {
            c = getch();
        }
        if (c == 'x') {
            break;
        }
        echo();
        clear();

        printGrid(current);
        refresh();
        char input[6];

        printw("Please enter the coordinates of the cell you would like to fill: ");
        refresh();
        getnstr(input, sizeof(input));

        int x = atoi(strtok(input, ","));
        int y = atoi(strtok(NULL, " "));

        char number;

        //Take initial dimensions
        printw("Which value should be put in the cell?");
        refresh();
        scanf("%c", &number);
        clear();

        current[x][y] = number;

        clear();
        printGrid(current);
        refresh();

        if (checkSol(current)) {
            printw("Congratulations, you win!");
            refresh();
            getch();
            break;
        }
    }
    return 0;
}

