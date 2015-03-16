# MorroGen
Trying to get procedural mod generation for Morrowind... little ways off from completion.

Currently all I have going is a program that reads in a specified mod file, parses it, and prints that to a json file.  
I've included a library for simplex noise that I plan on using to populate the heightmap data for a new 'LAND' record.
It seems to have the 'random' gradient data hardcoded, so I'll write a generator eventually through the use of a seed 
value so that I can have 32 bits worth of variety.  (I could do 64... but honestly, why?)

## Build Environment
I'm not gonna try to help you to build this code beyond a simple description of how I am implementing it.  I downloaded a GCC 
4.9.2 compiler for Windows somewhere or another and I am using a fresh install of 
[Eclipse](https://eclipse.org/downloads/packages/eclipse-ide-cc-developers/heliossr2) as an IDE.  Obviously I'm using C++.

Super valuable link for understanding Morrowind mod format:
http://www.uesp.net/morrow/tech/mw_esm.txt

Everything on here is helpful as well:
http://www.uesp.net/wiki/Tes3Mod:File_Format

Used the [Enchanted Editor](http://www.uesp.net/wiki/Tes3Mod:Enchanted_Editor) for verification of my parsing.

### Last note
If you look at this source you'll note that I am a shit C++ coder... I'm more comfortable with straight C, and you'll see me
using C style coding a lot despite having the power of C++ 2011 at my hands.  Deal with it.  Better yet, fix it.
