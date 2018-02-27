# MorroGen
Trying to get procedural mod generation for Morrowind... long way off from
completion.

Currently all I have going is a program that reads in a specified mod file,
parses it, and prints that to a json file. I've included a library for simplex
noise that I plan on using to populate the heightmap data for a new 'LAND'
record. It seems to have the 'random' gradient data hardcoded, so I'll write a
generator eventually through the use of a seed value so that I can have 32 bits
worth of variety.  (I could do 64... but honestly, why?)

## Build Environment
Using CMake, so build as normal.

```bash
$ mkdir build && cd build
$ cmake .. && make
```

Super valuable link for understanding Morrowind mod format:
http://www.uesp.net/morrow/tech/mw_esm.txt

Everything on here is helpful as well:
http://www.uesp.net/wiki/Tes3Mod:File_Format

Used the [Enchanted Editor](http://www.uesp.net/wiki/Tes3Mod:Enchanted_Editor) for verification of my parsing.
