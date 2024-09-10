#include <cstdlib>
#include <cstring>

#include "Rubik.hpp"
#include "Move.hpp"

// Constructor of random move
Move::Move()
{    
    face = static_cast<Faces>(Rubik::MOVES[rand() % NUM_FACES]);  // 0..5
    step = 1 + rand() % sizeof(Rubik::STEPS);  // 1..3
}

Move::Move(Faces face, int step) : face(face), step(step) {}

Move::Move(std::string strMove)
{
    // Empty strings will throw exception
    face = static_cast<Faces>(strMove[0]);
    if (strMove.size() == 1)
        step = 1;
    else
    {
        const char *p = Rubik::STEPS;
        const char *s = strchr(p, strMove[1]);
        step = (s && *s ? 2 + (s - p) : 1);
    }
}


std::string Move::print() const
{
    std::string strMove;
    strMove.push_back(static_cast<char>(face));
    if (step >= 2)
        strMove.push_back(Rubik::STEPS[step - 2]);
    return strMove;
}

// return number of parsed moves
std::vector<Move> Move::parse(std::string strMoves)
{
    std::vector<Move> moves;

    const char *s;
    auto strMove = strMoves.begin();
    while (strMove != strMoves.end())
    {
        Faces face = static_cast<Faces>(*strMove);
        int step = 1;
        if (++strMove != strMoves.end() && (s = strchr(Rubik::STEPS, *strMove)))
        {
            step = 2 + (s - Rubik::STEPS);
            ++strMove;
        }
        moves.push_back(Move(face, step));
    }
    
    return moves;
}

// Create random moves
std::string Move::create(int numMoves)
{
    std::string strMoves;

    while (--numMoves >= 0)
        strMoves.append(Move().print());
    
    return strMoves;
}
