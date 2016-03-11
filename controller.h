#pragma once

#include "map.h"
#include "characters.h"
#include <string>
#include <vector>

class Controller
{   
    public:
        Map &get_map();
        void find_characters();
        void game_loop();
        void monsters_move();
        int find_monster(int x, int y);
        void delete_monster(int x, int y);
        void push_log(std::string s);
        void print_log();
        static Controller& instance();
        Knight &get_knight();
        Princess &get_princess();
        Monster &get_monster(int x, int y);
        const static std::map <std::string, std::pair <int, int> > &get_directions();
    private:
        Controller(std::string file_name): map(file_name) {};
        Map map;
        Knight *knight;
        Princess *princess;
        std::vector <Monster*> monsters;
        std::vector <std::string> hit_log;
        static std::map <std::string, std::pair <int, int> > directions;
};