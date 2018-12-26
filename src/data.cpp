#include "data.hpp"


Graph read_serialized(std::istream& stream, size_t count)
{
    Graph input;

    long long int timestamp;
    Real x, y;

    while (count > input.size() && stream >> timestamp >> x >> y)
        input.emplace_back(x, y);

    return input;
}


Graph read_serialized(std::istream& stream)
{
    return read_serialized(stream, std::numeric_limits<size_t>::max());
}
