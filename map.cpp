#include <cstdio>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>
#include "characters.h"
#include "map.h"

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
            cout << source[i][j];
        }
        cout << endl;
    }
}

bool Map::is_wall(int x, int y) const
{
    if (source[y][x] == '#')
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Map::is_princess(int x, int y) const
{
    if (source[y][x] == PRINCESS_SYMBOL)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Map::is_monster(int x, int y) const
{
    if (source[y][x] == ZOMBIE_SYMBOL || source[y][x] == DRAGON_SYMBOL)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Map::is_empty(int x, int y) const
{
    if (source[y][x] == '.')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Map::is_knight(int x, int y) const
{
    if (source[y][x] == KNIGHT_SYMBOL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Map::move(int x1, int y1, int x2, int y2)
{
    source[y2][x2] = source[y1][x1];
    if (x1 != x2 || y1 != y2)
    {
        source[y1][x1] = '.';
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

void Map::find_path(int x, int y)
{
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            path[i][j] = 0;
        }
    }
    int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; 
    queue <pair <int, int> > wave;
    wave.push(make_pair(x, y));
    path[y][x] = 1;
    while (!wave.empty())
    {
        pair <int, int> cur_elem = wave.front();
        for (int i = 0; i < 4; ++i)
        {
            pair <int, int> next_elem = make_pair(cur_elem.first + dir[i][0], cur_elem.second + dir[i][1]); 
            if (!is_wall(next_elem.first, next_elem.second) && !path[next_elem.second][next_elem.first])
            {
                path[next_elem.second][next_elem.first] = path[cur_elem.second][cur_elem.first] + 1;
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