#ifndef GOMOVETREE_H
#define GOMOVETREE_H

#include "gomove.h"

struct GoMoveTreeNode
{
    GoMove move;
    GoMoveTreeNode *parent;
    GoMoveTreeNode *child;
};

struct GoMoveNode
{
    unsigned int parent;
    unsigned short x, y;
    enum GoMoveState state;
};

#endif
