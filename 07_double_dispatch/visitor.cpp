#include <iostream>
#include <typeinfo>

#define M(m) std::cout << m << std::endl;
#define SEPERATOR() std::cout << std::endl;

class SpaceShip;
class ApolloSpacecraft;

class Asteroid {
public:
    virtual void CollideWith(SpaceShip *) const { M(__PRETTY_FUNCTION__); }
    virtual void CollideWith(ApolloSpacecraft *) const { M(__PRETTY_FUNCTION__); }
    virtual ~Asteroid() { M(__PRETTY_FUNCTION__); }
};

class ExplodingAsteroid : public Asteroid {
public:
    virtual void CollideWith(SpaceShip *) const override { M(__PRETTY_FUNCTION__); }
    virtual void CollideWith(ApolloSpacecraft *) const override { M(__PRETTY_FUNCTION__); }
};

class SpaceShip {
public:
    // IMPORTANT method
    virtual void CollideWith(Asteroid *a) { a->CollideWith(this); }
    virtual ~SpaceShip() { M(__PRETTY_FUNCTION__); }
};
class ApolloSpacecraft : public SpaceShip {
    // IMPORTANT method
    virtual void CollideWith(Asteroid *a) { a->CollideWith(this); }
};

int main() {
    SpaceShip *target = new ApolloSpacecraft{};
    Asteroid *asteroid = new ExplodingAsteroid{};

    // Even though the dynamic type of asteroid (ExplodingAsteroid) resolves to the correct method
    // ExplodingAsteroid::CollideWith(), target is being interpreted based on his static type (Spaceship).
    // In C++, function overloading is done statically.
    asteroid->CollideWith(target);

    // In order to resolve target to his dynamic type, we must create another dynamic dispatch.
    // This is done by using a visitor pattern example. Other examples would work as well (like with dynamic casting).
    // Here, target resolves dynamically to ApolloSpacecraft::CollideWith and inside of this method,
    // a (from static type Asteroid, but from dynamic type ExplodingAsteroid) resolves again correctly to 
    // ExplodingAsteroid::CollideWith, passing this which is a pointer to ApolloSpacecraft as argument
    // meaning 
    //      ExplodingAsteroid::CollideWith(ApolloSpacecraft *) is the correct call.
    target->CollideWith(asteroid);
}