# Syscall Logger

A small Linux project that traces and logs system calls made by a target executable using `ptrace`. It captures each syscall, whether it succeeded or failed, and stores the result in JSON format. A Python analysis script (`log_analysis/`) then visualizes syscall frequency, categories, error rates, and common failure types.

## Features

- Monitors syscalls using `PTRACE_SYSCALL`
- Captures syscall names, return values, and success/failure status
- Generates a log file (`/tmp/syscalls_log.json`)
- Includes Python scripts using `pandas` and `matplotlib` for:
  - Frequency bar charts
  - Success vs. failure pie chart
  - Categorized syscall distribution
  - Most common error types

## What I Learned

This project taught me:
- How Linux processes interact with the kernel through system calls
- How to use `ptrace` to intercept syscall entry and exit points
- That not all signals from `waitpid` are syscall-related; I had to filter only syscall stops using `PTRACE_O_TRACESYSGOOD` and bit masking
- System calls can fail for many reasons (e.g., `ENOENT`), and return values must be interpreted carefully


## Types of Syscalls Observed

- **File I/O**: `read`, `write`, `openat`, `close`, `access`
- **Memory**: `mmap`, `brk`
- **Process**: `execve`, `getpid`
- **Threading/Sync**: `futex`, `rseq`
- Many syscalls succeed, but errors like `ENOENT` (file not found) are common and important to detect.

## Future Improvements

- Add support for capturing **all signals** and generating stats on them
- Include timestamps or durations between syscalls
- Group syscalls by originating thread or subprocess

---

**Note**: This is a small educational project for learning about syscall tracing and low-level Linux internals.
