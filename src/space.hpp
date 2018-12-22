/**
 * Usual structures for space manipulation
 */
#pragma once

#include <cmath>
#include <tuple>
#include <unordered_set>
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
 * Compute the unordered_set of points of the graph withing a given disk.
 *
 * Time: O(n ln n) with n = graph.size()
 */
std::unordered_set<size_t> disk(const Graph& graph, Point center, Real radius);

/**
 * Compute the minimal and maximal distance between two distinct points in the
 * given graph. The minimal distance can't be zero.
 *
 * Time: O(n²)
 */
std::pair<Real, Real> bound_dist(const Graph& graph);

#include "space.inl"
