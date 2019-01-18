/**
 * Usual structures for space manipulation
 */
#pragma once

#include <cassert>
#include <cmath>
#include <limits>
#include <tuple>
#include <unordered_set>
#include <vector>


// representation of each coordinate
typedef double Real;

// representation of a 2D point
typedef std::pair<Real, Real> Point;

// representation of a metric graph
typedef std::vector<Point> Graph;

// hash implementation for points
struct pointhash {
    public:
        template <typename T, typename U>
        std::size_t operator()(const std::pair<T, U> &x) const
        {
            return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
        }
};


/**
 * Compute the distance between two points
 */
inline Real dist(Point a, Point b);

/**
 * Compute the distance of a point to a set.
 */
Real dist(Point a, const std::vector<Point>& s);

/**
 * Compute the distance between two set of points.
 */
Real dist(const std::vector<Point>& s, const std::vector<Point>& p);

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
 * Time: O(n²) with n = graph.size()
 */
std::pair<Real, Real> bound_dist(const Graph& graph);


#include "space.inl"
