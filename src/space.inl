inline Real dist(Point a, Point b)
{
    const auto [lon1, lat1] = a;
    const auto [lon2, lat2] = b;

    const Real phi1 = lat1 * M_PI / 180;
    const Real phi2 = lat2 * M_PI / 180;

    const Real d_phi = phi2 - phi1;
    const Real dl = (lon2 - lon1) * M_PI / 180;

    const Real k = std::sin(d_phi / 2) * std::sin(d_phi / 2)
        + std::cos(phi1) * std::cos(phi2) * std::sin(dl / 2) * std::sin(dl / 2);
    return 2 * std::atan2(std::sqrt(k), std::sqrt(1-k));
}

template<typename Iterator>
std::pair<Real, Real> bound_dist(Iterator begin, Iterator end)
{
    Real min_dist = +std::numeric_limits<Real>::infinity();
    Real max_dist = -std::numeric_limits<Real>::infinity();

    for (auto p_x = begin ; p_x < end ; p_x++) {
        for (auto p_y = begin ; p_y < end ; p_y ++) {
            const Real curr_dist = dist(*p_x, *p_y);

            if (curr_dist > 0) {
                min_dist = std::min(min_dist, curr_dist);
                max_dist = std::max(max_dist, curr_dist);
            }
        }
    }

    return {min_dist, max_dist};
}
