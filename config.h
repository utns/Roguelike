#pragma  once

#include <string>
#include <fstream>
#include "rapidjson/document.h"

class Config
{
    public:
        int fireball_damage, knight_hp, knight_damage, princess_hp,
            princess_damage, dragon_hp, dragon_damage, zombie_hp,
            zombie_damage, wizard_hp, wizard_damage, zombie_spawn_cooldawn,
            dragon_spawn_cooldawn;
        static Config& instance();
        ~Config();
    private:
        Config() {};
        Config(std::string file_name);
        int get_int(std::string object_name, std::string member_name);
        int get_str_num(int offset);
        rapidjson::Document config;
        std::string config_str;
        std::ifstream fin;
        bool has_object(std::string object_name);
        bool is_object(std::string object_name);
        bool has_member(std::string object_name, std::string member_name);
        bool is_int(std::string object_name, std::string member_name);
};
