#include <cstdio>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>
#include "characters.h"
#include "map.h"
#include "controller.h"
#include "ncurses.h"

using namespace std;

Map::Map(string file_name)
{
    ifstream fin(file_name);
    fin >> w >> h;
    source.resize(h);
    path.resize(h);
    for (int i = 0; i < h; ++i)
    {
        source[i].resize(w);
        path[i].resize(w);
        for (int j = 0; j < w; ++j)
        {
            fin >> source[i][j];
        }
    }
}

void Map::display()
{
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            printw("%c", source[i][j]);
        }
        printw("\n");
    }
}

bool Map::is_wall(Point point) const
{
    return source[point.y][point.x] == '#';
}

bool Map::is_princess(Point point) const
{
    return source[point.y][point.x] == PRINCESS_SYMBOL;
}

bool Map::is_monster(Point point) const
{
    return source[point.y][point.x] == ZOMBIE_SYMBOL || source[point.y][point.x] == DRAGON_SYMBOL;
}

bool Map::is_empty(Point point) const
{
    return source[point.y][point.x] == '.';
}

bool Map::is_knight(Point point) const
{
    return source[point.y][point.x] == KNIGHT_SYMBOL;
}

void Map::move(Point point1, Point point2)
{
    source[point2.y][point2.x] = source[point1.y][point1.x];
    if (point1.x != point2.x || point1.y != point2.y)
    {
        source[point1.y][point1.x] = '.';
    }
}

const std::vector < std::vector <char> >& Map::get_source() const
{
    return source;
}

const std::vector < std::vector <int> >& Map::get_path() const
{
    return path;
}

void Map::find_path(Point point)
{
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            path[i][j] = 0;
        }
    }
    queue < Point > wave;
    wave.push(point);
    path[point.y][point.x] = 1;
    while (!wave.empty())
    {
        Point cur_elem = wave.front();
        for (auto &it: Controller::instance().get_directions())
        {
            Point next_elem = cur_elem + it.second;
            if (!is_wall(next_elem) && !path[next_elem.y][next_elem.x])
            {
                path[next_elem.y][next_elem.x] = path[cur_elem.y][cur_elem.x] + 1;
                wave.push(next_elem);
            }
        }
        wave.pop();
    }
}

int Map::get_width() const
{
    return w;
}

int Map::get_hight() const
{
    return h;
}

void Map::set_symbol(char symbol, Point point)
{
    source[point.y][point.x] = symbol;
}
