#include "data.hpp"


Graph read_serialized(std::istream& stream)
{
    Graph input;

    long long int timestamp;
    Real x, y;

    while (stream >> timestamp >> x >> y) {
        input.emplace_back(x, y);
    }

    return input;
}
