/**
 * Usual structures for space manipulation
 */
#pragma once

#include <cmath>
#include <set>
#include <tuple>
#include <vector>


// representation of each coordinate
typedef double Real;

// representation of a 2D point
typedef std::pair<Real, Real> Point;

// representation of a metric graph
typedef std::vector<Point> Graph;


/**
 * Compute the distance between two points
 */
inline Real dist(Point a, Point b);

/**
 * Compute the set of points of the graph withing a given disk.
 */
std::set<size_t> disk(const Graph& graph, Point center, Real radius);


#include "space.inl"
