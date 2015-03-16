//============================================================================
// Name        : mwScript.cpp
// Author      : Kyle Harris
// Version     :
// Copyright   : 
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include "ModFile.h"

int main(void) {
	puts("Hello World!");

	// Setup the file
	const char filename[512] = "E:/Games/Steam/steamapps/common/Morrowind/Data Files/ExampleTerrain.ESP";
	ModFile *myFile = new ModFile(filename);

	// Parse the data
	myFile->parseRawData();

	// Print to a json for verification
	myFile->printToReadableFile();

	// Delete the file when done (free up memory)
	delete myFile;

	printf("Main() completed successfully!\n");

	return EXIT_SUCCESS;
}
