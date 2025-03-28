# Operating Systems Course 🖥️

This repository contains C++ code written as part of a university-level Operating Systems course. The programs interact with the Linux `/proc` filesystem, system calls, and other low-level OS concepts.

## 📂 Contents

| File         | Description |
|--------------|-------------|
| `pstree.cpp` | Outputs a process tree in JSON format by reading from `/proc/[pid]/status`. Shows parent-child relationships between processes. |
| `statx.cpp`  | Displays file permission bits (rwx) and metadata (UID, GID, size) using the `statx` system call. |
| `uname.cpp`  | Uses `uname()` to display system info like OS name, version, and hostname. |
| `killall.cpp`| Searches `/proc` for processes matching a name and forcefully kills them using `kill()`. |
| `ps.cpp`     | Mimics a simplified `ps` command. Lists PID, executable path, working directory, base memory address, state, and command line arguments of running processes. |

## 🔧 Compilation

You can compile each program using `g++`. For example:

```bash
g++ -o pstree pstree.cpp
g++ -o statx statx.cpp
g++ -o uname uname.cpp
g++ -o killall killall.cpp
g++ -o ps ps.cpp
