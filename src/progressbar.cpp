#include "progressbar.hpp"


ProgressBar::ProgressBar(int cur_progress, int max_progress) :
    cur_progress(cur_progress),
    max_progress(max_progress)
{}

std::ostream& operator<<(std::ostream& stream, const ProgressBar& progressbar)
{
    const int width = 15;
    const double load = std::min(
        1.0,
        (double) progressbar.cur_progress / (double) progressbar.max_progress
    );

    const int c_full = int(width * load);
    const int c_part = int((width * load - c_full) * 8);
    const int c_empty = width - c_full - (c_part > 0);

    assert(0 <= c_full && c_full <= width);
    assert(0 <= c_part && c_part <= 7);
    assert(0 <= c_empty && c_empty <= width);

    const std::vector<std::string> part_chars = {" ", "\u258F", "\u258E",
        "\u258D", "\u258C", "\u258B", "\u258A", "\u2588"};
    stream << "▕";

    for (int i = 0 ; i < c_full ; i++)
        stream << "\u2588";

    if (c_part > 0)
        stream << part_chars[c_part];

    for (int i = 0 ; i < c_empty ; i++)
        stream << "\u00B7";

    stream << "▏";
    return stream;
}
