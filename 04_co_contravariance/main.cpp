#include <iostream>
#include <typeinfo>

// Testing in terminal:
// g++ -o main -std=c++11 -Wall -Wextra -pedantic main.cpp && ./main
// g++ -v | Apple LLVM version 8.1.0 (clang-802.0.42)

// ================================================================================================================
/*
Co- and Contravariance are concepts in object-oriented programming which allows
overriding the interface of method within an inheritance tree. 
The allow a redeclaration of the data types within return parameters as well as their
argument parameters in methods.
The overriding though is restricted to some rules. Data types within the base class
can only be specialised (covariance) or generalised (contravariance) in the derived class.

C++ supports natively only covariant return types. Contravariant arguments can be simulated
with some tricks, which though won't be addressed in this chapter.

> References
Contravariance and covariance in C++
    http://cpptruths.blogspot.co.at/2015/11/covariance-and-contravariance-in-c.html

General definitions of co- and contravariance
    https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#Inheritance_in_object-oriented_languages
*/
// ================================================================================================================

// We will demonstrate the concept of covariant return types as well as contravariance with
// a class VehicleFactory which creates objects from type Vehicle.
// Futhermore, a class from type CarFactory will be derived from VehicleFactory, creating Cars
// 
// Below is only valid code for covariance, for contravariance the methods will be commented out

// The classes our factories will create
class Vehicle {
public:
    virtual std::string getType() const {
        return typeid(*this).name();
    }
};
class Car : public Vehicle {};

// Now we will create classes which will be used as arguments in
// contravariant create methods. There aren't actually used somewhere
class Metal {};
class Iron : public Metal {};

class VehicleFactory {
public:
    // Obviously, for overriding the method must be virtual
    virtual Vehicle * create() const { return new Vehicle{}; }

    // This function creates a Vehicle based on a given material
    // In our case, we use Iron *. In case you are wondering why Iron and not the
    // base class Metal, explanation will be found in CarFactory.
    //
    /* virtual Vehicle * create(Iron *) const { return new Vehicle{}; } */

    // Don't forget about the virtual destructor
    virtual ~VehicleFactory() {}
};

// Now our derived factory which is so sophisticated, it can even create car
class CarFactory : public VehicleFactory {
public:
    // IMPORTANT: Look at the interface of the method
    // You'd expect for it to be the same as for VehicleFactory::create with
    // Vehicle being the return type. 
    //
    // That's the concept of covariance. As the derived class CarFactory, which
    // is specialised returns also a specialised return parameter (as Car derives from Vehicle),
    // this is completely safe to write. It improves the derived interface of VehicleFactory::create
    // with a "better" return parameter.
    //
    // So, why does it even make sense to override the method and exchange its return type.
    // Imagine a situation like:
    //
    // CarFactory *factory = new CarFactory{};
    //
    // What would factory->create() return if we call it (without using covariant return types in create())?
    // A pointer to Vehicle. That doesn't seem right because we would want for it to create a Car. 
    // We would have to cast it dynamically to the right class down (downcast),
    // which should be avoided considering it unsafe.
    //
    // Therefore it totally makes sense to specialize the return parameter.
    //
    // One last note, contravariant return types work only with pointers and references.
    // Classes like std::unique_ptr<T> or std::shared_ptr<T> do not work.
    virtual Car * create() const override { return new Car{}; }

    // IMPORTANT: as for contravariance we (unlike in covariance) don't specialise but generalise from base to derived.
    // In fact, we generalise the argument from the derived class Iron to the base Metal.
    // 
    // Contravariance works in the opposite order as covariance.
    // Covariance:     generalised (base) -> specialised (derived), along the inheritance tree
    // Contravariance: specialised (base) -> generalised (derived)
    //
    // Again, think of why we would need a specialised argument in the base
    // and a generalized argument in the derived one.
    // Imagine a situation like:
    // 
    // VehicleFactory *factory = new CarFactory{};
    // 
    // In order to create a car we need Metal, because Metal is more fancy.
    // BUT, and here's is the but. We can pass any material to CarFactory::create that is derived from Metal.
    //
    // As VehicleFactory's create() interface is defined as "Vehicle *create(Iron *) const", we can
    // call factory->create(new Iron{}). If the dynamic type of factory is VehicleFactory, then Iron* as argument
    // will work. And if the dynamic type is CarFactory? Then, it will also work, as Iron is derived from Metal!!
    //
    /* virtual Car * create(Metal *) const override { return new Car{}; } */

};

int main() {
    // In this example, a covariant return type in CarFactory is needed, becuase
    // otherwise a pointer Vehicle would have been returned which we could only downcast (dangerous)
    CarFactory *factory = new CarFactory{};
    Car *car = factory->create();
    std::cout << car->getType() << std::endl;

    // While the example above needed covariant return types, this example could theoretically
    // have to got without them. 
    // The variable generalisedFactory returns a pointer to Car which then will be stored
    // within a pointer to Vehicle (upcast). It doesn't matter, whether CarFactory::create returns a Car
    // or a Vehicle as in the end it will be stored as a pointer of the base class (is-a relationship).
    //
    // If CarFactory::create returns a vehicle, the within the method new Car{} will be upcasted to Vehicle* and returned.
    // If CarFactory::create returns a car, then the upcast happens while assigning Car* to generalisedCar(Vehicle *)
    //
    // The point is though, that the client which is using the CarFactory in his code benefits from a correct defined
    // interface preventing confusions about why a car factory will create *only* vehicles.
    VehicleFactory *generalisedFactory = new CarFactory{};
    Vehicle *generalisedCar = generalisedFactory->create();
    std::cout << generalisedCar->getType() << std::endl;
}