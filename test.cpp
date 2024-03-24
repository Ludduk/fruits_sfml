#include <fstream>
#include <iostream>
#include <string>

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

using namespace std;

int main()
{
    ifstream in("text.txt");
    string line;
    while (in >> line)
        cout << stof(line) << '\n';
    in.close();
    return 0;
}
