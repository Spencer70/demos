#include "Rubik.hpp"
#include "Piece.hpp"
#include "Move.hpp"

class Cube3 : public Rubik
{
public:

    Cube3(bool showMoves = false);

    virtual void print();

    virtual std::string rotate(std::string moves = "RANDOM");

    virtual std::string shuffle(std::string moves = "RANDOM");

    virtual bool isSolved();

protected:

    static constexpr const int NUMPIECES = 20;
    // 8 pieces per faces (excluding centerpiece)
    static constexpr const int FACE_NUMPIECES = 8;
    static constexpr const int DIM = 3;

    // Pieces of cube
    // Cube array of pieces: 8 corners + 12 edges
    std::array<Piece, NUMPIECES> pieces;
    // color-face to pieces map
    std::map<Colors, std::vector<Piece *>> facePieces;

    // Position-face to color-face map. Variant with rotations
    std::map<Faces, Colors> faceColors;
    // Get color-sides of posiition-face
    std::array<Colors, NUM_SIDES> sideColors(Faces face);

    // Tranformations
    void _rotate(std::string strMoves);
    void permute(std::string strMoves);

private:

    static constexpr const int MAX_RANDOM_ROTATIONS = 5;
    static constexpr const int MAX_RANDOM_SHUFFLE = 20;

    // Topology invariants
    //
    // Initial faces color as ordered in enum declaration of Colors
    static const std::map<Faces, Colors> initFaceColors;
    // Opposite of faces
    static const std::map<Faces, Faces> oppositeFace;
    // Sides of faces. Clockwise from face
    static const std::map<Faces, std::array<Faces, NUM_SIDES>> sideFaces;

    void buildPiece(int piece, const std::vector<Colors> &facets);
   
    void printRow(int leftWidth, const Colors row[], const char *end);
    void drawFace(Faces face, Colors layout[DIM][DIM]);
};