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

void Character::collide(Fireball *fireball)
{
    Controller::instance().push_log(string(1, fireball->get_symbol()) + " attack " + this->get_symbol() + " " +  to_string(fireball->get_damage()));
    hp -= fireball->get_damage();
    Controller::instance().delete_actor(fireball->get_point());
    if (hp <= 0)
    {
        Controller::instance().delete_actor(point);
    }
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

void Princess::collide(Fireball *fireball)
{
    Controller::instance().get_knight().set_hp(-1);
}

void Monster::move(Map &map)
{
    Point new_point = get_next_point();
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

Point Monster::get_next_point()
{
    Map& map = Controller::instance().get_map();
    vector < vector <int> > path = map.get_path();
    int min_path = INT_MAX;
    Point new_point = point;
    for (auto &it: Controller::instance().get_directions())
    {
        Point t_point = point + (it.second);
        if (path[t_point.y][t_point.x] < min_path && ((!map.is_monster(t_point) && !map.is_wall(t_point) && !map.is_princess(t_point)) || map.is_knight(t_point)))
        {
            min_path = path[t_point.y][t_point.x];
            new_point = t_point;
        }
    }
    return new_point;
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
    Controller::instance().push_log( "You take medkit (" + to_string(knight->get_hp()) + "hp)");
}

void MedKit::collide(Monster *monster)
{
    collide(static_cast<Character*>(monster));
    Controller::instance().push_log(string(1, monster->get_symbol()) + " take medkit (" + to_string(monster->get_hp()) + "hp)");
}

void MedKit::collide(Character *character)
{
    character->set_hp((character->get_hp() + hp_bonus));
    if (character->get_hp() > character->get_max_hp())
    {
        character->set_hp(character->get_max_hp());
    }
    Controller::instance().delete_actor(point);
    Controller::instance().medkit_count_dec();
}

void MedKit::collide(Fireball *fireball)
{
    Controller::instance().delete_actor(point);
    Controller::instance().delete_actor(fireball->get_point());
}

int Wizard::spell_cooldown = 2;

char Wizard::get_symbol() const
{
    return WIZARD_SYMBOL;
}

void Wizard::move(Map &map)
{
    --cur_colldown;
    Point dir = get_dir();
    bool find_knight = false;
    Point next_point = point + dir;
    while (!find_knight)
    {
        next_point = next_point + dir;
        if (map.is_knight(next_point))
        {
            find_knight = true;
            if (cur_colldown <= 0)
            {
                cur_colldown = spell_cooldown;
                Controller::instance().create_fireball(point + dir, dir);
            }
        }
        else if (map.is_wall(next_point))
        {
            break;
        }
    }
    if (!find_knight)
    {
        Monster::move(map);
    }
}

Point Wizard::get_dir()
{
    Map& map = Controller::instance().get_map();
    vector < vector <int> > path = map.get_path();
    int min_path = INT_MAX;
    Point new_point = point;
    vector <Point> directions = {Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)};
    for (auto dir: directions)
    {
        Point t_point = point + dir;
        if (path[t_point.y][t_point.x] < min_path && ((!map.is_monster(t_point) && !map.is_wall(t_point) && !map.is_princess(t_point)) || map.is_knight(t_point)))
        {
            min_path = path[t_point.y][t_point.x];
            new_point = t_point;
        }
    }
    return new_point - point;
}


char Fireball::get_symbol() const
{
    return symbols[dir];
}

map <Point, char> Fireball::symbols =
{
    {Point(0, -1), '^'},
    {Point(-1, 0), '<'},
    {Point(0, 1), 'v'},
    {Point(1, 0), '>'},
};

void Fireball::collide(Actor *actor)
{
    actor->collide(this);
}

void Fireball::move(Map &map)
{
    Point new_point = point + dir;
    Actor *actor = Controller::instance().get_actor(new_point);
    if (actor != nullptr)
    {
        collide(actor);
    }
    if (map.is_wall(new_point))
    {
        Controller::instance().delete_actor(point);
    }
    else if (Controller::instance().get_actor(new_point) == nullptr)
    {
        set_coordinate(new_point);
    }
}

void Fireball::collide(Fireball *fireball)
{
    Controller::instance().delete_actor(fireball->get_point());
    Controller::instance().delete_actor(point);
}

void Fireball::collide(MedKit *medkit)
{
    medkit->collide(this);
}

int Fireball::get_damage()
{
    return damage;
}

void Fireball::collide(Character *character)
{
    character->collide(this);
}


char Zombie_spawner::get_symbol() const
{
    return ZOMBIE_SPAWNER_SYMBOL;
}

void Zombie_spawner::move(Map &map)
{
    --spawn_cooldawn;
    if (spawn_cooldawn <= 0)
    {
        for (auto &it: Controller::instance().get_directions())
        {
            Point new_point = point + it.second;
            if (map.is_empty(new_point))
            {
                Actor* zombie = new Zombie(new_point.x, new_point.y);
                Controller::instance().push_actor(zombie);
                spawn_cooldawn = 7;
                break;
            }
        }
    }
}

void Zombie_spawner::collide(Actor *actor)
{
    actor->collide(this);
}


char Dragon_spawner::get_symbol() const
{
    return DRAGON_SPAWNER_SYMBOL;
}

void Dragon_spawner::collide(Actor *actor)
{
    actor->collide(this);
}

void Dragon_spawner::move(Map &map)
{
    --spawn_cooldawn;
    if (spawn_cooldawn <= 0)
    {
        for (auto &it: Controller::instance().get_directions())
        {
            Point new_point = point + it.second;
            if (map.is_empty(new_point))
            {
                Actor* zombie = new Dragon(new_point.x, new_point.y);
                Controller::instance().push_actor(zombie);
                spawn_cooldawn = 15;
                break;
            }
        }
    }
}

