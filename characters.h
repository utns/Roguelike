#pragma once

#include <string>
#include <map>
#include "map.h"
#include "Point.h"
#include "ncurses.h"

#define KNIGHT_SYMBOL 'K'
#define PRINCESS_SYMBOL 'P'
#define ZOMBIE_SYMBOL 'Z'
#define DRAGON_SYMBOL 'D'
#define MEDKIT_SYMBOL '+'

class Character;
class Knight;
class Princess;
class Monster;

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
    protected:
        Point point;
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
        void collide(Actor *actor);
        void collide(Character* character);
        void collide(Knight* knight);
        void collide(Monster* monster);
    private:
        int hp_restore = 500000;
};


class Character: public Actor
{
    public:
        Character(int x, int y): Actor(x, y) {};
        int get_hp() const;
        void set_hp(int Hp);
        int get_damage() const;
        int get_max_hp() const;
    protected:
        int hp, damage, max_hp;
};

class Knight: public Character
{
    public:
        Knight(int x, int y): Character(x, y) {hp = 50000; damage = 100; max_hp = 50000;};
        virtual void move(Map &map);
        char get_symbol() const;
        void collide(Actor *actor);
        void collide(Knight *knight) {};
        void collide(Princess *princess) {};
        void collide(Monster *monster);
};

class Princess: public Character
{
    public:
        Princess(int x, int y): Character(x, y) {hp = 1; damage = 0; max_hp = 1;};
        void move(Map &map) {};
        char get_symbol() const;
        void collide(Actor *actor);
        void collide(Knight *knight);
        void collide(Princess *princess) {};
        void collide(Monster *monster) {};
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
};

class Dragon: public Monster
{
    public:
        Dragon(int x, int y): Monster(x, y) {hp = 250; damage = 50; max_hp = 250;};
        char get_symbol() const;
}; 

class Zombie: public Monster
{
    public:
        Zombie(int x, int y): Monster(x, y) {hp = 100; damage = 10; max_hp = 100;};
        char get_symbol() const;
};
