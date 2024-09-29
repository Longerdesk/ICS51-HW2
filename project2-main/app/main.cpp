#include "SkipList.hpp"
#include <iostream>

int main()
{
    SkipList<unsigned, unsigned> s1;
    s1.insert(3, 10);
    std::cout << "GOOD" << std::endl;
    std::cout << s1.numLayers() << std::endl;

    s1.insert(7, 11);
    return 0;
}

