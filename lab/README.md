# REASONING

## GLOSSARY

- in further reading, for any nested loop

```C
for (i_0 = 0; i_0 < ISIZE; ++i_0) {
  for (i_1 = 0; i_1 < JSIZE; ++i_1) {
    ...
    for (i_n = 0; i_n < JSIZE; ++i_n) {
    }
    ...
  }
}
```

we will consider `I` a vector of indices `i_k`: `I = (i_0, i_1, ..., i_n)`. like so, any capital letter would stand for different vector of indices

## MAIN

- consider nested loop at lines `14 - 18` of `main.c`:

```C
for (i = 0; i < ISIZE; i++) {
  for (j = 0; j < JSIZE; j++) {
    a[i][j] = 10 * i + j;
  }
}
```

it can be made parallel using any number of workers, because there is only one operator working with array

- next, consider nested loop at lines `20 - 24` of `main.c`:

```C
for (i = 0; i < ISIZE; i++) {
  for (j = 0; j < JSIZE; j++) {
    a[i][j] = sin(0.00001 * a[i][j]);
  }
}
```

there are two operations: `S1: a[F(K)] = ...` and `S2: sin(... a[G(J)])`. In first, element of an array is an output value, in second - input.
in order to determine whether or not this loop can be made parallel, we need to analyze following equation:

`F(K) = G(J)`

or, in our case, following system of equations:

```
k_1 = j_1
k_2 = j_2
```

next, we'll calculate dependency distance as follows:

`D = J - K`

in our case `D = {0}`, so once again, loop can be made parallel using any number of workers

- final results can be seen in file `main-parallel.c`

## EXERCISES MAIN

- allowance exercise:
  - directions vector `di = '='` for any `i`
  - there are no dependencies between indices, so we can freely divide loops between processes. knowing that sending a message in MPI is very costly, it's more efficient to paralellize external index
- implementation exercise:
  - see `main-parallel.c`
  - once again, sending a message in MPI is very costly, so we are dividing in blocks by external index
- return exercise:
  - let's try to evaluate possible efficiency `E_n` and acceleration `S_n`
  - for acceleration we have: `S_n = T_1 / T_n`. we will approximate `T_n` as follows: `T_n = T_1 / (T_1 / n + T_gather) = n / (1 + T_gather * n / T_1)`. we will not take in consideration time, spent on allocations and deallocations, since it's negligible
  - for efficiency, let's consider: `E_n = S_n / n = 1 / (1 + T_gather * n / T_1)`
  - see plots in `main.png`

## EX. 1G

- consider nested loop at lines `14 - 18` of `1g.c`:

```C
for (i = 0; i < ISIZE; i++) {
  for (j = 0; j < JSIZE; j++) {
    a[i][j] = 10 * i + j;
  }
}
```

it can be made parallel using any number of workers, because there is only one operator working with array

- next, consider nested loop at lines `20 - 24` of `1g.c`:

```C
for (i = 1; i < ISIZE; i++) {
  for (j = 3; j < JSIZE - 1; j++) {
    a[i][j] = sin(0.00001 * a[i - 1][j - 3]);
  }
}
```

there are two operations: `S1: a[F(K)] = ...` and `S2: sin(... a[G(J)])`. In first, element of an array is an output value, in second - input.
in order to determine whether or not this loop can be made parallel, we need to analyze following equation:

`F(K) = G(J)`

or, in our case, following system of equations:

```
k_1 = j_1 - 1
k_2 = j_2 - 3
```

next, we'll calculate dependency distance as follows:

`D = J - K`

in our case `D = {1, 3}`, so there is true dependency and 2nd layer can be made parallel using no more than 3 workers, while first layer can't be made parallel at all. from here, we can conclude that given program is poorly paralellizable. so we will implement the following approach: we will divide given space into columns by number of workers, and they will each calculate their own slice and then exchange values at borders between them. line by line, thus we will calculate final matrix

- final results can be seen in file `1g-parallel.c`

## EXERCISES EX. 1G

- allowance exercise:
  - directions vector `di = '<'` for any `i`
  - there is an absolute dependence between indices in second loop, and first index can't be made parallel, while second can only be paralellized between 2 workers
- implementation exercise:
  - see `1g-parallel.c`
  - where possible, we will paralellize using external index, since it requires less messges sent, and is easier to write
- return exercise:
  - let's try to evaluate possible efficiency `E_n` and acceleration `S_n`
  - for acceleration we have: `S_n = T_1 / T_n = T_1 / (T_1 / n + 2 * T_send * n) = n / (1 + 2 * n^2 * T_send / T_1)`
  - for efficiency, let's consider: `E_n = S_n / n = 1 / (1 + 2 * n^2 * T_send / T_1)`
  - see plots in `1g.png`
