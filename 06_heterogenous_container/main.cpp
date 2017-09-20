#include <iostream>
#include <typeinfo>
#include <vector>

class Animal {
public:
    virtual std::string type() const { return typeid(*this).name(); }
    virtual ~Animal() {}
};

class Bat : public Animal {};
class Bear : public Animal {};
class Moose : public Animal {};
class Shark : public Animal {};
class Squirrel : public Animal {};

void printZoo(std::vector<Animal *> &zoo) {
    for (Animal *a : zoo) {
        std::cout << a->type() << std::endl;
    }
}

int main() {
    // Whenever a variable may contain references (pointer) to objects other classes,
    // it is called a heterogenous data structure. As here, vector is a container where
    // elements can be added / removed, we speak of heterogenous container.
    //
    // IMPORTANT: The reason why heterogenous data structures / container work is due to polymorphism.
    // This means that all objects within the container must be directly or indirectly inherited from the base reference.
    // Otherwise, this concept wouldn't work.
    std::vector<Animal *> zoo;

    zoo.push_back(new Bat{});
    zoo.push_back(new Bear{});
    zoo.push_back(new Moose{});
    zoo.push_back(new Shark{});
    zoo.push_back(new Squirrel{});

    printZoo(zoo);
}