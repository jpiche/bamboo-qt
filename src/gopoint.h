#ifndef GOPOINT_H
#define GOPOINT_H

#include <vector>

class GoPoint
{
public:
    GoPoint(unsigned short x, unsigned short y);
    ~GoPoint();
    unsigned short x() const;
    unsigned short y() const;
    bool operator!=(const GoPoint &other) const;
    bool operator==(const GoPoint &other) const;

    static bool isValid(unsigned short x, unsigned short y, unsigned short size);

private:
    unsigned short _x;
    unsigned short _y;
};

typedef std::vector<GoPoint> GOPOINTS;

#endif
