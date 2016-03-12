#include <iostream>
#include <string>
#include <cstdlib>
#include "controller.h"
#include "ncurses.h"

using namespace std;

int main()
{
    initscr();
    Controller::instance().find_characters();
    Controller::instance().game_loop();
    printw("Press any button...\n");
    getch();
    endwin();
    return 0;
}