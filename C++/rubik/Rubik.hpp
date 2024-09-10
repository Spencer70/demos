#include <memory>
#include <map>
#include <vector>
#include <string>

struct Rubik
{
    // UP, DOWN, RIGHT, LEFT, FRONT, BACK
    static constexpr const char MOVES[] = "UDRLFB";

    // Default step is 1 CW.
    // Extra steps: 2 CW, 1 CCW
    static constexpr const char STEPS[] = "2'";

    // Flag to show intermediate states
    bool show;

    Rubik(bool showMoves = false) : show(showMoves) {};
    virtual ~Rubik() = default;

    virtual void print() = 0;

    virtual std::string rotate(std::string moves = "RANDOM") = 0;

    virtual std::string shuffle(std::string moves = "RANDOM") = 0;

    virtual bool isSolved() = 0;

    virtual void solve() = 0;
};

class Rubik_Factory
{
public:

    struct AbstractFactory { virtual std::unique_ptr<Rubik> create(bool showMoves) const = 0; };
    // Factory helper
    // Declare in Rubik_Algorithms: static Rubik_Factory::Algorithm_Factory<Rubik_Algorithm> test_factory("Algorithm_name");
    template <class TRubik>
    struct Algorithm_Factory : public AbstractFactory
    {
        Algorithm_Factory(std::string algorithm) { Rubik_Factory::register_Algorithm(algorithm, this); }
        std::unique_ptr<Rubik> create(bool showMoves) const { return std::make_unique<TRubik>(showMoves); }
    };

    static std::unique_ptr<Rubik> create_Rubik(std::string algorithm = "Test", bool showMoves = false);

    static bool register_Algorithm(std::string algorithm, AbstractFactory *factory);

    static std::vector<std::string > available_Algorithms();

protected:

    static std::map<std::string , AbstractFactory *> &getFactories();
};
