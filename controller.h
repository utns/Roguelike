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
        int get_actor_num(Point point);
        void delete_actor(Point point);
        void push_log(std::string s);
        void print_log();
        static Controller& instance();
        Knight &get_knight();
        Princess &get_princess();
        Actor *get_actor(Point point);
        const static std::map <std::string, Point > &get_directions();
        void spawn_medkit();
        void medkit_count_dec();
    private:
        Controller(std::string file_name): map(file_name) {};
        Map map;
        Knight *knight;
        Princess *princess;
        std::vector <Actor*> actors;
        std::vector <std::string> hit_log;
        static std::map <std::string, Point > directions;
        int medkit_count = 0;
        int medkit_spawn_cooldown = 3;
};