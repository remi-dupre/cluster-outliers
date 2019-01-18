#pragma once

#include <json/json.h>

#include "data.hpp"
#include "space.hpp"

#include "clustering/offline.hpp"


/**
 * Get informations about a solution as json.
 */
Json::Value soluce_infos(const Graph& graph, int k, int nb_outliers,
    const Graph& clusters, Json::Value params);
