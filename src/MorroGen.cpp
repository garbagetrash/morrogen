//============================================================================
// Name        : MorroGen.cpp
// Author      : Kyle Harris
// Version     :
// Description : Morrowind Mod Generator
//============================================================================

#include <memory>
#include <iostream>

#include "ModFile.h"
#include "Resources.h"


void parse_existing_file(const char *filename) {
  // Setup the file
  ModFile myFile = ModFile(filename);

  // Parse the data
  myFile.parseRawData();
}

void create_new_terrain_mod() {
  // Generate new mod
  ModFile newMod = ModFile();
  //newMod->generateNewLand("NewLandMod.esp", -48, -14, -30, 5,
  newMod.generateNewLand("NewLandMod.esp", -11, -10, -11, -10,
                          NoiseType::shallow_large_islands,
                          RegionType::ASCADIAN_ISLES, 0);
}

void create_test_mod() {
  // Generate new mod
  ModFile newMod = ModFile();

  newMod.generateTestCell();
}

int main(void) {
  std::cout << "Hello World!" << std::endl;

  // Create a new terrain mod
  create_new_terrain_mod();
  //create_test_mod();

  std::cout << "Main() completed successfully!" << std::endl;

  return 0;
}
