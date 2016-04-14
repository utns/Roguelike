#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "config.h"
#include "actors.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

using namespace std;
using namespace rapidjson;

Config& Config::instance()
{
    static Config config("config.txt");
    return config;
}

Config::Config(std::string file_name)
{
    fin.open(file_name);
    if (!fin.is_open())
    {
        printf("Can't open file: \"%s\"\n", file_name.c_str());
        exit(1);
    }
    stringstream sstr;
    sstr << fin.rdbuf();
    config_str = sstr.str();
    if (config.Parse(config_str.c_str()).HasParseError())
    {
        printf("Error(string %d): %s\n", get_str_num(config.GetErrorOffset()),
            GetParseError_En(config.GetParseError()));
        exit(1);
    }
    else
    {
        fireball_damage = get_int("Fireball", "damage");
        knight_damage = get_int("Knight", "damage");
        knight_hp = get_int("Knight", "hp");
        princess_damage = get_int("Princess", "damage");
        princess_hp = get_int("Princess", "hp");
        dragon_damage = get_int("Dragon", "damage");
        dragon_hp = get_int("Dragon", "hp");
        zombie_damage = get_int("Zombie", "damage");
        zombie_hp = get_int("Zombie", "hp");
        wizard_damage = get_int("Wizard", "damage");
        wizard_hp = get_int("Wizard", "hp");
        zombie_spawn_cooldawn = get_int("Zombie_spawner", "cooldawn");
        dragon_spawn_cooldawn = get_int("Dragon_spawner", "cooldawn");
    }
}

int Config::get_int(std::string object_name, std::string member_name)
{
    if (has_object(object_name) && is_object(object_name)
        && has_member(object_name, member_name)
           && is_int(object_name, member_name))
    {
        return (config[object_name.c_str()])[member_name.c_str()].GetInt();
    }
    else
    {
        exit(1);
    }
}

int Config::get_str_num(int offset)
{
    int cur_str = 1;
    for (int i = 0; i < offset; ++i)
    {
        if (config_str[i] == '\n')
        {
            ++cur_str;
        }
    }
    return cur_str;
}

bool Config::has_object(std::string object_name)
{
    if (config.HasMember(object_name.c_str()))
    {
        return true;
    }
    else
    {
        printf("Config: Can't find object \"%s\"\n", object_name.c_str());
        return false;
    }
}

bool Config::is_object(std::string object_name)
{
    if (config[object_name.c_str()].IsObject())
    {
        return true;
    }
    else
    {
        printf("Config: \"%s\" isn't object\n", object_name.c_str());
        return false;
    }
}

bool Config::has_member(std::string object_name, std::string member_name)
{
    if (config[object_name.c_str()].HasMember(member_name.c_str()))
    {
        return true;
    }
    else
    {
        printf("Config: Object \"%s\" hasn't member \"%s\"\n", object_name.c_str(), member_name.c_str());
        return false;
    }
}

bool Config::is_int(std::string object_name, std::string member_name)
{
    if (config[object_name.c_str()][member_name.c_str()].IsInt())
    {
        return true;
    }
    else
    {
        printf("Config: In object \"%s\" member \"%s\" isn't int\n", object_name.c_str(), member_name.c_str());
        return false;
    }
}

Config::~Config()
{
    fin.close();
}
