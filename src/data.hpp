#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include <json/json.h>

#include "space.hpp"


/**
 * Read raw graph from a stream. It must be ascii-formated, with 3 columns
 * separated with spaces:
 *  - the first column is ignored
 *  - the second and third column contains longitude and latitude of points
 */
Graph read_serialized(std::istream& stream);  // Read the full graph
Graph read_serialized(std::istream& stream, int count);  // Read `count` points


/**
 * Representation of a graph that is not fully loaded into memory, but is still
 * iterable.
 */
class StreamingGraphIterator;
class StreamingGraph
{
    public:
        /**
         * Link the graph to an opened file.
         */
        StreamingGraph(std::ifstream& file);

        /**
         * Reset the cursor to the begining of the graph.
         */
        void reset();

        /**
         * Read next `count` points from the graph and move the cursor after
         * these datas.
         */
        Graph read(int count);

        /**
         * Count the size of the graph, move the pointer to the end of the
         * graph.
         */
        size_t size();

        /* Reset the cursor to the begining and gives an iterator on it */
        StreamingGraphIterator begin();

        /* Gives an unusable iterator representing the end of the graph */
        StreamingGraphIterator end();

        int cursor;  // position we are reading at on the graph

    private:
        std::ifstream& file;  // file containing input graph

    friend class StreamingGraphIterator;
};

/**
 * Input Iterator for streaming graphs
 */
class StreamingGraphIterator
{
    public:
        StreamingGraphIterator(StreamingGraph& graph);
        StreamingGraphIterator operator++(int count);
        StreamingGraphIterator operator++();
        Point operator*() const;
        Point* operator->();

    private:
        bool overflow;
        Point value;
        StreamingGraph& graph;

    friend class StreamingGraph;
    friend bool operator==(const StreamingGraphIterator&,
        const StreamingGraphIterator&);
    friend bool operator!=(const StreamingGraphIterator&,
        const StreamingGraphIterator&);
};

bool operator==(const StreamingGraphIterator&, const StreamingGraphIterator&);
bool operator!=(const StreamingGraphIterator&, const StreamingGraphIterator&);


/**
 * Convert a point from (lon, lat) angles to cartesian coordinates in
 * [0, 2[ x [0, 1[
 */
inline Point radial_to_cart(Real lon, Real lat);
inline Point radial_to_cart(Point radial);


#include "data.inl"
