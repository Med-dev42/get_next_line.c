*This project has been created as part of the 42 curriculum by mlazar.*

# get_next_line

## Description

`get_next_line` is a project of the 42 curriculum. The goal is to write a
function that returns **one line at a time** from a file descriptor, so that
repeated calls let you read a whole text file (or the standard input) line by
line.

```c
char *get_next_line(int fd);
```

- It returns the line that was read, **including the terminating `\n`** (if
  there is one).
- It returns `NULL` when there is nothing left to read, or if an error
  occurred.
- It works with any `BUFFER_SIZE` defined at compile time.

The project teaches **static variables**, **file descriptors**, the `read`
system call and careful **dynamic memory management** (no leaks).

### Bonus

The bonus version handles **several file descriptors at the same time**:
you can alternate calls between different fds without losing the reading
position of each one.

## Instructions

### Files

| Part | Files |
|---|---|
| Mandatory | `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h` |
| Bonus | `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h` |

### Compilation

The buffer size is chosen at compile time with `-D BUFFER_SIZE=n`
(default value: `42`):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl
```

For the bonus:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
```

### Usage

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

The caller is responsible for `free`-ing each returned line, and for opening
and closing the file descriptor.

## Algorithm and data structure

### Data structure

The function keeps what it has read but not yet returned in a
`static char *stash`:

- **Mandatory part:** one `static char *stash`.
- **Bonus part:** an array `static char *stash[FD_MAX]` (with
  `FD_MAX = 1024`), so each file descriptor has its own stash.

A static variable is needed because `read` may fetch more than one line
(`BUFFER_SIZE` bytes at once). The kernel only remembers the reading offset,
not the extra data that was already read, so that data must survive between
calls. A static variable lives in the `.data`/`.bss` segment for the whole
run of the program, which makes this possible.

### Algorithm

Each call to `get_next_line(fd)` does three steps, each in its own function:

1. **`read_to_stash`** : allocate a buffer of `BUFFER_SIZE + 1` bytes and
   call `read` in a loop, appending each chunk to the stash with
   `ft_strjoin`, until the stash contains a `'\n'` or `read` returns `0`
   (end of file). If `read` returns `-1`, everything is freed and `NULL` is
   returned.
2. **`search_line`** : copy from the stash everything up to and including
   the first `'\n'` (or up to the end of the string if there is none). This
   copy is the line returned to the caller.
3. **`clean_stash`** : keep only what comes after the returned line, in a
   new allocated string, and free the old stash. If nothing remains, the
   stash is freed and set to `NULL`.

Invalid input (`fd < 0`, `BUFFER_SIZE <= 0`, and for the bonus
`fd >= FD_MAX`) returns `NULL` immediately.

**Why this design?** It is simple and easy to follow, every function has a
single responsibility (which also fits the 25-line limit of the Norm), and
memory ownership is clear: the stash is always freed when it becomes e						mpty
or when an error happens, and the caller owns the returned line.

**Limitation.** Because the stash is re-allocated with `ft_strjoin` after
each `read`, reading a very large line with a very small `BUFFER_SIZE`
(for example `BUFFER_SIZE=1` on a line of millions of characters) is slow,
since the stash is copied each time. This is acceptable for normal files, and
the code stays much simpler than a growing-buffer approach.

## Resources

### References

- `man 2 read`, `man 2 open`, `man 2 close`
- Articles and tutorials about static variables and the memory layout of a
  C program (`.text`, `.data`, `.bss`, heap, stack, user space, kernel space)
- Documentation about file descriptors and the `read` system call

### Testing

- Own `main` with different files and `BUFFER_SIZE` values (`1`, `42`,
  `10000000`)
- `gnlTester` and `gnl_tester`
- `valgrind` to check for leaks and invalid accesses

### AI usage

AI was used for the following tasks:

- Explaining concepts: static variables and where they are stored in memory,
  file descriptors, `read`/`open`/`close`, `size_t` vs `ssize_t`.
- Reviewing my code for logic errors, memory management and Norm issues.
- Understanding the results of the testers.

I wrote and tested the code myself.
