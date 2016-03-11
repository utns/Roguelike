#include <climits>
#include <iostream>
#include <map>
#include <string>
#include "map.h"
#include "characters.h"
#include "controller.h"

using namespace std;


map <string, pair <int, int> > Character::directions = 
{
    {"a", std::make_pair(-1, 0)},
    {"w", std::make_pair(0, -1)},
    {"d", std::make_pair(1, 0)},
    {"s", std::make_pair(0, 1)},
};  

int Character::get_damage() const
{
	return damage;
}

int Character::get_hp() const
{
	return hp;
}

void Character::set_hp(int Hp)
{
	hp = Hp;
}

int Character::get_x() const
{
	return x;
}

int Character::get_y() const
{
	return y;
}

void Character::set_coordinate(int X, int Y, Map &map)
{
    map.move(x, y, X, Y);
    x = X;
    y = Y; }

void Knight::move(Map &map)
{
	string command; 
	cin >> command;
	auto dir = directions.find(command);
	if (dir == directions.end())
	{
		return;
	}
    int new_x = x + (dir->second).first;
    int new_y = y + (dir->second).second;
    if (!map.is_wall(new_x, new_y))
    {
        if (map.is_princess(new_x, new_y))
        {
            Controller::instance().get_princess().set_hp(-1);
        }
        else if (map.is_monster(new_x, new_y))
        {
        	Monster &monster = Controller::instance().get_monster(new_x, new_y);
            monster.set_hp(monster.get_hp() - damage);  
            Controller::instance().push_log("You attack " + string(1, monster.get_symbol()) + " " + to_string(get_damage())
                + " (" + to_string(monster.get_hp())+ " hp)");
            if (monster.get_hp() <= 0)
            {
                Controller::instance().delete_monster(new_x, new_y);
                set_coordinate(new_x, new_y, map);
            }
        } 
        else 
        {
            set_coordinate(new_x, new_y, map);
        }
    }
}

char Knight::get_symbol() const
{
	return KNIGHT_SYMBOL;
}

char Princess::get_symbol() const
{
	return PRINCESS_SYMBOL;
}

void Monster::move(Map &map)
{
	vector < vector <int> > path = map.get_path();
	int min_path = INT_MAX;
    int new_x, new_y;
	for (auto &it: directions)
	{
		int t_x = x + (it.second).first;
		int t_y = y + (it.second).second;
		if (path[t_y][t_x] < min_path && (map.is_empty(t_x, t_y) || map.is_knight(t_x, t_y)))
		{
			min_path =path[t_y][t_x];
		    new_x = t_x;
		    new_y = t_y;
		}
	}
    if (map.is_knight(new_x, new_y)) 
    {
        Controller::instance().push_log(string(1, get_symbol()) + " attack you " + to_string(damage));
        Controller::instance().get_knight().set_hp(Controller::instance().get_knight().get_hp() - damage);
    }
    else 
    {
        set_coordinate(new_x, new_y, map);
    }
}

char Dragon::get_symbol() const
{
	return DRAGON_SYMBOL;
}

char Zombie::get_symbol() const
{
	return ZOMBIE_SYMBOL;
}
