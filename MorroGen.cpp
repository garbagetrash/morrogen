//============================================================================
// Name        : MorroGen.cpp
// Author      : Kyle Harris
// Version     :
// Copyright   : 
// Description : Morrowind Mod Generator
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include "ModFile.h"

int main(void) {
	puts("Hello World!");

	// Setup the file
	//const char filename[512] = "E:/Games/Steam/steamapps/common/Morrowind/Data Files/ExampleTerrain.ESP";
	//ModFile myFile(filename);

	// Parse the data
	//myFile.parseRawData();

	// Print to a json for verification
	//myFile.printToReadableFile();

	// Generate new mod
	ModFile newMod;
	newMod.generateNewLand(12, 14, 0);

	printf("Main() completed successfully!\n");

	return EXIT_SUCCESS;
}
