# MorroGen
Trying to get procedural mod generation for Morrowind... long way off from
completion.

So far I've gotten exterior heightmap generation working with a couple
different types of noise, and I also implemented a basic uniformly distributed
tree placement algorithm.  Right now I just have the various Bitter Coast trees
with random rotations.

I need to adjust the tree height offsets on a tree by tree basis to avoid
floaters.  I also need to start looking at how the game handles ground textures
so I can start to intelligently set that based on height.  After those two
updates I want to look at beginning to use more advanced procedural algorithms
to give the terrain a more sensible global structure... in particular I'm
interested in trying to impelement
[this paper](https://arches.liris.cnrs.fr/publications/articles/SIGGRAPH2013_PCG_Terrain.pdf).
We'll see how it goes, but that'd be an excellent start to a more realistic
looking landmass.  After that I need to look into supporting more clutter in
the same way that I'm doing the Bitter Coast trees.  Then I can look at adding
enemies to the terrain.

Long term I also want to generate villages and towns as well, but I don't know
as much about those sorts of algorithms.  I have a couple references to study,
however, starting with
[this survey](http://www.citygen.net/files/Procedural_City_Generation_Survey.pdf).
One advantage I have is that I currently have no intentions of getting into
procedural building modeling, so it would only be the pathing and building
placement algorithms that I'd have to think about, using the vanilla Morrowind
assets.

Finally, procedural dungeons.  I figure there ought to be plenty of papers and
blogposts on how to go about generating those, but I haven't looked into any of
the specifics yet.

The trickiest part about the last two paragraphs is that I'd have to start
supporting interior cell generation as well... I don't know anything about how
those are structured in the .esp mod format, and I also don't know anything
about how the game handles NPCs, their AI, their pathing, their dialogue,
ect...  which is why those two longer term goals are somewhat long shots.


## Build Environment
Using CMake, so build as normal.  Requires C++14 support from compiler, so
think gcc 5+.

```bash
$ mkdir build && cd build
$ cmake .. && make
```

Super valuable link for understanding Morrowind mod format:
http://www.uesp.net/morrow/tech/mw_esm.txt
http://www.mwmythicmods.com/tutorials/MorrowindESPFormat.html


Everything on here is helpful as well:
http://www.uesp.net/wiki/Tes3Mod:File_Format

Used the [Enchanted Editor](http://www.uesp.net/wiki/Tes3Mod:Enchanted_Editor) for verification of my parsing.

![Terrain](screenshots/terrain.png?raw=true)
![Trees](screenshots/trees.png?raw=true)
![Mountains](screenshots/mountains.png?raw=true)
![Islands](screenshots/islands.png?raw=true)
