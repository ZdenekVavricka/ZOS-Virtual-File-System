# I-Node Based Virtual File System (VFS)

**Author:** Zdeněk Vavřička
**Date:** February 2022
**Course:** Fundamentals of Operating Systems (KIV/ZOS)

---

## Overview
This project is a **Virtual File System (VFS)** implemented in **C**, simulating a simplified Unix-like filesystem structure based on **i-nodes**. The system operates within a single binary file on the host OS, managing its own internal structures including superblocks, bitmaps, and data blocks.

## System Architecture
The filesystem is organized into several logical sections to manage data and metadata efficiently:
- **Super-block:** Contains critical filesystem metadata such as disk size, cluster size, and addresses of bitmaps and data blocks.
- **I-Node & Block Bitmaps:** Track the allocation status of i-nodes and data blocks using bit-level representation.
- **I-Nodes:** Store file metadata, including file type, size, and pointers to data blocks.
- **Data Blocks:** Fixed-size units (256B) used for storing raw file data, directory structures, and indirect pointers.
- **Addressing:** Supports direct pointers, as well as **1st-order** and **2nd-order indirect pointers** to handle larger files within the 2GB system limit.

## Key Features
- **File & Directory Management:** Full support for standard operations like creating, moving, and deleting files and directories.
- **Symbolic Links:** Implementation of the `slink` command to create shortcuts to existing files.
- **Host OS Integration:** Commands for importing (`incp`) and exporting (`outcp`) files between the local disk and the virtual environment.
- **Batch Processing:** Ability to load and execute a sequence of commands from an external script file.
- **Disk Formatting:** Dynamic initialization of the virtual drive with a user-specified size (e.g., `format 600MB`).

## Supported Commands
The VFS provides a comprehensive CLI for user interaction:
- `ls`, `cd`, `pwd`: Navigate and list directory contents.
- `mkdir`, `rmdir`: Manage directory structures.
- `cp`, `mv`, `rm`: Copy, move, or delete internal files.
- `cat`: Display file content.
- `info`: Show detailed metadata and cluster allocation for a specific file or directory.
- `load`: Execute a batch of commands from a text file.

## Technical Details
- **Implementation:** Procedural C code organized into modular components (`vfs.c`, `commands.c`, `utilities.c`).
- **Constraints:** Maximum filename length is 12 characters (11 + null terminator).
- **Storage Limits:** Supports virtual disk sizes from 20KB up to 2GB.
- **Platform:** Developed and verified on Windows.

## Compilation and Usage
The project includes a `Makefile` for automated building using `gcc`.

### Build:
```bash
make