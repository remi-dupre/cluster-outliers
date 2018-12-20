inline Real dist(Point a, Point b)
{
    const auto [x_a, y_a] = a;
    const auto [x_b, y_b] = b;

    return sqrt(pow(x_a - x_b, 2) + pow(y_a - y_b, 2));
}
