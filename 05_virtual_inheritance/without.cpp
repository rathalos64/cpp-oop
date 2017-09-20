#include <iostream>
#include <typeinfo>

#define METHOD(m) std::cout << m << std::endl;
#define SEPERATOR() std::cout << std::endl;

class Animal {
public:
    Animal() {  METHOD(__PRETTY_FUNCTION__); }
    Animal(std::string name) : name(name) { METHOD(__PRETTY_FUNCTION__); }
    virtual void makeNoise() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~Animal() { METHOD(__PRETTY_FUNCTION__); }
    std::string getName() const {
        return name;
    }

private:
    std::string name;
};

class Mammal : public Animal {
public:
    Mammal() : Animal("from Mammal") {  METHOD(__PRETTY_FUNCTION__); }
    virtual void eat() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~Mammal() { METHOD(__PRETTY_FUNCTION__); }
};

class WingedAnimal : public Animal {
public:
    WingedAnimal() : Animal("from WingedAnimal") {  METHOD(__PRETTY_FUNCTION__); }
    virtual void fly() const { METHOD(__PRETTY_FUNCTION__); }
    virtual ~WingedAnimal() { METHOD(__PRETTY_FUNCTION__); }
};

class Bat : public Mammal, public WingedAnimal {
public:
    Bat() {  METHOD(__PRETTY_FUNCTION__); }
    virtual ~Bat() { METHOD(__PRETTY_FUNCTION__); }
};

int main() {
    // Two instances of Animal will be constructed (as Mammal and WingedAnimal don't share a common base)
    // The order in which the different bases are constructed is defined by the order in the inheritance list
    //      public Mammal, public WingedAnimal
    //  
    // Animal::Animal()
    // Mammal::Mammal()
    // Animal::Animal()
    // WingedAnimal::WingedAnimal()
    // Bat::Bat()
    Bat bat; SEPERATOR();

    // You must specify the Base in order to call makeNoise
    // Without it, the compile won't understand which makeNoise you want to call
    //      Bat::WingedAnimal::Animal::makeNoise or Bat::Mammal::Animal::makeNoise
    //
    // bat.makeNoise() 
    //      non-static member 'makeNoise' found in multiple base-class subobjects of type 'Animal'
    bat.WingedAnimal::makeNoise(); 
    std::cout << bat.WingedAnimal::getName() << std::endl;
    SEPERATOR();

    // Working with pointers do work. 
    // Here m is of static type Mammal with dynamic type Bat
    // As Mammal has only one implemenation of makeNoise defined (in his Animal's class), it will work
    Mammal *m = &bat;
    m->eat();
    m->makeNoise();
    std::cout << m->getName() << std::endl;
    SEPERATOR();

    // However, casting bat to a pointer to Animal will fail (as there are multiple Animal bases)
    // Animal *a = new Bat{};
    // a->makeNoise();
    //      ambiguous conversion from derived class 'Bat' to base class 'Animal':

    SEPERATOR();

    // Please note the order in which the instances are destructed (again)
    // virtual Bat::~Bat()
    // virtual WingedAnimal::~WingedAnimal()
    // virtual Animal::~Animal()
    // virtual Mammal::~Mammal()
    // virtual Animal::~Animal()
}
