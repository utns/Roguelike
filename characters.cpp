#include <climits>
#include <iostream>
#include <map>
#include <string>
#include "map.h"
#include "characters.h"
#include "controller.h"
#include "ncurses.h"
#include "Point.h"

using namespace std;

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

Point Character::get_point() const
{
	return point;
}

void Character::set_coordinate(Point new_point, Map &map)
{
    map.move(point, new_point);
    point = new_point;
}

void Knight::move(Map &map)
{
    string command;
    command = string(1, getch());
	auto dir = Controller::instance().get_directions().find(command);
	if (dir == Controller::instance().get_directions().end())
	{
		return;
	}
    Point new_point = point + dir->second;
    if (!map.is_wall(new_point))
    {
        if (map.is_princess(new_point))
        {
            Controller::instance().get_princess().set_hp(-1);
        }
        else if (map.is_monster(new_point))
        {
        	Monster &monster = Controller::instance().get_monster(new_point);
            monster.set_hp(monster.get_hp() - damage);  
            Controller::instance().push_log("You attack " + string(1, monster.get_symbol()) + " " 
                + to_string(get_damage()) + " (" + to_string(monster.get_hp())+ " hp)");
            if (monster.get_hp() <= 0)
            {
                Controller::instance().delete_monster(new_point);
                set_coordinate(new_point, map);
            }
        } 
        else 
        {
            set_coordinate(new_point, map);
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
    Point new_point = point;
	for (auto &it: Controller::instance().get_directions())
	{
		Point t_point = point + (it.second);
		if (path[t_point.y][t_point.x] < min_path && (map.is_empty(t_point) || map.is_knight(t_point)))
		{
			min_path = path[t_point.y][t_point.x];
		    new_point = t_point;
		}
	}
    if (map.is_knight(new_point))
    {
        Controller::instance().push_log(string(1, get_symbol()) + " attack you " + to_string(damage));
        Controller::instance().get_knight().set_hp(Controller::instance().get_knight().get_hp() - damage);
    }
    else 
    {
        set_coordinate(new_point, map);
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
