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
  std::unique_ptr<ModFile> myFile = std::make_unique<ModFile>(filename);

  // Parse the data
  myFile->parseRawData();
}

void create_new_terrain_mod() {
  // Generate new mod
  std::unique_ptr<ModFile> newMod = std::make_unique<ModFile>();
  //newMod->generateNewLand("NewLandMod.esp", -48, -14, -30, 5,
  newMod->generateNewLand("NewLandMod.esp", -20, -10, -20, -10,
                          NoiseType::shallow_large_islands,
                          RegionType::ASCADIAN_ISLES, 0);
}

int main(void) {
  std::cout << "Hello World!" << std::endl;

  // Create a new terrain mod
  create_new_terrain_mod();

  std::cout << "Main() completed successfully!" << std::endl;

  return 0;
}
