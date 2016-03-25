#pragma once

#include "map.h"
#include "characters.h"
#include <string>
#include <vector>
#include <list>
#include "Point.h"

class Controller
{   
    public:
        Map &get_map();
        void find_characters();
        void game_loop();
        std::list<Actor *>::iterator get_actor_iter(Point point);
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
        void create_fireball(Point pos, Point dir);
        void draw_actors();
        void push_actor(Actor* actor);
        void actors_move();
    private:
        Controller(std::string file_name): map(file_name) {};
        Map map;
        Knight *knight;
        Princess *princess;
        std::list <Actor*> actors;
        std::vector <std::string> hit_log;
        static std::map <std::string, Point > directions;
        int medkit_count = 0;
        int medkit_spawn_cooldown = 3;
};