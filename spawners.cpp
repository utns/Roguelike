#include "controller.h"
#include "characters.h"
#include "Point.h"

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
