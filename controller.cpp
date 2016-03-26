#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include <string>
#include "map"
#include "actors.h"
#include "controller.h"
#include "ncurses.h"
#include <list>

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
                    actors.push_back(new Dragon(j, i));
                    break;
                }
                case ZOMBIE_SYMBOL:
                {
                    actors.push_back(new Zombie(j, i));
                    break;
                }
                case WIZARD_SYMBOL:
                {
                    actors.push_back(new Wizard(j, i));
                    break;
                }
                case MEDKIT_SYMBOL:
                {
                    ++medkit_count;
                    actors.push_back(new MedKit(j, i));
                    break;
                }
                case ZOMBIE_SPAWNER_SYMBOL:
                {
                    actors.push_back(new Zombie_spawner(j, i));
                    break;
                }
                case DRAGON_SPAWNER_SYMBOL:
                {
                    actors.push_back(new Dragon_spawner(j, i));
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
        actors_move();
        if (medkit_spawn_cooldown <= 0 && medkit_count < 5)
        {
            spawn_medkit();
        }
        if (princess->get_hp() <= 0)
        {
            clear();
            printw("YOU WIN!!\n");
            game_over = true;
        }
        else if (knight == nullptr || knight->get_hp() <= 0)
        {
            clear();
            printw("YOU LOSE!!!\n");
            game_over = true;
        }
        --medkit_spawn_cooldown;
    }
}

list<Actor *>::iterator Controller::get_actor_iter(Point point)
{
    for (auto actor = actors.begin(); actor != actors.end(); ++actor)
    {
        if ((*actor) != nullptr && (*actor)->get_point() == point)
        {
            return actor;
        }
    }
    return actors.end();
}

void Controller::delete_actor(Point point)
{
    auto actor = get_actor_iter(point);
    if (actor != actors.end())
    {
        delete (*actor);
        (*actor) = nullptr;
        map.set_symbol('.', point);
    }
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
    static Controller controller("map1.txt");
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
    {"w", Point(0, -1)},
    {"a", Point(-1, 0)},
    {"s", Point(0, 1)},
    {"d", Point(1, 0)},
    {"y", Point(-1, -1)},
    {"u", Point(1, -1)},
    {"b", Point(-1, 1)},
    {"n", Point(1, 1)},
};

Actor * Controller::get_actor(Point point)
{
    if (princess->get_point() == point)
    {
        return princess;
    }
    else if (knight->get_point() == point)
    {
        return knight;
    }
    else
    {
        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            if ((*actor) != nullptr && (*actor)->get_point() == point)
            {
                return (*actor);
            }
        }
    }
    return nullptr;
}

void Controller::spawn_medkit()
{
    bool medkit_spawned = false;
    srand(time(NULL));
    while (!medkit_spawned)
    {
        int x = rand() % map.get_width();
        int y = rand() % map.get_hight();
        if (map.is_empty(Point(x, y)))
        {
            actors.push_back(new MedKit(x, y));
            map.set_symbol(MEDKIT_SYMBOL, Point(x, y));
            medkit_spawn_cooldown = 3;
            ++medkit_count;
            medkit_spawned = true;
        }
    }
}

void Controller::medkit_count_dec()
{
    --medkit_count;
}

void Controller::create_fireball(Point pos, Point dir)
{
    actors.push_front(new Fireball(pos.x, pos.y, dir));
    map.set_symbol(actors.front()->get_symbol(), actors.front()->get_point());
}

void Controller::draw_actors()
{
    map.set_symbol(knight->get_symbol(), knight->get_point());
    for (auto actor = actors.begin(); actor != actors.end(); ++actor)
    {
        map.set_symbol((*actor)->get_symbol(), (*actor)->get_point());
    }
}

void Controller::push_actor(Actor* actor)
{
    actors.push_front(actor);
    map.set_symbol(actor->get_symbol(), actor->get_point());
}

void Controller::actors_move()
{
    for (auto actor = actors.begin(); actor != actors.end(); ++actor)
    {
        if ((*actor) != nullptr)
        {
            (*actor)->move(map);
        }
        else
        {
            actors.erase(actor--);
        }
    }
}
