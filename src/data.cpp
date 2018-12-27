#include "data.hpp"


Graph read_serialized(std::istream& stream, int count)
{
    Graph input;

    long long int timestamp;
    Real lon, lat;

    while (count > (int) input.size() && stream >> timestamp >> lon >> lat)
        input.emplace_back(lon, lat);

    return input;
}


Graph read_serialized(std::istream& stream)
{
    return read_serialized(stream, std::numeric_limits<int>::max());
}
