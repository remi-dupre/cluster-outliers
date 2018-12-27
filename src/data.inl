inline Point radial_to_cart(Real lon, Real lat)
{
    return {(180.0 + lon) / 180.0, (90.0 - lat) / 180.00};
}

inline Point radial_to_cart(Point radial)
{
    const auto [lon, lat] = radial;
    return radial_to_cart(lon, lat);
}
