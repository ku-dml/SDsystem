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

1. If you fix `*.hpp`, then you should execute `make clean` in order to clear cache build.
2. If you manage large instance ($n>15000$), you should change parameter `N` in `lib/lib.hpp` to large number ([here](https://github.com/ku-dml/SDsystem/blob/main/src/lib/lib.hpp#L18)).

## type

| type | target | Oracle ($\theta_t$) | Alg. | Oracle |
| --- | --- | --- | --- | --- |
| 0 | Edge | $O(n^2)$ | 2 | One-by-one |
| 1 | Edge | $O(n^2)$ | 1 | One-by-one |
| 2 | Edge | $O(n^2)$ | 1 | All-at-once |
| 3 | Edge | $O(n)$ | 2 | One-by-one |
| 4 | Edge | $O(n)$ | 1 | One-by-one |
| 5 | Edge | $O(n)$ | 1 | All-at-once |
| 6 | Vertex | $O(n^2)$ | 2 | One-by-one |
| 7 | Vertex | $O(n^2)$ | 1 | One-by-one |
| 8 | Vertex | $O(n^2)$ | 1 | All-at-once |
| 9 | Vertex | $O(n)$ | 2 | One-by-one |
| 10 | Vertex | $O(n)$ | 1 | One-by-one |
| 11 | Vertex | $O(n)$ | 1 | All-at-once |

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
