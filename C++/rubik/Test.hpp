class Cube3_Test : public Cube3
{
public:

    Cube3_Test(bool showMoves = false);

    virtual std::string shuffle(std::string moves = "RANDOM");
    virtual void solve();

private:

    std::string strMoves;
};
