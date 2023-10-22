#!/bin/sh

cd build && cmake .. && make && ./MorroGen && cp NewLandMod.esp ~/.steam/steam/steamapps/common/Morrowind/Data\ Files/
