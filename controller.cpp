#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string>
#include "map"
#include "characters.h"
#include "controller.h"
#include "ncurses.h"

using namespace std;

Map &Controller::get_map()
{
    return map;
}

void Controller::find_characters()
{
    vector < vector <char> > source = map.get_source(); 
    for (int i = 0; i < source.size(); ++i)
    {
        for (int j = 0; j < source[i].size(); ++j)
        {
            switch (source[i][j])
            {
                case KNIGHT_SYMBOL:
                {
                    knight = new Knight(j, i);
                    break;
                } 
                case PRINCESS_SYMBOL:
                {
                    princess = new Princess(j, i);
                    break;
                }
                case DRAGON_SYMBOL:
                {
                    monsters.push_back(new Dragon(j, i));
                    break;
                }
                case ZOMBIE_SYMBOL:
                {
                    monsters.push_back(new Zombie(j, i));
                    break;
                }
            }
        }
    }
}

void Controller::game_loop()
{ 
    bool game_over = false;
    while(!game_over)
    {
        clear();
        map.display();
        print_log();
        hit_log.clear();
        printw("HP: %d\n", knight->get_hp());
        knight->move(map);
        map.find_path(knight->get_point());
        monsters_move();
        if (princess->get_hp() <= 0)
        {
            clear();
            printw("YOU WIN!!\n");
            game_over = true;
        }
        else if (knight->get_hp() <= 0)
        {
            clear();
            printw("YOU LOSE!!!\n");
            game_over = true;
        }
    }
}

void Controller::monsters_move()
{
    for (int i = 0; i < monsters.size(); ++i)
    {
        monsters[i]->move(map);
    }
}

int Controller::find_monster(Point point)
{
    for (int i = 0; i < monsters.size(); ++i)
    {
        if (monsters[i]->get_point() == point)
        {
            return i;
        }
    }
}    

void Controller::delete_monster(Point point)
{
    int i = find_monster(point);
    delete monsters[i];
    monsters.erase(monsters.begin() + i);
}

void Controller::print_log()
{
    for (int i = 0; i < hit_log.size(); ++i)
    {
        printw("%s\n", hit_log[i].c_str()); 
    }
}

Controller& Controller::instance()
{
    static Controller controller("map.txt");
    return controller;
}

Knight& Controller::get_knight()
{
    return *knight;
}

Princess& Controller::get_princess()
{
    return *princess;
}

Monster & Controller::get_monster(Point point)
{
    return *monsters[find_monster(point)];
}

void Controller::push_log(string s)
{
    hit_log.push_back(s);
}

const std::map <std::string, Point > &Controller::get_directions()
{
    return directions;
}

map <string, Point > Controller::directions =
{
    {"k", Point(0, -1)},
    {"h", Point(-1, 0)},
    {"j", Point(0, 1)},
    {"l", Point(1, 0)},
    {"y", Point(-1, -1)},
    {"u", Point(1, -1)},
    {"b", Point(-1, 1)},
    {"n", Point(1, 1)},
};  

