#include "controller.h"
#include "actors.h"
#include "Point.h"

void Spawner::spawn_monster(int spawn_cooldawn)
{
    --cur_cooldawn;
    if (cur_cooldawn <= 0)
    {
        for (auto &it: Controller::instance().get_directions())
        {
            Point new_point = point + it.second;
            if (Controller::instance().get_map().is_empty(new_point))
            {
                actor->set_point(new_point);
                Controller::instance().push_actor(actor);
                actor = nullptr;
                cur_cooldawn = spawn_cooldawn;
                break;
            }
        }
    }
}

int Zombie_spawner::spawn_cooldawn = 7;

char Zombie_spawner::get_symbol() const
{
    return ZOMBIE_SPAWNER_SYMBOL;
}

void Zombie_spawner::move(Map &map)
{
    if (actor == nullptr)
    {
        actor = new Zombie(-1, -1);
    }
    spawn_monster(spawn_cooldawn);
}

void Zombie_spawner::collide(Actor *actor)
{
    actor->collide(this);
}

int Dragon_spawner::spawn_cooldawn = 15;

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
    if (actor == nullptr)
    {
        actor = new Dragon(-1, -1);
    }
    spawn_monster(spawn_cooldawn);
}
