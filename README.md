# Case-Insensitive Case-Preserving Overlay FUSE File System

#### cicpoffs

This is a case-insensitive overlay FUSE file system, like [CIOPFS](https://www.brain-dump.org/projects/ciopfs/).

The difference is that:

- This one preserves the original case.
- This one doesn’t have the limitation that “All filenames in the data directory which aren’t all lower case are ignored.”
- This one has been very poorly ported to work with modern (3.0+) versions of FUSE.

## Motivations

- Run Stardew Valley (linux) with some mods (that are cross-platform in theory, but relies on Windows' case-insensitive file system).
- Run TESV:Skyrim (linux) with some mods (that are cross-platform in theory, but may have issues deriving from Windows' case-insensitive file system and there's no enforceable convention in modding).

## Preserve inode number

Add `-o use_ino` argument to the commandline otherwise each case combination that points to the same file will get different inodes.

## License

GPLv2
