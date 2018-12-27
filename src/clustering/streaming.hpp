#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <json/json.h>

#include "../data.hpp"
#include "../space.hpp"

#include "offline.hpp"


/**
 * Implementation of a streaming k-clustering with outliers algorithm as defined
 * in section 3 of "Streaming Algorithms for k-Center Clustering with Outliers
 * and with Anonymity" from Richard Matthew McCutchen and Samir Khuller.
 *
 * If the directory 'logs/' exists, outputs result in 'logs/streaming.json'.
 */
Graph streaming_a4_clustering(std::istream& stream, int k, int nb_outliers,
    double alpha = 4, double beta = 8, double eta = 16);

/**
 * Check if the solution saved in 'logs/streaming.json' is correct using
 * parameters and radius saved in this file.
 */
bool streaming_check_soluce(std::istream& stream);
