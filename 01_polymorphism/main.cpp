#include <iostream>

// Testing in terminal:
// g++ -o main -std=c++11 -Wall -Wextra -pedantic main.cpp && ./main
// g++ -v | Apple LLVM version 8.1.0 (clang-802.0.42)

// ================================================================================================================
/*
The basic definition from TC++PL (the C++ Programming Language,
which is maintained and created by the creator Bjarne Stroustrup itself) 
at 12.2.6, 13.6.1, D&E 2.9:

> Polymorphism means,
providing a single interface (methods) to entities of different types. 
Virtual functions provide dynamic (run-time) polymorphism through an interface provided by a base class. 
Overloaded functions and templates provide static (compile-time) polymorphism.

Basically, this just means that polymorphism:
.. lets you write "algorithmic" code once and then apply it to many types of data, which helps writing reuseable code.

Usually, people are talking about polymorphism in terms of inheritance (virutal method, ...)

> Run-time means,
the compiler must generate code for all the types the program might handle 
while running, and at run-time the correct code is selected (the process of selecting the right method
is called virtual/dynamic dispatch, see below).

> Compile-time means,
the choice of type-specific code is made during compilation (mostly done by templating or function overloading).
Again, the process of choosing and selecting the right method is called dispatching, in this case static dispatching.

> Virtual / dynamic dispatch
is the process of selecting which implementation of a polymorphic operation (method or function) to call at run time. 
One requirement to ensure dynamic dispatch is by declaring a method within a class as virtual.
C++ compilers typically implement dynamic dispatch with a data structure called a virtual table (vtable) 
that defines the message-to-method mapping for a given class (C++ as such has no notion of a vtable). 
Instances of that type will then store a pointer to this table as part of their instance data. 

Important for the exam, C++ applies (run-time) polymorphism through virtual methods. At run-time the right method is
dispatched by dynamic dispatch / (late) binding.

> References:
Mechanism of polymorphism (compile-time, run-time, ...), terminology:
    https://stackoverflow.com/a/5854862

Polymorphism Wikipedia
    https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Subtyping

Static, dynamic (and multiple, not necessary though for this chapter) dispatch:
    https://softwareengineering.stackexchange.com/a/224689

Dynamic binding, general definition and C++ implementation:
    https://en.wikipedia.org/wiki/Dynamic_dispatch

Dynamic (late) binding and static (early) binding vs static dispatch and dynamic dispatch (mostly definitions, bit confusing):
    https://stackoverflow.com/a/20187789
    https://softwareengineering.stackexchange.com/a/200123

Virutal / dynamic dispatch implementation details (don't go there unless you are drunk):
    https://stackoverflow.com/a/3972780
*/
// ================================================================================================================

// Static polymorphism by function overloading
void f(int& x)    { x += 2; }
void f(double& x) { x += 2; }

// Dynamic polymorphism by virtual functions
struct Animal { 
    // in this case, it doesn't matter whether Animal is pure virtual or only virtual
    virtual void doSomething() = 0;
};

struct Lemming : public Animal {
    virtual void doSomething() override { std::cout << "🐀  " << "Being a cute lemming" << std::endl; }
};

struct Bear : public Animal {
    virtual void doSomething() override { std::cout << "🐻  " << "Being a strong protecting bear" << std::endl; }
};

void callDoSomething(Animal *a) {
    // here happens the resolve of the dynamic bound type at run-time
    // and the right method is dispatched dynamically / virtually
    // Note: a is run-time polymorphic
    a->doSomething();
}

int main() {
    // is resolved at compile time
    int a = 1337; double b = 42.0;
    f(a);
    f(b);

    // in case you're confused with the {} initializer, refer to https://stackoverflow.com/a/18222927
    Animal *animal = new Lemming{};
    // is resolved at run-time
    callDoSomething(animal);
    callDoSomething(new Bear{});

    // side note: animal is a lvalue and new Bear{} and rvalue
}