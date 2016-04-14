#pragma once

#include <string>
#include <map>
#include "map.h"
#include "Point.h"
#include "ncurses.h"
#include "config.h"

#define KNIGHT_SYMBOL 'K'
#define PRINCESS_SYMBOL 'P'
#define ZOMBIE_SYMBOL 'Z'
#define DRAGON_SYMBOL 'D'
#define MEDKIT_SYMBOL '+'
#define WIZARD_SYMBOL 'W'
#define ZOMBIE_SPAWNER_SYMBOL '!'
#define DRAGON_SPAWNER_SYMBOL '$'

class Character;
class Knight;
class Princess;
class Monster;
class Zombie;
class Dragon;
class Wizard;
class MedKit;
class Fireball;

class Actor
{
    public:
        Actor(int x, int y): point(x, y) {};
        virtual ~Actor() {};
        Point get_point() const;
        void set_coordinate(Point new_point);
        virtual void move(Map &map) = 0;
        virtual char get_symbol() const = 0;
        virtual void collide(Actor* actor) = 0;
        virtual void collide(Character* character) {};
        virtual void collide(Knight* knight) {};
        virtual void collide(Princess* princess) {};
        virtual void collide(Monster* monster) {};
        virtual void collide(MedKit* medkit) {};
        virtual void collide(Fireball* fireball) {};
        void set_point(Point new_point);
    protected:
        Point point;
};

class Spawner: public Actor
{
    public:
        Spawner(int x, int y): Actor(x, y) {};
    protected:
        Actor* actor = nullptr;
        int cur_cooldawn = 0;
        void spawn_monster(int spawn_cooldawn);
};

class Zombie_spawner: public Spawner
{
    public:
        Zombie_spawner(int x, int y): Spawner(x, y) {};
        void move(Map &map);
        char get_symbol() const;
        void collide(Actor* actor);
    private:
        static int spawn_cooldawn;
};

class Dragon_spawner: public Spawner
{
    public:
        Dragon_spawner(int x, int y): Spawner(x, y) {};
        void move(Map &map);
        char get_symbol() const;
        void collide(Actor* actor);
    private:
        static int spawn_cooldawn;
};

class Modificator: public Actor
{
    public:
        Modificator(int x, int y): Actor(x, y) {};
};

class MedKit: public Modificator
{
    public:
        MedKit(int x, int y): Modificator(x, y) {};
        void move(Map &map) {};
        char get_symbol() const;
        void collide(Actor* actor);
        void collide(Character* character);
        void collide(Knight* knight);
        void collide(Monster* monster);
        void collide(Fireball* fireball);
    private:
        int hp_bonus = 100;
};

class Fireball: public Actor
{
    public:
        Fireball(int x, int y, Point dir): Actor(x, y), dir(dir)  {damage = Config::instance().fireball_damage;};
        void move(Map &map);
        char get_symbol() const;
        void collide(Actor* actor);
        void collide(Fireball* fireball);
        void collide(MedKit* medkit);
        void collide(Character* character);
        int get_damage();
    private:
        int damage;
        Point dir;
        static std::map <Point, char> symbols;
};

class Character: public Actor
{
    public:
        Character(int x, int y): Actor(x, y) {};
        int get_hp() const;
        void set_hp(int Hp);
        int get_damage() const;
        int get_max_hp() const;
        void collide(Fireball* fireball);
    protected:
        int hp, damage, max_hp;
};

class Knight: public Character
{
    public:
        Knight(int x, int y): Character(x, y) {hp = max_hp = Config::instance().knight_hp; damage = Config::instance().knight_damage;};
        virtual void move(Map &map);
        char get_symbol() const;
        void collide(Actor *actor);
        void collide(Monster *monster);
};

class Princess: public Character
{
    public:
        Princess(int x, int y): Character(x, y) {hp = max_hp = Config::instance().princess_hp; damage = Config::instance().princess_damage;};
        void move(Map &map) {};
        char get_symbol() const;
        void collide(Actor *actor);
        void collide(Knight *knight);
        void collide(Monster *monster) {};
        void collide(Fireball *fireball);
};

class Monster: public Character
{
    public:
        Monster(int x, int y): Character(x, y) {};
        void move(Map &map);
        void collide(Actor *actor);
        void collide(Knight *knight);
        void collide(Princess *princess) {};
        void collide(Monster *monster) {};
    protected:
        Point get_next_point();
};

class Dragon: public Monster
{
    public:
        Dragon(int x, int y): Monster(x, y) {hp = max_hp = Config::instance().dragon_hp; damage = Config::instance().dragon_damage;};
        char get_symbol() const;
};

class Zombie: public Monster
{
    public:
        Zombie(int x, int y): Monster(x, y) {hp = max_hp = Config::instance().zombie_hp; damage = Config::instance().zombie_damage;};
        char get_symbol() const;
};

class Wizard: public Monster
{
    public:
        Wizard(int x, int y): Monster(x, y) {hp = max_hp = Config::instance().wizard_hp; damage = Config::instance().wizard_damage;}
        void move(Map &map);
        char get_symbol() const;
    protected:
        int cur_colldown = 0;
        static int spell_cooldown;
        Point get_dir();
};