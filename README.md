# SDsystem

## build

<!-- 1. Download [OGDF::Elderberry](https://www.ogdf.uni-osnabrueck.de/2023/09/14/elderberry/)
2. unzip OGDF and put under this directory -->
1. execute `prebuild.sh` or following commands (See [official reference](https://github.com/ogdf/ogdf/blob/master/doc/build.md))
```bash
cd OGDF
cmake .
make -j8
```

## execution

`./exp.sh`

## grh

```
u_1 w_1 v_1
u_1 w_2 v_2
...
u_k w_m n_n
```

## Note

If you fix `*.hpp`, then you should execute `make clean` in order to clear cache build.

## type

| type | target | Oracle ($\theta_t$) | Alg. | Oracle |
| --- | --- | --- | --- | --- |
| 0 | Edge | $O(n^2)$ | 2 | Single |
| 1 | Edge | $O(n^2)$ | 1 | Single |
| 2 | Edge | $O(n^2)$ | 1 | All |
| 3 | Edge | $O(n)$ | 2 | Single |
| 4 | Edge | $O(n)$ | 1 | Single |
| 5 | Edge | $O(n)$ | 1 | All |
| 6 | Vertex | $O(n^2)$ | 2 | Single |
| 7 | Vertex | $O(n^2)$ | 1 | Single |
| 8 | Vertex | $O(n^2)$ | 1 | All |
| 9 | Vertex | $O(n)$ | 2 | Single |
| 10 | Vertex | $O(n)$ | 1 | Single |
| 11 | Vertex | $O(n)$ | 1 | All |

## parameters

### exp.sh

| nodes: | 10 | 15 | 20 | 25 | 30 |
| --- | --- | --- | --- | --- | --- |

| probability: | 0.1 | 0.3 |
| --- | --- | --- |

| tlime: | 1800s|
| --- | --- |

### exp_delay.sh

| nodes: | 25 | 100 | 500 |
| --- | --- | --- | --- |

| probability: |  0.3 |
| --- | --- |

| tlime: | 900s|
| --- | --- |

## Dependencies

We use [OGDF](https://github.com/ogdf/ogdf/) to use SPQR-tree.
