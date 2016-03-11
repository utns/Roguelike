#pragma once

#include <string>
#include <vector>

class Map
{
    public:
        Map(std::string file_name);
        void display();
        bool is_wall(int x, int y) const;
        bool is_princess(int x, int y) const;
        bool is_monster(int x, int y) const;
        bool is_empty(int x, int y) const;
        bool is_knight(int x, int y) const;
        void move(int x1, int y1, int x2, int y2);
        const std::vector < std::vector <char> > &get_source() const;
        const std::vector < std::vector <int> > &get_path() const;
        void find_path(int x, int y);
        int get_width() const;
        int get_hight() const;
    private:
        std::vector < std::vector <char> > source;
        std::vector < std::vector <int> > path;
        int w, h;
};

