#include <iostream>
#include <typeinfo>

// Testing in terminal:
// g++ -o main -std=c++11 -Wall -Wextra -pedantic main.cpp && ./main
// g++ -v | Apple LLVM version 8.1.0 (clang-802.0.42)

// ================================================================================================================
/*
Mixin are a concept within OOP which aborts the idea of code reuse
through *only* inheritance or composition. Specifically, it abandons the "put-all-the-reusable-code-within-a-base-class"
approach. The problem with inheritance *only* is that there is not clear or intuitive way of
taking individual concepts and combining them together. 
Composition would work as a replacement but would abuse the actual relationship between the classes.

Mixins are meant to be used as building blocks, each of one depicts in most cases a orthogonal concept.
This means that the class does not have to inherit from base class in order to work.
These building block are basic primitive classes.

Think of Mixins as Lego blocks which you can combine in any particular order you want.

Mixins are realized with templates and inheritance in C++.

> References:
What are mixins as a concept? (must-read)
	https://stackoverflow.com/questions/18773367/what-are-mixins-as-a-concept

C++ Mixins (must-read)
	http://www.thinkbottomup.com.au/site/blog/C _Mixins_-_Reuse_through_inheritance_is_good

Inheritance is bad
	http://blogs.perl.org/users/sid_burn/2014/03/inheritance-is-bad-code-reuse-part-1.html
*/
// ================================================================================================================

// This is the base class which we can expand with other primitive classes.
// Mixins compose complex classes with just the needed functionality out of primitive classes. 
class Number {
public:
	typedef int value_type;
	
	Number() : n(0) {}
	Number(value_type n) : n(n) {}

	virtual void set(value_type n) { this->n = n; }
	virtual value_type get() const { return n; }

private:
	value_type n;
};

// This class represents the concept of something undoable. A value can be set an reverted for one step.
//
// Note the template parameter, meaning that this concept can build upon
// another concepts. The main advantage here is that any class could be enhanced
// with the Undoable mechanism. 
//
// With inheritance only, we would have written down the exact name
// of the class we want to inherit from. With templates, we are more flexible and can decide the
// way of the inherit tree.
template <typename BASE, typename value_type = typename BASE::value_type>
class Undoable : public BASE {
public:
	// side note: a cool way to "inherit" constructors which works better than
	//
	// Undoable() : BASE(0) {}
	// Undoable(value_type n) : BASE(n) {}
	//
	// https://stackoverflow.com/questions/347358/inheriting-constructors
	using BASE::BASE;

	virtual void set(value_type n) override {
		before = BASE::get();
		BASE::set(n); 
	}

	void undo() { BASE::set(before); }
private:
	value_type before;
};

// This class represents the concept of something redoable. A value can be forwarded to a previously reseted value.
template <typename BASE, typename value_type = typename BASE::value_type>
class Redoable : public BASE {
public:
	// side note: a cool way to "inherit" constructors which works better than
	//
	// Undoable() : BASE(0) {}
	// Undoable(value_type n) : BASE(n) {}
	//
	// https://stackoverflow.com/questions/347358/inheriting-constructors
	using BASE::BASE;

	virtual void set(value_type n) override {
		after = n;
		BASE::set(n); 
	}

	void redo() { BASE::set(after); }
private:
	value_type after;
};

int main() {
	// create a number which can only be undone
	Undoable<Number> n(0);
	std::cout << n.get() << std::endl;

	// set a number
	n.set(1337);
	std::cout << n.get() << std::endl;

	// undo it
	n.undo();
	std::cout << n.get() << std::endl;

	// undo it again, it remains the same
	n.undo();
	std::cout << n.get() << std::endl;

	// create a number which can be undone and redone
	// this is the magic of mixins, different funtionalities can be
	// build on one by one through template parameters
	Redoable<Undoable<Number>> x();

	// this can even combinated by a typedef / using
	// using MagicNumber = Redoable<Undoable<Number>>;
	typedef Redoable<Undoable<Number>> MagicNumber;

	// this could even be build in other directions
	// using TheSameMagicNumber = Undoable<Redoable<Number>>;
	typedef Undoable<Redoable<Number>> TheSameMagicNumber;

	MagicNumber number(9999);
	std::cout << number.get() << std::endl;
	number.set(10000);
	std::cout << number.get() << std::endl;
	number.undo();
	std::cout << number.get() << std::endl;
	number.redo();
	std::cout << number.get() << std::endl;
}