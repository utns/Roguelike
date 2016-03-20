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

void collide(Character *left, Character *right)
{
    left->collide(right);
}
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

void Character::set_coordinate(Point new_point)
{
    Controller::instance().get_map().move(point, new_point);
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
        Character *character = Controller::instance().find_character(new_point);
        if (character != nullptr)
        {
			this->collide(character);
        }
        if (Controller::instance().find_character(new_point) == nullptr)
        {
            set_coordinate(new_point);
        }
    }
}

char Knight::get_symbol() const
{
	return KNIGHT_SYMBOL;
}

void Knight::collide(Character *character)
{
    character->collide(this);
}

void Knight::collide(Monster *monster)
{
    Controller::instance().push_log(string(1, monster->get_symbol()) + " attack you " + to_string(monster->get_damage()));
    hp -= monster->get_damage();
}

char Princess::get_symbol() const
{
	return PRINCESS_SYMBOL;
}

void Princess::collide(Character *character)
{
    character->collide(this);
}

void Princess::collide(Knight *knight)
{
    hp = -1;
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
    Character *character = Controller::instance().find_character(new_point);
    if (character != nullptr)
    {
        collide(character);
    }
    else
    {
        set_coordinate(new_point);
    }
}

void Monster::collide(Knight *knight)
{
    hp -= knight->get_damage();
    Controller::instance().push_log("You attack " + string(1, get_symbol()) + " "
                                    + to_string(get_damage()) + " (" + to_string(hp)+ " hp)");
    if (hp <= 0)
    {
        Controller::instance().delete_monster(point);
    }
}

void Monster::collide(Character *character)
{
    character->collide(this);
}


char Dragon::get_symbol() const
{
	return DRAGON_SYMBOL;
}

char Zombie::get_symbol() const
{
	return ZOMBIE_SYMBOL;
}
