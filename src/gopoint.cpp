#include "gopoint.h"

GoPoint::GoPoint(unsigned short x, unsigned short y)
{
    _x = x;
    _y = y;
}

GoPoint::~GoPoint()
{
}

inline static bool isValid(unsigned short x, unsigned short y, unsigned short size)
{
    return x < size && y < size;
}

unsigned short GoPoint::x() const
{
    return _x;
}

unsigned short GoPoint::y() const
{
    return _y;
}

bool GoPoint::operator==(const GoPoint &other) const
{
    return (this->_x == other.x() && this->_y == other.y());
}

bool GoPoint::operator!=(const GoPoint &other) const
{
    return !(*this == other);
}
