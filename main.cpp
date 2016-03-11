#include <iostream>
#include <string>
#include <cstdlib>
#include "controller.h"

using namespace std;

int main()
{
    Controller::instance().find_characters();
    Controller::instance().game_loop();
    return 0;
}