#!/usr/bin/python

import json
import math
import multiprocessing
import numpy as np
import os
import sys

from PIL import Image, ImageDraw
from glob import glob

import tools.space as space
import tools.draw as draw


tweets_file = 'dataset.txt'

height = 3600
width = 2 * height

#####
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

#####
## Show clusters on the map

try: os.makedirs('maps')
except: pass

for filename in sorted(glob('logs/*.json')):
    print('Read solution from `%s`' % filename)

    loc_img = img.copy()
    pix = loc_img.load()

    with open(filename) as file:
        data = json.load(file)

    radius = data['radius']

    if data['parameters']['log_type'] == 'counter_example':
        radius = data['parameters']['critical_radius']

    for cluster in data['clusters']:
        draw.circle(pix, cluster['center'], radius, width, height)

    output_file = 'maps/' + filename.split('/')[1].split('.')[0] + '.png'
    loc_img.save(output_file)
