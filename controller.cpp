#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "map"
#include "characters.h"
#include "controller.h"

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
        system("clear");
        map.display();
        print_log();
        hit_log.clear();
        cout << "HP: " << knight->get_hp() << endl;
        knight->move(map);
        map.find_path(knight->get_x(), knight->get_y());
        monsters_move();
        if (princess->get_hp() <= 0)
        {
            system("clear");
            cout << "YOU WIN!!!" << endl;
            game_over = true;
        }
        else if (knight->get_hp() <= 0)
        {
            system("clear");
            cout << "YOU LOSE!!!" << endl;
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

int Controller::find_monster(int x, int y)
{
    for (int i = 0; i < monsters.size(); ++i)
    {
        if (monsters[i]->get_x() == x && monsters[i]->get_y() == y)
        {
            return i;
        }
    }
}    

void Controller::delete_monster(int x, int y)
{
    int i = find_monster(x, y);
    delete monsters[i];
    monsters.erase(monsters.begin() + i);
}

void Controller::print_log()
{
    for (int i = 0; i < hit_log.size(); ++i)
    {
        cout << hit_log[i] << endl;
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

Monster& Controller::get_monster(int x, int y)
{
    return *monsters[find_monster(x, y)];
}

void Controller::push_log(string s)
{
    hit_log.push_back(s);
}