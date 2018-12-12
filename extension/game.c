#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <zconf.h>
#include "grid.h"
#include "cell.h"
#include "initial_configs.h"
#include <ncurses.h>
#include <unistd.h>
#include <memory.h>
#include <stdbool.h>
#include "tet.h"
#include "sudoku.h"

void printScreen(cell **board, int width, int height){
    move(0, 0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            attron(COLOR_PAIR((i + j + 1) % 5));
            printw(symbol(board[i][j]));
        }
        printw("\n");
    }
}

void printUserInput(cell **game, int width, int height){
    char number[3];

    int options = 0;
    echo();
    //Must choose at least one cell
    while (options == 0) {
        printw("Please enter the number of cells you would like to set to ON: ");
        refresh();
        getnstr(number, sizeof(number));
        options = atoi(strtok(number, " "));
        clear();
    }

    //Enough space to enter "x,y x,y x,y... x,y" with maximum 211,57
    char input[options * 7 - 1];

    //Take initial dimensions
    printw("Put in the coordinates of the cells that should be ON: ");
    refresh();
    getnstr(input, sizeof(input));

    //Take first input outside loop because of strtok params
    int x = atoi(strtok(input, ","));
    int y = atoi(strtok(NULL, " "));
    if (x < width && y < height) {
        flipCell(&game[y][x]);
    }

    //Loop through rest of cells
    for (int i = 0; i < options - 2; i++) {
        x = atoi(strtok(NULL, ","));
        y = atoi(strtok(NULL, " "));

        //The final condition make sure to eliminate all duplicate cell inputs
        if (x < width && y < height && !game[x][y].s) {
            flipCell(&game[y][x]);
        }
    }

    //For end of line if string doesn't fill the line
    x = atoi(strtok(NULL, ","));
    y = atoi(strtok(NULL, "\n"));

    //The final condition make sure to eliminate all duplicate cell inputs
    if (x < width && y < height && !game[x][y].s) {
        flipCell(&game[y][x]);
    }
}

void takeDimensions(int *width, int *height, int maxW, int maxH) {
    char input[6];

    //Take initial dimensions
    printw("Please enter the size of your matrix <= (maxW, maxH): ");
    refresh();
    getnstr(input, sizeof(input));

    //Need to fix seg fault on strings entered

    *width = atoi(strtok(input, ","));
    *height = atoi(strtok(NULL, "\n"));
    clear();

    //If dimensions don't fulfil requirements, ask for re-entry
    while ((*width > maxW || *height > maxH)) {
        printw("Please enter dimensions <= (%d, %d): ", maxW, maxH);
        refresh();
        getnstr(input, sizeof(input));
        *width = atoi(strtok(input, ","));
        *height = atoi(strtok(NULL, "\n"));
        clear();
    }
}

//Forward declaration for functions below
void printMenu(cell **game, int width, int height, int *tick, bool *quit, bool *cGOL);

//Segmentation fault if grid isn't big enough
void printInitialConfigs(cell **game, int width, int height, int *tick, bool *quit, bool *cGOL) {
    char option = 'a';

    while (option != '1' && option != '2' && option != '3' &&
           option != '4' && option != '5' && option != '6'&& option != '7') {
        printw("Initial board setup options:\n\n");
        printw("\t1 - Night Sky\n");
        printw("\t2 - Glider Gun\n");
        printw("\t3 - Glider\n");
        printw("\t4 - Butterfly\n");
        printw("\t5 - Geometry\n");
        printw("\t6 - Periodic\n");
        printw("\t7 - Return to menu\n");

        printw("\nPlease choose an option between 1 and 6, or exit with 7");
        refresh();
        scanf("%c", &option);
        clear();
    }

    switch (atoi((const char *) &option)) {
        case 1:
            *tick = 20000;
            nightSky(game, width, height);
            break;
        case 2:
            *tick = 10000;
            gliderGun(game, width, height);
            break;
        case 3:
            *tick = 50000;
            glider(game, width, height);
            break;
        case 4:
            butterfly(game, width, height);
            break;
        case 5:
            *tick = 50000;
            geometry(game, width, height);
            break;
        case 6:
            *tick = 80000;
            periodic(game, width, height);
            break;
        case 7:
            printMenu(game, width, height, tick, quit, cGOL);
            break;
    }
}

bool checkDefault() {
    char answer = 'a';

    while (answer != 'y' && answer != 'n') {
        printw("Would you like to choose the game size? (y or n)");
        refresh();
        scanf("%c", &answer);
        clear();
    }

    if (answer == 'y') {
        //Ask user for sizing
        return false;
    }
    //Use default
    return true;
}

void printMenu(cell **game, int width, int height, int *tick, bool *quit, bool *cGOL) {
    char option = 'a';

    while (option != '1' && option != '2' && option != '3' && option != '4'
      && option != '5' && option != '6') {
        printw("Initial board setup options:\n\n");
        printw("\t1 - Random Configuration\n");
        printw("\t2 - Pre-made Configuration\n");
        printw("\t3 - User Input\n");
        printw("\t4 - Play Tetris\n");
        printw("\t5 - Play Sudoku\n");
        printw("\t6 - Quit\n");

        printw("\nPlease choose an option between 1 and 6");
        refresh();
        scanf("%c", &option);
        clear();
    }
    int maxW, maxH;
    switch (atoi((const char *) &option)) {
        case 1:
            randomConfig(game, width, height);
            break;
        case 2:
            printInitialConfigs(game, width, height, tick, quit, cGOL);
            break;
        case 3:
            printUserInput(game, width, height);
            break;
        case 4:
            *cGOL = FALSE;
            runTETRIS();
            break;
        case 5:
            *cGOL = FALSE;
            runSudoku();
            break;
        case 6:
            *quit = TRUE;
    }
}

void runCGOL(cell **game, int width, int height, int tick) {
    clear();
    printScreen(game, width, height);
    refresh();
    //Wait for user to click before starting
    getchar();

    while (1) {
        evolve(game, width, height);
        printScreen(game, width, height);
        refresh();
        //Change the speed of evolution with a timer for different patterns
        if (getch() == 'p') { //Hold p to pause
            while(getch() != 'r') {
                if (getch() == 's') {
                    evolve(game, width, height);
                    printScreen(game, width, height);
                    refresh();
                }
            } //Press r to resume
        } else if (getch() == 'x') { //Press x to quit run
            break;
        }
            usleep(tick);
    }
    printScreen(game, width, height);
    refresh();
}

int main(int argc, char **argv) {
    //Initialise screen
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    //Hide cursors
    curs_set(0);
    int tick = 180000;

    //Find max size of screen
    int maxW, maxH;
    getmaxyx(stdscr, maxH, maxW);
    //Leave a line at the edge
    maxW -= 1;
    int width = maxW, height = maxH;

    //Allow user to choose size of game
    if (!checkDefault()) {
        takeDimensions(&width, &height, maxW, maxH);
    }

    //Setup the grid
    cell** game = {setupGrid(width, height)};

    bool quit = FALSE;
    while(1) {
        attron(COLOR_PAIR(5));
        //Determines whether or not to run cGOL
        bool cGOL = TRUE;
        printMenu(game, width, height, &tick, &quit, &cGOL);
        //If option 6 is selected, user exits to terminal
        if (quit) {
            break;
        }

        if (cGOL) {
            //Enter non-blocked read mode
            timeout(1);
            noecho();
            runCGOL(game, width, height, tick);
            //Enter blocked read mode
            echo();
            timeout(-1);

            //Clean up
            resetGrid(game, width, height);
        }

        clear();
    }
    endwin();
}
