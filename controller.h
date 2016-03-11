#pragma once

#include "map.h"
#include "characters.h"
#include <string>
#include <vector>

using namespace std;

class Controller
{   
    public:
        Map &get_map();
        void find_characters();
        void game_loop();
        void monsters_move();
        int find_monster(int x, int y);
        void delete_monster(int x, int y);
        void push_log(string s);
        void print_log();
        static Controller& instance();
        Knight &get_knight();
        Princess &get_princess();
        Monster &get_monster(int x, int y);
    private:
        Controller(string file_name): map(file_name) {};
        Map map;
        Knight *knight;
        Princess *princess;
        vector <Monster*> monsters;
        vector <string> hit_log;
};