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

StreamingGraph::StreamingGraph(std::ifstream &file) :
    cursor (0),
    file (file)
{
    assert(file.is_open());
}

void StreamingGraph::reset()
{
    assert(file.is_open());

    cursor = 0;
    file.clear();
    file.seekg(0, std::ios::beg);
}

Graph StreamingGraph::read(int count)
{
    assert(file.is_open());

    const Graph res = read_serialized(file, count);
    cursor += res.size();
    return res;
}

size_t StreamingGraph::size()
{
    assert(file.is_open());

    while (!read(1024).empty());
    return cursor;
}

StreamingGraphIterator StreamingGraph::begin()
{
    assert(file.is_open());

    reset();
    return StreamingGraphIterator(*this);
}

StreamingGraphIterator StreamingGraph::end()
{
    std::ifstream tmp_stream;
    StreamingGraph tmp_graph (tmp_stream);
    StreamingGraphIterator it (tmp_graph);
    it.overflow = true;
    return it;
}

StreamingGraphIterator::StreamingGraphIterator(StreamingGraph& graph) :
    overflow (false),
    graph (graph)
{
    const Graph& initial = graph.read(1);

    if (initial.empty())
        overflow = true;
    else
        value = initial[0];
}

StreamingGraphIterator StreamingGraphIterator::operator++(int count)
{
    graph.read(count-1);

    const Graph res = graph.read(1);

    if (res.empty())
        overflow = true;
    else
        value = res[0];

    return *this;
}

StreamingGraphIterator StreamingGraphIterator::operator++()
{
    return operator++(1);
}

Point StreamingGraphIterator::operator*() const
{
    return value;
}

Point* StreamingGraphIterator::operator->()
{
    return &value;
}

bool operator==(const StreamingGraphIterator& it_a,
    const StreamingGraphIterator& it_b)
{
    return (it_a.overflow && it_b.overflow) || ((!it_a.overflow &&
        !it_b.overflow) && it_a.graph.cursor == it_b.graph.cursor);
}

bool operator!=(const StreamingGraphIterator& it_a,
    const StreamingGraphIterator& it_b)
{
    return !(it_a == it_b);
}
