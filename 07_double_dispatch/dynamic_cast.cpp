#include <iostream>

#define M() std::cout << __PRETTY_FUNCTION__ << std::endl;

class SpaceShip {
public:
    virtual ~SpaceShip() { M(); }
};
class ApolloSpacecraft : public SpaceShip { ~ApolloSpacecraft() { M(); } };

class Asteroid {
public:
    virtual void CollideWith(SpaceShip *target) const { 
        M(); 

        if (ApolloSpacecraft *a = dynamic_cast<ApolloSpacecraft *>(target)) { this->CollideWith(a); }
        else { /* invalid spaceship, default collision */ }

    }
    virtual ~Asteroid() { M(); }

protected:
    virtual void CollideWith(ApolloSpacecraft *) const { M(); } 
};

int main() {
    Asteroid *asteroid = new Asteroid{};
    SpaceShip *target = new ApolloSpacecraft{};

    asteroid->CollideWith(target);

    delete target;
    delete asteroid;
}