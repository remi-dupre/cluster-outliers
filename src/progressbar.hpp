/**
 * Some day I could make use of a module displaying a nice progress bar.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>


class ProgressBar
{
    public:
        ProgressBar(int cur_progress, int max_progress);

    private:
        int cur_progress, max_progress;

    friend std::ostream& operator<<(std::ostream&, const ProgressBar&);
};


std::ostream& operator<<(std::ostream& stream, const ProgressBar& progressbar);
