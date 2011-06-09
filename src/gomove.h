#ifndef GOMOVE_H
#define GOMOVE_H

enum GoMoveState { EMPTY, BLACK, WHITE, KO, ILLEGAL };

typedef std::vector<std::vector<GoMoveState> > MOVEMATRIX;

struct GoMove
{
    unsigned short x, y;
    enum GoMoveState state;
};

#endif
