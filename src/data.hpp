#pragma once

#include <iostream>
#include <limits>

#include "space.hpp"


Graph read_serialized(std::istream& stream);
Graph read_serialized(std::istream& stream, size_t count);
