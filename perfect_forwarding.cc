#include <stdio.h>
#include <utility>
#include <memory>

template <typename T, typename Arg>
std::shared_ptr<T> factory(Arg&& arg)
{
    return std::shared_ptr<T>(new T(std::forward<Arg>(arg)));
}

class A
{
public:
    A()
    {
        printf("default constructor being called\n");
        size_ = 10;
        data_ = new char[size_];
    }

    A(size_t s)
    {
        printf("1 parameter constructor being called\n");
        size_ = s;
        data_ = new char[size_];
    }

    A(const A& other)
    {
        printf("copy constructor being called\n");
        data_ = new char[other.size_];
        std::copy(other.data_, other.data_ + other.size_, data_);
        size_ = other.size_;
    }

    A(A&& other)
    {
        printf("move constructor being called\n");
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    ~A()
    {        
        printf("destructor being called\n");
        if (data_)
            delete [] data_;        
    }


private:
    size_t size_;
    char* data_;
};

int main()
{
    A a1; // call default constructor
    auto a2 = factory<A>(5); // call 1 parameter constructor
    auto a3 = factory<A>(a1); // call copy constructor
    auto a4 = factory<A>(A()); // call move constructor, where a temporary instance of A is instantiated and is destructed soon

    return 0;
}

