#include <stdexcept>
#include <iostream>

#include "bamboo_gamerecord.h"
#include "gopoint.h"

BambooGamerecord::BambooGamerecord(const unsigned short &size)
{
    boardSize = size;
    currentMove = 0;
    nextMove = BLACK;
    blackCaptures = 0;
    whiteCaptures = 0;
}

BambooGamerecord::~BambooGamerecord()
{
}

unsigned short BambooGamerecord::getSize() const
{
    return boardSize;
}

MOVEMATRIX BambooGamerecord::getMatrix()
{
    MOVEMATRIX matrix;
    matrix.reserve(boardSize);
    for (unsigned short y = 0; y < boardSize; ++y) {
        std::vector<GoMoveState> y_row;
        y_row.reserve(boardSize);
        matrix.push_back(y_row);
        for (unsigned short x = 0; x < boardSize; ++x) {
            matrix.at(y).push_back(EMPTY);
        }
    }

    //std::cout << "made matrix of size: " << matrix.size() << "," << matrix.back().size() << std::endl;

    if (tree.empty()) {
        return matrix;
    }

    for (std::map<unsigned int, GoMoveNode>::const_iterator i = tree.begin(); i != tree.end(); ++i) {
        if (EMPTY == matrix.at(i->second.y).at(i->second.x)) {
            //std::cout << "playing out tree move (" << i->second.x << "," << i->second.y << ")" << std::endl;
            matrix = removeCaptures(matrix, i->second.x, i->second.y, i->second.state);
            try {
                matrix.at(i->second.y).at(i->second.x) = i->second.state;
            } catch (std::out_of_range& oor) {
                std::cout << "BambooGamerecord::getMatrix() Out of range: " << oor.what() << std::endl;
            }
        }
    }

    return matrix;
}

void BambooGamerecord::addMove(unsigned short x, unsigned short y)
{
    addMove(nextMove, x, y);
}

void BambooGamerecord::addMove(const GoMoveState &color, unsigned short x, unsigned short y)
{
    //std::cout << "Move at (" << x << "," << y << ")" << std::endl;

    const std::vector<std::vector<GoMoveState> > matrix = getMatrix();

    if (y >= matrix.size()
        || x >= matrix.at(y).size()
        || matrix.at(y).at(x) != EMPTY
    ) {
        return;
    }

    // return if move is suicide

    const unsigned int num = tree.empty() ? 0 : (tree.end()->first)+1;
    GoMoveNode n = {currentMove, x, y, color};
    tree.insert(std::map<unsigned int, GoMoveNode>::value_type(num, n));
    nextMove = color == BLACK ? WHITE : BLACK;
}

GoMoveState BambooGamerecord::getNextMove() const
{
    return nextMove;
}

std::vector<std::vector<GoMoveState> >
BambooGamerecord::removeCaptures(std::vector<std::vector<GoMoveState> > matrix,
                           unsigned short x, unsigned short y,
                           GoMoveState color)
{
    // TODO
    //  remove duplicate code

    try {
        // looking at stones N of placed stone
        if ((matrix.at(y - 1).at(x) == WHITE && color == BLACK)
            || (matrix.at(y - 1).at(x) == BLACK && color == WHITE)
        ) {
            GOPOINTS pendingCapsN;
            GOPOINTS toRemoveN;
            unsigned short libs = numLibs(matrix, x, y - 1, pendingCapsN, toRemoveN);
            std::cout << "Group at (" << x << "," << y -1 << ") has " << libs << " libs before placement" << std::endl;
            if (1 == libs) {
                matrix = removeGroup(matrix, toRemoveN);
            }
        }
    } catch (std::out_of_range& oor) {
        std::cout << "BambooGamerecord::removeCaptures Out of range: " << oor.what() << std::endl;
    }
   
    try {
        // looking at stones S of placed stone
        if ((matrix.at(y + 1).at(x) == WHITE && color == BLACK)
            || (matrix.at(y + 1).at(x) == BLACK && color == WHITE)
        ) {
            GOPOINTS pendingCapsS;
            GOPOINTS toRemoveS;
            unsigned short libs = numLibs(matrix, x, y + 1, pendingCapsS, toRemoveS);
            std::cout << "Group at (" << x << "," << y +1 << ") has " << libs << " libs before placement" << std::endl;
            if (1 == libs) {
                matrix = removeGroup(matrix, toRemoveS);
            }
        }
    } catch (std::out_of_range& oor) {
        std::cout << "BambooGamerecord::removeCaptures Out of range: " << oor.what() << std::endl;
    }
   
    try {
        // looking at stones W of placed stone
        if ((matrix.at(y).at(x - 1) == WHITE && color == BLACK)
            || (matrix.at(y).at(x - 1) == BLACK && color == WHITE)
        ) {
            GOPOINTS pendingCapsW;
            GOPOINTS toRemoveW;
            unsigned short libs = numLibs(matrix, x -1 , y, pendingCapsW, toRemoveW);
            std::cout << "Group at (" << x - 1 << "," << y << ") has " << libs << " libs before placement" << std::endl;
            if (1 == libs) {
                matrix = removeGroup(matrix, toRemoveW);
            }
        }
    } catch (std::out_of_range& oor) {
        std::cout << "BambooGamerecord::removeCaptures Out of range: " << oor.what() << std::endl;
    }
   
    try {
        // looking at stones E of placed stone
        if ((matrix.at(y).at(x + 1) == WHITE && color == BLACK)
            || (matrix.at(y).at(x + 1) == BLACK && color == WHITE)
        ) {
            GOPOINTS pendingCapsE;
            GOPOINTS toRemoveE;
            unsigned short libs = numLibs(matrix, x + 1, y, pendingCapsE, toRemoveE);
            std::cout << "Group at (" << x + 1 << "," << y << ") has " << libs << " libs before placement" << std::endl;
            if (1 == libs) {
                matrix = removeGroup(matrix, toRemoveE);
            }
        }
    } catch (std::out_of_range& oor) {
        std::cout << "BambooGamerecord::removeCaptures Out of range: " << oor.what() << std::endl;
    }

    return matrix;
}

