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

void collide(Actor *left, Actor *right)
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

int Character::get_max_hp() const
{
    return max_hp;
}


Point Actor::get_point() const
{
	return point;
}

void Actor::set_coordinate(Point new_point)
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
        Actor *actor = Controller::instance().get_actor(new_point);
        if (actor != nullptr)
        {
			this->collide(actor);
        }
        if (Controller::instance().get_actor(new_point) == nullptr)
        {
            set_coordinate(new_point);
        }
    }
}

char Knight::get_symbol() const
{
	return KNIGHT_SYMBOL;
}

void Knight::collide(Actor *actor)
{
    actor->collide(this);
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

void Princess::collide(Actor *actor)
{
    actor->collide(this);
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
		if (path[t_point.y][t_point.x] < min_path && ((!map.is_monster(t_point) && !map.is_wall(t_point) && !map.is_princess(t_point))
                                                      || map.is_knight(t_point)))
		{
			min_path = path[t_point.y][t_point.x];
		    new_point = t_point;
		}
	}
    Actor *actor = Controller::instance().get_actor(new_point);
    if (actor != nullptr)
    {
        collide(actor);
    }
    if (Controller::instance().get_actor(new_point) == nullptr)
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
        Controller::instance().delete_actor(point);
    }
}

void Monster::collide(Actor *actor)
{
    actor->collide(this);
}


char Dragon::get_symbol() const
{
	return DRAGON_SYMBOL;
}

char Zombie::get_symbol() const
{
	return ZOMBIE_SYMBOL;
}

char MedKit::get_symbol() const
{
    return MEDKIT_SYMBOL;
}

void MedKit::collide(Actor *actor)
{
    actor->collide(this);
}

void MedKit::collide(Knight *knight)
{
    collide(static_cast<Character*>(knight));
}

void MedKit::collide(Monster *monster)
{
    Controller::instance().push_log("Monster take medkit " + to_string(monster->get_point().x) + " " + to_string(monster->get_point().y) +
        monster->get_symbol());
    collide(static_cast<Character*>(monster));
}

void MedKit::collide(Character *character)
{
    character->set_hp((character->get_hp() + hp_restore));
    if (character->get_hp() > character->get_max_hp())
    {
        character->set_hp(character->get_max_hp());
    }
    Controller::instance().delete_actor(point);
    Controller::instance().medkit_count_dec();
}


