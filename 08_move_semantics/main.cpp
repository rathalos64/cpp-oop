#include <iostream>

#define M() std::cout << __PRETTY_FUNCTION__ << std::endl;
#define SEPERATOR() std::cout << std::endl;

// Rvalue references allow a function to branch at compile time (via overload resolution) 
// on the condition "Am I being called on an lvalue or an rvalue?"! 
void Foo(int&) { M(); }
void Foo(int&&) { M(); }

int getFoo() { return 42; }

// Things that are declared as rvalue reference can be lvalues or rvalues. 
// The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue!

// std::move turns its argument into an rvalue!

// Move semantics allows an object, under certain conditions, to take ownership of some other object's external resources
//      Turning expensive copies into cheap moves

class Intvec
{
public:
    explicit Intvec(size_t num = 0) : m_size(num), m_data(new int[m_size]) { M(); }

    ~Intvec() {
        M();
        if (m_data) {
            delete[] m_data;
            m_data = 0;
        }
    }

    Intvec(const Intvec& other) : m_size(other.m_size), m_data(new int[m_size]) {
        M();
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    // Without a move assignment operator,
    // a call to the assignment operator with an rvalue
    // e.g 
    //      a = Intvec(42);
    //
    // will reuslt in:
    // Intvec::Intvec(size_t)
    // Intvec &Intvec::operator=(const Intvec &)
    // Intvec::Intvec(const Intvec &)
    // Intvec::~Intvec()
    // Intvec::~Intvec()
    // 
    // Meaning that the rvalue is composed in the constructor, passed to the assignment operator,
    // THEN a COPY of it is created (of a variable which wont' care about afterwards), and then its contents are swapper.
    // In the end, the temporary within the assignment operator is destroyed and then the rvalue is destroyed too (it's going out of scope).
    //
    // A COPY of a temporary value without a location in the memory is unnecessary and expensive (in terms of performance)
    Intvec& operator=(const Intvec& other) {
        M();
        Intvec tmp(other);
        std::swap(m_size, tmp.m_size);
        std::swap(m_data, tmp.m_data);
        return *this;
    }

    // With the move assignment operator, the assignment of an rvalue (which is treated as an rvalue reference)
    // is reduced to:
    // Intvec::Intvec(size_t)
    // Intvec &Intvec::operator=(Intvec &&)
    // Intvec::~Intvec()
    //
    // Meaning the temporary will be created and then its contents are moved out (its internals are stolen).
    // Afterwards, it will be destroyed. No unnecessary copy is created.
    // 
    // The magic of move semantics.
    Intvec& operator=(Intvec &&other) {
        M();
        m_size = other.m_size;
        m_data = other.m_data;
        other.m_data = nullptr;
        
        return *this;
    }

    int size() const { return m_size; }
private:
    size_t m_size;
    int* m_data;
};

int main() {
    int i = 1337;
    Foo(i);

    Foo(getFoo());
    SEPERATOR();

    Intvec a(42);
    Intvec b(1377);
    std::cout << "Size of variable intvec1: " << a.size() << std::endl;
    std::cout << "Size of intvec2: " << b.size() << std::endl;
    SEPERATOR();

    b = a;
    std::cout << "Size of intvec2: " << b.size() << std::endl;
    SEPERATOR();

    a = Intvec(9999);
    std::cout << "Size of intvec1: " << a.size() << std::endl;
    SEPERATOR();

    // If we would want that our a's internals will be stolen, when assigning to b, 
    // then we could - even though it is an lvalue - assign it through the move assignment operator.
    // For this purpose, there exists std::move which basically just casts an lvalue to an rvalue.
    // As rvalue defines as "something without a name which is going to be destroyed", std::move simply
    // hides the name of the passed variable, dequalifying it as an lvalue.
    //
    // BEWARE: after stealing the internals of an lvalue by casting it with std::move, don't dare to access it.
    // Otherwise, it will result in unexpected behaviour. Basically, we would try to access a memory area, which 
    // points to nothing. 
    //
    // In this case though, as we only swap the contents in Intvec& operator=(Intvec &&other),
    // a will contain the contents of b. But we don't care anyway.
    b = std::move(a);
    std::cout << "Size of intvec2: " << b.size() << std::endl;
    // std::cout << "Size of intvec1: " << a.size() << std::endl;
    SEPERATOR();
}