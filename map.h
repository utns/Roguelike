#pragma once

#include <string>
#include <vector>
#include "Point.h"

class Map
{
    public:
        Map(std::string file_name);
        void display();
        bool is_wall(Point point) const;
        bool is_princess(Point point) const;
        bool is_monster(Point point) const;
        bool is_empty(Point point) const;
        bool is_knight(Point point) const;
        void move(Point point1, Point point2);
        const std::vector < std::vector <char> > &get_source() const;
        const std::vector < std::vector <int> > &get_path() const;
        void find_path(Point point);
        int get_width() const;
        int get_hight() const;
        void set_symbol(char symbol, Point point);
    private:
        std::vector < std::vector <char> > source;
        std::vector < std::vector <int> > path;
        int w, h;
};

