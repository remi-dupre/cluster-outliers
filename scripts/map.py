#!/usr/bin/python
import json
import math
import multiprocessing
import numpy as np
import sys

from PIL import Image, ImageDraw

import space
import draw


tweets_file = 'dataset.txt'
clusts_file = 'logs/streaming.json'
output_file = 'map.png'

height = 3600
width = 2 * height


## Create map visualisation of emitted tweets
print('Read tweets from `%s`' % tweets_file)

img = Image.new('F', (width, height), 0)
pix = img.load()

def incr(x, y):
    pix[x, y] += 0.5 * (255 - pix[x, y])

with open(tweets_file) as data:
    for line in data.readlines():
        t, lon, lat = map(float, line.split())
        x, y = space.radial_to_cart(lon, lat)
        incr(int(x * height), int(y * height))

img = img.convert('RGB')


## Show clusters on the map
print('Read solution from `%s`' % clusts_file)
pix = img.load()

with open(clusts_file) as file:
    data = json.load(file)
    radius = data['radius']

    for cluster in data['clusters']:
        draw.circle(pix, cluster['center'], radius, width, height)

# img = Image.fromarray(pix)
img.save(output_file)
