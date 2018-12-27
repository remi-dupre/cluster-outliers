#pragma once

#include <iostream>
#include <limits>

#include "space.hpp"


Graph read_serialized(std::istream& stream);
Graph read_serialized(std::istream& stream, int count);

/**
 * Convert a point from (lon, lat) angles to cartesian coordinates in
 * [0, 2[ x [0, 1[
 */
inline Point radial_to_cart(Real lon, Real lat);
inline Point radial_to_cart(Point radial);


#include "data.inl"
