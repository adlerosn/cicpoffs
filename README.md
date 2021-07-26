# Case-Insensitive Case-Preserving Overlay FUSE File System
#### cicpoffs

This is a case-insensitive overlay FUSE file system, like [CIOPFS](https://www.brain-dump.org/projects/ciopfs/).

The difference is that:
- This one preserves the original case.
- This one doesn’t have the limitation that “All filenames in the data directory which aren’t all lower case are ignored.”

## Motivation

Run Stardew Valley (linux) with some mods (that are cross-platform in theory, but relies on Windows' case-insensitive file system).

## Preserve inode number

Add `-o use_ino` argument to the commandline otherwise each case combination that points to the same file will get different inodes.

## License

This project uses some modified GPLv2 code on a single file (`fuse_launcher_gpl2.cpp`).

All other files are avaliable as MIT.

Due to this reason, the resulting compiled binary will be GPLv2-licensed unless that single file rewritten.
