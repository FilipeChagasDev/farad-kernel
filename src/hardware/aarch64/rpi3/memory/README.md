# Raspberry Pi 3 memory management

In the RPI3 implementation, the memory pages are 64KiB.

The space that starts between the first 64KiB block after & _end and ends at address 0x3C000000 is used as a dynamic allocation space for physical pages (the so-called physical heap).

The aarch64 architecture implements the complete pagetable as a table tree. When the pagetable is allocated, only the level 2 table is allocated. Level 3 tables are only allocated when necessary.

Each table in the tree occupies an entire 64KiB physical page. It is necessary to save them.
