#include <cstring>

#include "Rubik.hpp"


std::map<std::string, Rubik_Factory::AbstractFactory *> &Rubik_Factory::getFactories()
{
    // Hack to prevent static init fiasco
    static std::map<std::string, Rubik_Factory::AbstractFactory *> Factories;

    return Factories;
}

std::vector<std::string> Rubik_Factory::available_Algorithms()
{
    auto &Factories = getFactories();
    std::vector<std::string> keys;
    for (auto it = Factories.begin(); it != Factories.end(); ++it)
        keys.push_back(it->first);

    return keys;
}

std::unique_ptr<Rubik> Rubik_Factory::create_Rubik(std::string algorithm, bool showMoves)
{
    auto &Factories = getFactories();
    auto it = Factories.find(algorithm);

    if (it == Factories.end())
        return nullptr;

    return it->second->create(showMoves);
}

bool Rubik_Factory::register_Algorithm(std::string algorithm, AbstractFactory *factory)
{    
    // Check already registered
    auto &Factories = getFactories();
    if (Factories.count(algorithm))
        return false;
    
    Factories[algorithm] = factory;
    return true;
}
