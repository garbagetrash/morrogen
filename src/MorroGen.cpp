//============================================================================
// Name        : MorroGen.cpp
// Author      : Kyle Harris
// Version     :
// Description : Morrowind Mod Generator
//============================================================================

#include <memory>
#include <iostream>
#include <stdlib.h>

#include "ModFile.h"


int main(void) {
  std::cout << "Hello World!" << std::endl;

  // Setup the file
  const char filename[512] = "E:/Games/Steam/steamapps/common/Morrowind/Data Files/ExampleTerrain.ESP";
  std::unique_ptr<ModFile> myFile = std::make_unique<ModFile>(filename);

  // Parse the data
  //myFile.parseRawData();

  // Print to a json for verification
  //myFile.printToReadableFile();

  // Generate new mod
  std::unique_ptr<ModFile> newMod = std::make_unique<ModFile>();
  newMod->generateNewLand(12, 14, 0);

  std::cout << "Main() completed successfully!" << std::endl;

  return 0;
}
