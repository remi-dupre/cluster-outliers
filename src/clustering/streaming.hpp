#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../data.hpp"
#include "../space.hpp"
#include "../progressbar.hpp"

#include "offline.hpp"


/**
 * Implementation of a streaming k-clustering with outliers algorithm as defined
 * in section 3 of "Streaming Algorithms for k-Center Clustering with Outliers
 * and with Anonymity" from Richard Matthew McCutchen and Samir Khuller.
 *
 * Time: O(kzn + (kz)² log P) with n = graph.size() and z = nb_outliers and 
 */
Graph streaming_clustering(StreamingGraph& graph, int k, int nb_outliers,
    double alpha = 4, double beta = 8, double eta = 16);
