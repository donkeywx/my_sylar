#include <iostream>
#include "../sylar.h"
using namespace std;



int main()
{
    std::vector<std::string> strings;

    sylar::Backtrace(strings);

    for (int i = 0; i < strings.size(); i++)
    {
        cout << strings[i] << endl;
    }
    return 0;
}