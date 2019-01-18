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
