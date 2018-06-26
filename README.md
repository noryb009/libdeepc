# Deep C

Deep C (libdeepc) is an experimental C standard library. I started this project
to learn more about how the C standard library is implemented (and therefore
how most standard libraries are implemented). I plan on writing blog posts to
share my findings.

While the library has a good portion of the C standard library (including
`threads.h`), it is still missing critical pieces, and nothing has been
optimized yet.

# DO NOT USE IN PRODUCTION

There are plenty of bugs in this library. Many functions are not yet
implemented. For example, `free` doesn't do anything (yet).
