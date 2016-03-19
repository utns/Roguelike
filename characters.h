#pragma once

#include <string>
#include <map>
#include "map.h"
#include "Point.h"

#define KNIGHT_SYMBOL 'K'
#define PRINCESS_SYMBOL 'P'
#define ZOMBIE_SYMBOL 'Z'
#define DRAGON_SYMBOL 'D'

class Character
{
    public:
        Character(int x, int y): point(x, y) {};
        virtual void move(Map &map) = 0;
        virtual char get_symbol() const = 0;
        int get_hp() const;
        void set_hp(int Hp);
        int get_damage() const;
        Point get_point() const;
        void set_coordinate(Point new_point, Map &map);
        virtual ~Character() {};
    protected:
        Point point;
        int hp, damage;
};

class Knight: public Character
{
    public:
        Knight(int x, int y): Character(x, y) {hp = 50000; damage = 100;};
        virtual void move(Map &map);
        char get_symbol() const;
};

class Princess: public Character
{
    public:
        Princess(int x, int y): Character(x, y) {hp = 1; damage = 0;};
        void move(Map &map) {};
        char get_symbol() const;
};

class Monster: public Character 
{
    public:
        Monster(int x, int y): Character(x, y) {};
        void move(Map &map);
};

class Dragon: public Monster
{
    public:
        Dragon(int x, int y): Monster(x, y) {hp = 250; damage = 50;};
        char get_symbol() const;
}; 

class Zombie: public Monster
{
    public:
        Zombie(int x, int y): Monster(x, y) {hp = 100; damage = 10;};
        char get_symbol() const;
};
