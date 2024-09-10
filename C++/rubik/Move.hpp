#include <string>

#include "Faces.hpp"

struct Move
{
    Faces face;
    int step;

    // Constructor of random move
    Move();

    Move(Faces face, int step);
    Move(std::string strMove);
    
    // Print string
    std::string print() const;

    // Create random moves
    static std::string create(int numMoves);
    // return number of parsed moves
    static std::vector<Move> parse(std::string strMoves);
};