unsigned short
BambooGamerecord::numLibs(std::vector<std::vector<GoMoveState> > matrix,
                    unsigned short x, unsigned short y,
                    GOPOINTS &pendingCaps, GOPOINTS &toRemove)
{
    toRemove.push_back(GoPoint(x, y));
    const GoMoveState color = matrix.at(y).at(x);
    unsigned short libs = 0;
    for (unsigned short i = 0; i < 4; ++i) {
        unsigned short ix, iy;
        if (0 == i) {
            if (0 == x) {
                continue;
            }
            ix = x - 1;
            iy = y;
        } else if (1 == i) {
            ix = x + 1;
            iy = y;
        } else if (2 == i) {
            if (0 == y) {
                continue;
            }
            ix = x;
            iy = y - 1;
        } else if (3 == i) {
            ix = x;
            iy = y + 1;
        }

        if (ix >= boardSize
            || iy >= boardSize
        ) {
            continue;
        }

        GoPoint p(ix, iy);
        bool found = false;
        for(GOPOINTS::const_iterator it = pendingCaps.begin(); it != pendingCaps.end(); ++it) {
            if (*it == p) {
                found = true;
                break;
            }
        }
        if (found)
            continue;
        pendingCaps.push_back(GoPoint(ix, iy));

        if (EMPTY == matrix.at(iy).at(ix)) {
            libs += 1;
        } else if (color == matrix.at(iy).at(ix)) {
            libs += numLibs(matrix, ix, iy, pendingCaps, toRemove);
        }
    }
    return libs;
}

MOVEMATRIX BambooGamerecord::removeGroup(MOVEMATRIX matrix, GOPOINTS &toRemove)
{
    for(GOPOINTS::const_iterator it = toRemove.begin(); it != toRemove.end(); ++it) {
        std::cout << "removing (" << it->x() << "," << it->x() << ")" << std::endl;
        matrix.at(it->y()).at(it->x()) = EMPTY;
    }
    return matrix;
}

MOVEMATRIX BambooGamerecord::removeGroup(MOVEMATRIX matrix, unsigned short x,
                                   unsigned short y)
{
    std::vector<GoPoint> removedCaps;
    return removeGroup(matrix, x, y, removedCaps);
}

MOVEMATRIX BambooGamerecord::removeGroup(MOVEMATRIX matrix, unsigned short x,
                                   unsigned short y,
                                   std::vector<GoPoint> &removedCaps)
{
    removedCaps.push_back(GoPoint(x, y));
    const GoMoveState color = matrix.at(y).at(x);
    for (unsigned short i = 0; i < 4; ++i) {
        unsigned short ix, iy;
        if (0 == i) {
            if (0 == x) {
                continue;
            }
            ix = x - 1;
            iy = y;
        } else if (1 == i) {
            ix = x + 1;
            iy = y;
        } else if (2 == i) {
            if (0 == y) {
                continue;
            }
            ix = x;
            iy = y - 1;
        } else if (3 == i) {
            ix = x;
            iy = y + 1;
        }

        if (ix >= matrix.size()
            || iy >= matrix.size()
        ) {
            continue;
        }

        std::cout << "Remove search (" << ix << "," << iy << ") color " << color << std::endl;

        if (color == matrix.at(iy).at(ix)) {
            GoPoint p(ix, iy);
            bool found = false;
            for(std::vector<GoPoint>::const_iterator it = removedCaps.begin(); it != removedCaps.end(); ++it) {
                if (*it == p) {
                    found = true;
                    break;
                }
            }
            if (found)
                continue;
            matrix = removeGroup(matrix, iy, ix, removedCaps);
        }
    }
    std::cout << "Removing (" << x << "," << y << ")" << std::endl;
    matrix.at(y).at(x) = EMPTY;
    return matrix;
}
