#include "Point.h"

Point operator+(const Point &left, const Point &right)
{
    return Point(left.x + right.x, left.y + right.y);
}

Point operator-(const Point &left, const Point &right)
{
    return Point(left.x - right.x, left.y - right.y);
}

Point Point::operator*(const int a) const
{
    return Point(x * a, y * a);
}

Point Point::operator/(const int a) const
{
    return Point(x / a, y / a);
}

bool operator==(const Point &left, const Point &right)
{
    return left.x == right.x && left.y == right.y;
}

bool operator!=(const Point &left, const Point &right)
{
    return left.x != right.x && left.y != right.y;
}

bool operator<(const Point &left, const Point &right)
{
    return left.x < right.x || (left.x == right.x && left.y < right.y);
}

Point operator*(const int a, const Point &point)
{
    return point * a;
}
