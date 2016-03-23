#pragma once

class Point
{
    public:
        Point() {};
        Point(int x = 0, int y = 0): x(x), y(y) {};
        friend Point operator+(const Point &left, const Point &right);
        friend Point operator-(const Point &left, const Point &right);
        friend bool operator==(const Point &left, const Point &right);
        friend bool operator!=(const Point &left, const Point &right);
        friend bool operator<(const Point &left, const Point &right);
        Point operator*(const int a) const;
        friend Point operator*(const int a, const Point &point);
        Point operator/(const int a) const;
        int x = 0;
        int y = 0;
};
