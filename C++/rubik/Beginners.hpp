class Cube3_Beginners : public Cube3
{
public:

    Cube3_Beginners(bool showMoves = false);

    virtual void solve();

private:

    const Piece *fromColors(const std::vector<Colors> &facets);
    bool matchFaces(const Piece *piece, const std::vector<Faces> &faces, bool matchAll = true);
    bool matchFacet2Face(const Piece *piece, Faces facet, Faces face);

    void L1L2Stage(const char *workingFace,
                    const Piece *(Cube3_Beginners::*findPieceCB)(Colors, Colors),
                    Colors (Cube3_Beginners::*preparePieceCB)(const Piece *, Colors),
                    void (Cube3_Beginners::*solvePieceCB)(const Piece *));

    void TopCross();
    const Piece *TopCross_findPiece(Colors side, Colors prev);
    Colors TopCross_preparePiece(const Piece *piece, Colors sideColor);
    void TopCross_solvePiece(const Piece *piece);
    
    void TopCorners();
    const Piece *TopCorners_findPiece(Colors side, Colors prev);
    Colors TopCorners_preparePiece(const Piece *piece, Colors sideColor);
    void TopCorners_solvePiece(const Piece *piece);

    void MiddleFlip();

    void MiddleEdges();
    const Piece *MiddleEdges_findPiece(Colors side, Colors prev);
    Colors MiddleEdges_preparePiece(const Piece *piece, Colors sideColor);
    void MiddleEdges_solvePiece(const Piece *piece);
    void swapEdge(bool toRight);

    template<typename F>
    std::vector<Piece *> findPieces(std::vector<Piece *> src, F f);

    void LLCross(std::vector<Piece *> LLEdges, Colors upColor);
    void LLSolvedCross(std::vector<Piece *> LLEdges);
    const Piece *findSemiLLCross(std::vector<Piece *> crossPieces);

    void LLRightPosCorners(std::vector<Piece *> LLCorners);
    void LLSolvedCorners(std::vector<Piece *> LLCorners, Colors upColor);
};
