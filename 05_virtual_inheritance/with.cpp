#include <iostream>
#include <typeinfo>

#define METHOD(m) std::cout << m << std::endl;
#define SEPERATOR() std::cout << std::endl;

class Animal {
public:
    Animal() {  METHOD(__PRETTY_FUNCTION__); }
    virtual void makeNoise() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~Animal() { METHOD(__PRETTY_FUNCTION__); }
};

class Mammal : public virtual Animal {
public:
    Mammal() {  METHOD(__PRETTY_FUNCTION__); }
    virtual void eat() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~Mammal() { METHOD(__PRETTY_FUNCTION__); }
};

class WingedAnimal : public virtual Animal {
public:
    WingedAnimal() {  METHOD(__PRETTY_FUNCTION__); }
    virtual void fly() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~WingedAnimal() { METHOD(__PRETTY_FUNCTION__); }
};

class Bat : public Mammal, public WingedAnimal {
public:
    Bat() {  METHOD(__PRETTY_FUNCTION__); }
    virtual ~Bat() { METHOD(__PRETTY_FUNCTION__); }
};

int main() {
    // When inheriting virutally (public virtual ...), the bases of WingedAnimal and Mammal will be the same instances
    // See the construction order.
    //
    // Animal::Animal()
    // Mammal::Mammal()
    // WingedAnimal::WingedAnimal()
    // Bat::Bat()
    //
    // Note that the diamond problem only appeared, because the most derived class (Bat) inherited from two (or more)
    // classes which were derived from the same base (Animal)
    Bat bat; SEPERATOR();

    // Now, calling makeNoise (or sending the object bat the message makeNoise) will work
    // as there is only one Animal base
    bat.makeNoise(); SEPERATOR();

    // Now, even upcasting bat to Animal works
    Animal *a = &bat;
    a->makeNoise();

    SEPERATOR();
    
    // The destruction order
    // virtual Bat::~Bat()
    // virtual WingedAnimal::~WingedAnimal()
    // virtual Mammal::~Mammal()
    // virtual Animal::~Animal()
}
