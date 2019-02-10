#!/usr/bin/python

import json
import matplotlib
import matplotlib.pyplot as plt
from glob import glob


matplotlib.rc('text', usetex=True)
matplotlib.rcParams.update({'font.size': 20})


time_plot = []
ratio_plot = []

#####
# Collect datas

for filename in glob('logs/*.json'):
    with open(filename) as file:
        data = json.load(file)

    if data['parameters']['log_type'] != 'streaming':
        continue

    outliers = data['parameters']['outliers']
    time = data['parameters']['running_time']
    ratio = data['parameters']['approx_ratio']

    time_plot.append((outliers, time))
    ratio_plot.append((outliers, ratio))

#####
# Plot datas

time_plot.sort()
ratio_plot.sort()


fig, ax1 = plt.subplots()

color = 'tab:red'
ax1.set_xlabel('outliers')
ax1.set_ylabel('time (s)', color=color)
ax1.plot(*zip(*time_plot), color=color)
ax1.tick_params(axis='y', labelcolor=color)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()


fig, ax1 = plt.subplots()

color = 'tab:blue'
ax1.set_xlabel('outliers')
ax1.set_ylabel('approximation bound ratio', color=color)
ax1.plot(*zip(*ratio_plot), color=color)
ax1.tick_params(axis='y', labelcolor=color)


fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()
