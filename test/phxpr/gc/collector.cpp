//  Copyright (c) 2011 Bryce Lelbach
//  Copyright (c) 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <phxpr/gc/make_shared.hpp>
#include <phxpr/gc/shared_ptr.hpp>
#include <phxpr/gc/collector.hpp>

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct X
{
    void* fill[32];
    phxpr::shared_ptr<X> p;
};

void report()
{
    std::cout << "Calling find_unreachable_objects:\n";

    std::clock_t t = std::clock();

    std::size_t n = phxpr::find_unreachable_objects(true);

    t = std::clock() - t;

    std::cout << n << " unreachable objects.\n";
    std::cout << "  " << static_cast<double>(t) / CLOCKS_PER_SEC << " seconds.\n";
}

void free()
{
    std::cout << "Calling free_unreachable_objects:\n";

    std::clock_t t = std::clock();

    phxpr::free_unreachable_objects(true);

    t = std::clock() - t;

    std::cout << "  " << static_cast<double>(t) / CLOCKS_PER_SEC << " seconds.\n";
}

int main()
{
    std::vector< phxpr::shared_ptr<X> > v1, v2;

    int const n = 256;

    std::cout << "Filling v1 and v2\n";

    for(int i = 0; i < n; ++i)
    {
        v1.push_back(phxpr::make_shared<X>());
        v2.push_back(phxpr::make_shared<X>());
    }

    report();

    std::cout << "Creating the cycles\n";

    for(int i = 0; i < n - 1; ++i)
    {
        v2[i]->p = v2[i+1];
    }

    v2[n-1]->p = v2[0];

    report();

    std::cout << "Resizing v2 to size of 1\n";

    v2.resize(1);
    report();

    std::cout << "Clearing v2\n";

    v2.clear();
    report();

    std::cout << "Clearing v1\n";

    v1.clear();
    report();

    free();
    report();
}

