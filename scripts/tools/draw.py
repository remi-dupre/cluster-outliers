"""
Basic functions to draw shapes on the map.
"""
import tools.space as space


def circle(pix, p_center, radius, width, height):
    x, y = space.radial_to_cart(*p_center)
    x, y = int(x * height), int(y * height)

    def in_circle(x, y):
        p_curr = space.cart_to_radial(x / height, y / height)
        return radius - 5e-3 <= space.dist(p_curr, p_center) <= radius

    while not in_circle(x, y):
        y = (y + 1) % height

    drawn = set()
    to_draw = [(x, y)]

    while to_draw:
        x, y = to_draw.pop()

        for dx, dy in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
            px = (x + dx + width) % width
            py = (y + dy + height) % height

            if (px, py) not in drawn and in_circle(px, py):
                pix[px, py] = (86, 135, 41)
                drawn.add((px, py))
                to_draw.append((px, py))
