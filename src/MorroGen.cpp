//============================================================================
// Name        : MorroGen.cpp
// Author      : Kyle Harris
// Version     :
// Description : Morrowind Mod Generator
//============================================================================

#include <memory>
#include <iostream>

#include "ModFile.h"


void parse_existing_file(const char *filename) {
  // Setup the file
  std::unique_ptr<ModFile> myFile = std::make_unique<ModFile>(filename);

  // Parse the data
  myFile->parseRawData();

  // Print to a json for verification
  myFile->printToReadableFile();
}

void create_new_mod() {
  // Generate new mod
  std::unique_ptr<ModFile> newMod = std::make_unique<ModFile>();
  newMod->generateNewLand(-15, 5, 0);
}

int main(void) {
  std::cout << "Hello World!" << std::endl;

  // Create a new terrain mod
  create_new_mod();

  std::cout << "Main() completed successfully!" << std::endl;

  return 0;
}
