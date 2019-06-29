Streaming k-Clusters with Outliers Approximation
================================================

Implement k-cluster with outliers streaming algorithm.

It contains the implementation of two algorithms from these papers:

 - Section 3 from Algorithms for Facility Location Problems with Outliers,
   Moses Charikar, Samir Khuller, David M. Mount, Giri Narasimhan
   [acm](https://dl.acm.org/citation.cfm?id=365555).

 - Algorithm 3.1 from Streaming Algorithms for k-Center Clustering with
   Outliers and with Anonymity, Richard Matthew McCutchen, Samir Khuller
   [springer](https://link.springer.com/chapter/10.1007/978-3-540-85363-3_14).
   Where the requirement to an offline 4-approximation algorithm has been
   replaced with the above algorithm.


Building
--------

#### Dependencies

The core code only requires a **C++17**/**OpenMP** capable compilator and the
library **jsoncpp**.

Building maps and graphs requires **python3** together with libraries
**glob**, **pillow** and **matplotlib**.

#### Compilation

```
https://github.com/remi-dupre/cluster-outliers.git
cd cluster-outliers
make release
```


Usage
-----

#### Running

```bash
# Use every logical threads as a default, but it seems to behave better without
# any hyperthreading going on.
export OMP_NUM_THREADS=4

# Cover the graph saved in dataset.txt using 50 clusters and 10 outliers.
./clustering dataset.txt 50 10
```

The input file must contain one vertex per line, beeing described with three
columns separated with spaces. The first columns is ignored, and the second and
last columns contains longitude and latitude of the point.

#### Testing

```bash
# Build 2D-maps in the directory maps/ from existing logs
scripts/map.py

# Plot a benchmark about existing logs
scripts/plot.py
```


Results
-------

All tests were run on [this dataset](../ressources/dataset.txt?raw=true)
containing the geolocalisation of 1,000,000 tweets around the world.

Here are measures for several parameters:

 - *clusters*: the number of allowed clusters in the solution
 - *outliers*: number of allowed outliers
 - *alpha*: disks growth parameter in the algorithm
 - *cost*: max radius of the solution
 - *min opt*: a lower bound to the optimal solution
 - *max opt*: an upper bound to the optimal solution
 - *time*: run time of the algorithm

| clusters | outliers |  α  |  cost  | min opt | max ratio |   time   |
|:--------:|:--------:|:---:|:------:|:-------:|:---------:|:--------:|
|    20    |    10    | 1.1 | 0.8282 |  0.3125 |   2.6502  |  2.6360s |
|    20    |    50    | 1.1 | 0.6900 |  0.1992 |   3.4637  | 18.3627s |
|    20    |    10    |  4  | 1.4160 |  0.3125 |   4.5312  |  1.9803s |
|    20    |    20    |  4  | 0.8843 |  0.2666 |   3.3169  |  2.4288s |
|    20    |    30    |  4  | 0.8665 |  0.2373 |   3.6514  |  2.4714s |
|    20    |    40    |  4  | 1.3749 |  0.2178 |   6.3136  |  2.6495s |
|    20    |    50    |  4  | 1.2991 |  0.1992 |   6.5210  |  3.1103s |


For parameters clusters = 20, outliers = 50 and α = 1.1 the output looks like:

![Result with k=20 z=50](../ressources/maps/streaming_20-50.png?raw=true)
![Animation with k=20 z=50](../ressources/maps/streaming_20-50.gif?raw=true)


The bound on the optimal is computed by finding (clusters + outliers + 1) disks
that don't intersect, this is how it looks like using previous parameters:

![Counter example with k=20 z=50](../ressources/maps/counter_expl_20-50.png?raw=true)
