"""
Basic functions to manipulate space coordinates.
"""
import math


def radial_to_cart(lon, lat):
    x =  (180 + lon) / 180
    y =  (90 - lat) / 180
    return x, y

def cart_to_radial(x, y):
    lon = (x * 180) - 180
    lat = 90 - (y * 180)
    return lon, lat

def dist(p1, p2):
    lon1, lat1 = p1
    lon2, lat2 = p2

    phi1 = lat1 * math.pi / 180
    phi2 = lat2 * math.pi / 180

    d_phi = phi2 - phi1
    dl = (lon2 - lon1) * math.pi / 180

    a = math.sin(d_phi / 2) * math.sin(d_phi / 2) + math.cos(phi1) * math.cos(phi2) \
        * math.sin(dl / 2) * math.sin(dl / 2)
    return 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
