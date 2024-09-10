#include "Cube3.hpp"
#include <algorithm>

// Initial faces color as ordered in enum declaration of Colors
const std::map<Faces, Colors> Cube3::initFaceColors =
{
    { Faces::UP, Colors::WHITE },
    { Faces::DOWN, Colors::YELLOW },
    { Faces::RIGHT, Colors::BLUE },
    { Faces::LEFT, Colors::GREEN },
    { Faces::FRONT, Colors::RED },
    { Faces::BACK, Colors::ORANGE }
};

// Opposite of faces
const std::map<Faces, Faces> Cube3::oppositeFace =
{
    { Faces::UP, Faces::DOWN },
    { Faces::DOWN, Faces::UP },
    { Faces::RIGHT, Faces::LEFT },
    { Faces::LEFT, Faces::RIGHT },
    { Faces::FRONT, Faces::BACK },
    { Faces::BACK, Faces::FRONT }
};
// Sides of faces. Clockwise from face
const std::map<Faces, std::array<Faces, NUM_SIDES>> Cube3::sideFaces =
{
    { Faces::UP, { Faces::LEFT, Faces::BACK, Faces::RIGHT, Faces::FRONT } },
    { Faces::DOWN, { Faces::LEFT, Faces::FRONT, Faces::RIGHT, Faces::BACK } },
    { Faces::RIGHT, { Faces::FRONT, Faces::UP, Faces::BACK, Faces::DOWN } },
    { Faces::LEFT, { Faces::BACK, Faces::UP, Faces::FRONT, Faces::DOWN } },
    { Faces::FRONT, { Faces::LEFT, Faces::UP, Faces::RIGHT, Faces::DOWN } },
    { Faces::BACK, { Faces::RIGHT, Faces::UP, Faces::LEFT, Faces::DOWN } }
};

std::array<Colors, NUM_SIDES> Cube3::sideColors(Faces face)
{
    std::array<Colors, NUM_SIDES> sideColors;

    for (int i = 0; i < sideColors.size(); i++)
        sideColors[i] = faceColors.at(sideFaces.at(face)[i]);

    return sideColors;
}

Cube3::Cube3(bool showMoves) : Rubik(showMoves), faceColors(initFaceColors)
{
    // Seeds random moves.
    srand((uintptr_t)this);

    // piece counter
    int piece = 0;
    std::array<Colors, 2> UpDown = { faceColors.at(Faces::UP), faceColors.at(Faces::DOWN) };
    std::array<Colors, NUM_SIDES> sides = sideColors(Faces::UP);
    for (int side = 0; side < sides.size(); side++)
    {
        int sideNext = (side + 1) % sides.size();

        // Middle edges
        buildPiece(piece++, { sides[side], sides[sideNext] });
        for (Colors sideUpDown : UpDown)
        {
            // Up, down edges
            buildPiece(piece++, { sides[side], sideUpDown });
            // Corners
            buildPiece(piece++, { sides[side], sides[sideNext], sideUpDown });
        }
    }
}

void Cube3::buildPiece(int piece, const std::vector<Colors> &facets)
{
    // Create piece and store it
    pieces[piece].Init(facets);
    // Add piece reference (raw pointer) to faces
    // At init, facets are pointing to same color face
    for (Colors facet : facets)
        facePieces[facet].push_back(&pieces[piece]);
}

void Cube3::print()
{
    // print cube faces unfolded
    Colors layout[DIM][DIM];
    int leftWidth = 2 * DIM * 4; // 4 = 2 * escape + letter + white-space

    // UP
    drawFace(Faces::UP, layout);
    for (int row = 0; row < DIM; row++)
        printRow(leftWidth, layout[row], "\n");

    // LEFT - FRONT - RIGHT - BACK. Sides of DOWN
    Colors layout4[NUM_SIDES][DIM][DIM];
    auto sides = sideFaces.at(Faces::DOWN);
    for (int side = 0; side < NUM_SIDES; side++)
        drawFace(sides[side], layout4[side]);
    for (int row = 0; row < DIM; row++)
    {
        for (int side = 0; side < NUM_SIDES; side++)
            printRow(0, layout4[side][row], "");
        printf("\n");
    }

    // DOWN
    drawFace(Faces::DOWN, layout);
    for (int i = 0; i < DIM; i++)
        printRow(leftWidth, layout[i], "\n");
}

