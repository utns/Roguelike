#pragma once

#include "map.h"
#include "characters.h"
#include <string>
#include <vector>
#include "Point.h"

class Controller
{   
    public:
        Map &get_map();
        void find_characters();
        void game_loop();
        void monsters_move();
        int find_monster(Point point);
        void delete_monster(Point point);
        void push_log(std::string s);
        void print_log();
        static Controller& instance();
        Knight &get_knight();
        Princess &get_princess();
        Monster &get_monster(Point point);
        const static std::map <std::string, Point > &get_directions();
    private:
        Controller(std::string file_name): map(file_name) {};
        Map map;
        Knight *knight;
        Princess *princess;
        std::vector <Monster*> monsters;
        std::vector <std::string> hit_log;
        static std::map <std::string, Point > directions;
};