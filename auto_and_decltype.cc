#include <initializer_list>
#include <vector>
#include <stdio.h>

template<typename T>
void f(T param);

void g()
{
    printf("authenticating user...\n");
}

template<typename T1, typename T2, typename T3>
T3 foo(T1& c, T2 i)
{
    g();
    return c[i];
}

// template<typename T1, typename T2>
// decltype(c[i]) foo(T1& c, T2 i) // error: c and i have not been declared/defined at decltype(c[i])
// {
//     g();
//     return c[i];
// }

template<typename T1, typename T2>
auto foo(T1& c, T2 i) -> decltype(c[i])
{
    g();
    return c[i];
}

template<typename T1, typename T2>
auto foo2(T1& c, T2 i)
{
    g();
    return c[i];
}

template<typename T1, typename T2>
decltype(auto) foo3(T1& c, T2 i)
{
    g();
    return c[i];
}

template<typename T1, typename T2>
decltype(auto) foo4(T1&& c, T2 i)
{
    g();
    return std::forward<T1>(c)[i];
}

// -------------------------------
// utilities

// declare but not define, to elicit an error message from which we can know a type deduced by compiler
template<typename T>
class TD;

// print all elements in a std::vector
void print(const std::vector<int>& v)
{
    printf("v = {");
    for (size_t i = 0; i < v.size() - 1; ++i)
        printf(" %i,", v[i]);
    printf(" %i }\n", v.back());
}

auto createInitList()
{
    // return { 1, 2, 3 }; // error: auto in a fuction return type implies template type deduction
}

int main()
{
    auto x = { 11 ,23, 9 }; // correct, x is deduced as std::intializer_list
    //f({11, 23, 9}); // error: can't deduce type for T
    
    std::vector<int> v;
    auto resetV = [&v](const auto& newValue) 
    {
        v = newValue;
    };
    //resetV({ 1, 2, 3 }); // error: auto in a lambda parameter implies template type deduction

    // -------------------------------
    // decltype test

    auto i = 5;
    //TD<decltype(i)> iType; // according to compiler error message, we know decltype(i) is int
    //TD<decltype((i))> i_with_parentheses; // according to compiler error message, we know decltype((i)) is int&

    const auto& i_ref_const = i;
    //TD<decltype(i_ref_const)> iRefType; // according to compiler error message, we know decltype(i_ref_const) is const int&
    
    auto i1 = i_ref_const; // i1 is in
    decltype(auto) i2 = i_ref_const; // i2 is const int&

    // -------------------------------
    // trailing return type test

    v = { 11, 23, 9 }; // init vector
    foo<std::vector<int>, int, int&>(v, 0) = 2;
    print(v);

    foo<std::vector<int>, int>(v, 1) = 5;
    print(v);

    //foo2<std::vector<int>, int>(v, 2) = 3; // error: what foo2 returns is a rvalue instead of lvalue

    foo3<std::vector<int>, int>(v, 2) = 3; // correct: since we use decltype(auto), foo3(c, i) returns exactly whatever c[i] returns
    print(v);

    //foo3<std::vector<int>, int>(std::vector<int>{2, 3, 4}, 1); // error: rvalue cannot be passed to lvalue reference

    printf("the temporary vector's 2nd element is: %i\n",foo4<std::vector<int>, int>(std::vector<int>{2, 3, 4}, 1));

    return 0;
}