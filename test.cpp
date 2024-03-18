#include <iostream>

using namespace std;

class A
{
    char x = 'a';
public:

    A()
    {
        cout << "A\n";
    }
    char get_char()
    {
        return x;
    }
};

class B : public A
{
    char x = 'b';
 public:
     B()
     {
         cout << "B\n";
     }
     char get_char()
     {
         return x;
     }
};

template <class T>
class I
{
    T t;


    public:

    I(){}
    
    char get_char()
    {
        return t.get_char();
    }

};



int main()
{

    I<A> i;
    I<B> j;

    cout << i.get_char() << '\n';

    cout << j.get_char() << '\n';
    return 0;
}
