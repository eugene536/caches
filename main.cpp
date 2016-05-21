#include <iostream>
#include "fixedcache.hpp"
#include "lrucache.hpp"

using namespace std;

int main()
{
    cache::LRUCache<int, int> cache(10);
    cout << "Hello World!" << endl;
    return 0;
}
