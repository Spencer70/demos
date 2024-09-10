#include "Cube3.hpp"
#include "Test.hpp"
#include <algorithm>


static Rubik_Factory::Algorithm_Factory<Cube3_Test> test_factory("Test");


Cube3_Test::Cube3_Test(bool showMoves) : Cube3(showMoves) {}

std::string Cube3_Test::shuffle(std::string moves)
{
    strMoves = Cube3::shuffle(moves);

    return strMoves;
}

void Cube3_Test::solve()
{
    // Test algorithm.
    // Take stored moves and reverse them.
    auto moves = Move::parse(strMoves);

    std::string strReversedMoves;
    for (auto it = moves.rbegin(); it != moves.rend(); it++)
    {
        if (it->step != NUM_SIDES / 2)
            it->step = (it->step + NUM_SIDES / 2) % NUM_SIDES;
        strReversedMoves.append(it->print());
    }

    permute(strReversedMoves);
}