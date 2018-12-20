#include <iostream>

#include "clustering.hpp"
#include "space.hpp"


int main()
{
    Point a {0, 0};
    Point b {1, 1};

    std::cout << dist(a, b) << '\n';



    Graph G = {
        {1, 2}, {1, 1},
        {10, 10}, {10, 11},
        {30, 39}
    };

    Graph cluster = a3_clustering(G, 2, 1);

    for (auto p : cluster) {
        std::cout << p.first << ',' << p.second << '\n';
    }

    std::cout << cluster.size() << '\n';

    return 0;
}
