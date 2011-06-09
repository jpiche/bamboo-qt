#ifndef GAMERECORD_H
#define GAMERECORD_H

#include <vector>
#include <map>

#include "gomovetree.h"
#include "gopoint.h"

class BambooGamerecord
{
public:
    BambooGamerecord(const unsigned short&);
    ~BambooGamerecord();
    std::vector<std::vector<GoMoveState> > getMatrix();
    unsigned short getSize() const;
    void addMove(const GoMoveState &color, unsigned short x, unsigned short y);
    void addMove(unsigned short x, unsigned short y);
    GoMoveState getNextMove() const;

private:
    MOVEMATRIX removeCaptures(MOVEMATRIX matrix, unsigned short x, unsigned short y, GoMoveState color);
    MOVEMATRIX removeGroup(MOVEMATRIX matrix, unsigned short x, unsigned short y);
    MOVEMATRIX removeGroup(MOVEMATRIX matrix, unsigned short x, unsigned short y, std::vector<GoPoint> &removedCaps);
    MOVEMATRIX removeGroup(MOVEMATRIX matrix, GOPOINTS &toRemove);
    unsigned short numLibs(MOVEMATRIX matrix, unsigned short x, unsigned short y, GOPOINTS &pendingCaps, GOPOINTS &toRemove);

    unsigned short boardSize;
    unsigned int currentMove;
    unsigned int blackCaptures;
    unsigned int whiteCaptures;
    std::map<unsigned int, GoMoveNode> tree;
    GoMoveState nextMove;
};

#endif