void Cube3::printRow(int leftWidth, const Colors row[], const char *end)
{
    // Colored letters to print facets. ANSI codes.
    const std::map<const Colors, const char *> colorCodes =
    {
        // color             color_escape      letter  reset_escape
        { Colors::WHITE,    "\e[1;38;5;015m"     "W"     "\e[0m" },
        { Colors::YELLOW,   "\e[1;38;5;226m"     "Y"     "\e[0m" },
        { Colors::BLUE,     "\e[1;38;5;012m"     "B"     "\e[0m" },
        { Colors::GREEN,    "\e[1;38;5;010m"     "G"     "\e[0m" },
        { Colors::RED,      "\e[1;38;5;009m"     "R"     "\e[0m" },
        { Colors::ORANGE,   "\e[1;38;5;166m"     "O"     "\e[0m" }
    };

    printf("%*s %s %s %s", leftWidth, colorCodes.at(row[0]), colorCodes.at(row[1]), colorCodes.at(row[2]), end);
}

void Cube3::drawFace(Faces face, Colors layout[DIM][DIM])
{
    Colors faceColor = faceColors.at(face);

    std::array<Colors, NUM_SIDES> sides = sideColors(face); 
    // Init layout with centerpiece
    layout[1][1] = faceColor;
    
    for (auto ppiece : facePieces.at(faceColor))
    {
        int row = 1, col = 1;
        Colors pieceColor = faceColor;

        for (auto it : *ppiece)
        {
            Colors piece_facet = it.first;
            Colors piece_face = it.second;

            if (piece_face == faceColor)
                pieceColor = piece_facet;
            else if (piece_face == sides.at(static_cast<int>(Sides::TOP)))
                row = 0;
            else if (piece_face == sides.at(static_cast<int>(Sides::BOTTOM)))
                row = 2;
            if (piece_face == sides.at(static_cast<int>(Sides::LEFT)))
                col = 0;
            else if (piece_face == sides.at(static_cast<int>(Sides::RIGHT)))
                col = 2;
        }
        layout[row][col] = pieceColor;
    }
}

std::string Cube3::rotate(std::string moves)
{
    if (moves == "RANDOM")
        moves = Move::create(rand() % MAX_RANDOM_ROTATIONS);
    
    if (show)
        printf("Rotation: %s\n", moves.c_str());

    _rotate(moves);

    return moves;
}

void Cube3::_rotate(std::string strMoves)
{
    std::vector<Move> moves = Move::parse(strMoves);

    for (Move const& move : moves)
    {
        auto &sides = sideFaces.at(move.face);
        
        // Copy previous face colors
        auto prevFaceColors = faceColors;
        for (int i = 0; i < NUM_SIDES; i++)
        {
            // Assign previous face color to current face
            int prevFace = (NUM_SIDES + i - move.step) % NUM_SIDES;
            Colors prevColor = prevFaceColors[sides[prevFace]];
            faceColors[sides[i]] = prevColor;
        }
        if (show)
        {
            printf("%s\n", move.print().c_str());
            print();
        }
    }
}

std::string Cube3::shuffle(std::string moves)
{
    if (moves == "RANDOM")
        moves = Move::create(1 + MAX_RANDOM_SHUFFLE / 2 + rand() % MAX_RANDOM_SHUFFLE / 2);

    if (show)
        printf("Shuffle: %s\n", moves.c_str());

    permute(moves);

    return moves;
}

void Cube3::permute(std::string strMoves)
{
    std::vector<Move> moves = Move::parse(strMoves);
    for (Move const& move : moves)
    {
        auto &faceColor = faceColors.at(move.face);
        auto sides = sideColors(move.face);

        // Remove pieces of this rotating face from side faces
        const std::vector<Colors> vfaceColor = { faceColors.at(move.face) };
        for (auto &side : sides)
        {
            auto &sidePieces = facePieces[side];
            auto it = std::remove_if(sidePieces.begin(), sidePieces.end(), 
                                            [vfaceColor](Piece *p) { return p->matchFaces({vfaceColor}, false); });
            sidePieces.erase(it, sidePieces.end());
        }

        // Rotate pieces
        Colors *pside;
        for (auto ppiece : facePieces[faceColor])
            for (auto facet_face : *ppiece)
                if ((pside = std::find(sides.begin(), sides.end(), facet_face.second)) != sides.end())
                {
                    // Only changes piece facing a side
                    int side = std::distance(sides.begin(), pside);
                    int nextSide = (NUM_SIDES + side + move.step) % NUM_SIDES;
                    Colors nextColor = sides[nextSide];
                    // Update face of facet
                    ppiece->at(facet_face.first) = nextColor;
                    // Add to new side
                    facePieces[nextColor].push_back(ppiece);
                }

        if (show)
        {
            printf("%s\n", move.print().c_str());
            print();
        }
    }
}

bool Cube3::isSolved()
{
    auto predicate = [](auto const& piece){ return piece.isSolved(); };
    return std::all_of(pieces.begin(), pieces.end(), predicate);
}

