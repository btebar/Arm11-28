#include <stdlib.h>
#include "game.h"
#include "grid.h"
#include "cell.h"
#include <ncurses.h>
#include <zconf.h>

//Decide defaults
#define DEFAULT_WIDTH 50
#define DEFAULT_HEIGHT 100

void printScreen(cell **board, int width, int height){
    move(0, 0);
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            printw(symbol(board[i][j]));
        }
        printw("\n");
    }
}

int main(int argc, char **argv) {
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;

    if (argc == 4) {
        int width = atoi(argv[2]);
        int height = atoi(argv[3]);
    }
    initscr();

    screen game = {setupGrid(width, height)};
    flipCell(&game.board[10][10]);
    flipCell(&game.board[10][11]);
   // flipCell(&game.board[11][10]);
  //  flipCell(&game.board[11][11]);

    printScreen(game.board, width, height);
    refresh();
    getchar();

    while(1) {
        evolve(game, width, height);
        printScreen(game.board, width, height);
        refresh();
        getchar();
    }
}
