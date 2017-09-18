#include <iostream>
#include <typeinfo>

// Testing in terminal:
// g++ -o main -std=c++11 -Wall -Wextra -pedantic main.cpp && ./main
// g++ -v | Apple LLVM version 8.1.0 (clang-802.0.42)

// ================================================================================================================
/*
Multiple Dispatch refers to a extended abstraction technique which uses runtime
polymorphism. Unlike single dispatch where upon invocation the runtime type of
a single underlying object determines which function is called / dispatched, multiple dispatch
uses the runtime type of multiple objects for dynamic dispatching (late binding).

Multiple Dispatch is not natively implemented in C++.

> References:
A guide to multiple dispatch with a brilliant explanation of single dispatch and runtime polymorphism
    http://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/

Rutime polymorphism / subtyping
    https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Subtyping

Late binding / dynamic binding / dynamic dispatch in C++
    https://en.wikipedia.org/wiki/Late_binding#Late_binding_in_C.2B.2B
*/

class Animal {
public:
    virtual std::string getType() const {
        return typeid(*this).name();
    }

    virtual void hello() const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    // Do not forget to include a virtual destructor
    // in every class which is supposed to be used
    // polymorphic. And DON'T forget to make the destructor virtual.
    //
    // When the destructor is non-virtual then calling
    // delete a; where a is a pointer to Animal but the underlying
    // runtime type is Bear, results in calling Animal's destructor, not Bear's.
    virtual ~Animal() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};

class Bear : public Animal {
public:
    virtual void hello() const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    ~Bear() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};

class Lemming : public Animal {
public:
    virtual void hello() const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    ~Lemming() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};

void SayHello(Animal *a) {
    // With single dispatch, the runtime type of one single
    // object determines the function, which will be dispatched when invoked.
    // The correct function is dynamically bound to the function.
    a->hello();
}

// Here we overload several function which should demonstrate multiple dispatch.
// When calling Cuddle with both parameters being pointers to the base class Animal,
// it would be cool if C++ could resolve the pointer to their underlying runtime type
// call the appropriate overloaded function.
// With static types this works pretty well, but with dynamic types (pointer to base classes
// with underlying runtime types) this doesn't want to pull off.
//
// Say we call Cuddle(animal1, animal2) where
// animal1 = pointer to Animal | runtime type is Bear
// animal2 = pointer to Animal | runtime type is Lemming
// then the call should be resolve to 
// Cuddle(Animal *, Animal *) but to Cuddle (Bear *, Lemming *)
//
// Below the implementations:
void Cuddle(Animal *a, Animal *b) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Cuddle(Bear *b, Lemming *l) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// we have the parameters also be the other way around, otherwise is resolves naturally
// to Cuddle(Animal *, Animal *)
void Cuddle(Lemming *l, Bear *b) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
    Animal *a = new Bear();
    Animal *b = new Lemming();

    std::cout << "> Demonstrate single dynamic dispatch (dynamic binding)" << std::endl;
    SayHello(a);
    SayHello(b);
    std::cout << std::endl;

    // When we now call Cuddle with the both pointer as arguments
    // we would think that they are resolved based on their dynamic type.
    // But unfortenately, C++ doesn't provide this natively, so the fallback function
    // will be called Cuddle(Animal *, Animal *) instead. 
    //
    // C++ takes for multiple dispatch in this approach only the static type and NOT
    // the dynamic one.
    std::cout << "> Demonstrate failed multiple dynamic dispatch (dynamic binding)" << std::endl;
    Cuddle(a, b);
    std::cout << std::endl;

    std::cout << "> Show destructor calling order (only if implemented with virtual destructors)" << std::endl;
    delete a;
    delete b;
}
