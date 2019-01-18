#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>


/**
 * A serializable progress bar representation.
 */
class ProgressBar
{
    public:
        /**
         * Create a progressbar with 0 <= cur_progress <= max_progress.
         */
        ProgressBar(int cur_progress, int max_progress);

    private:
        int cur_progress, max_progress;  // state of the progressbar

    friend std::ostream& operator<<(std::ostream&, const ProgressBar&);
};


std::ostream& operator<<(std::ostream& stream, const ProgressBar& progressbar);
