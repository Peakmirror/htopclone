# htopclone
## Build

**Requirements**
- GCC
- Linux (reads from /proc, won't work on Mac or Windows)

**Compile**
```bash
gcc -Wall -Wextra -o htopclone main.c stats.c memory.c
```

**Run**
```bash
./htopclone
```
