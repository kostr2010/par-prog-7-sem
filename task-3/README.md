# HOW TO BUILD

run `make`

# HOW TO RUN

run `run.sh N`, where N - number of threads
`res.txt` is the output file, that contains elements of the modified array

# HOW TO VERIFY

`res_unparallel.txt` is generated using non-parallel algorithm, so `diff -s res_unparallel.txt res.txt` verifies parallel res
