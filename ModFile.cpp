/*
 * ModFile.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModFile.h"
#include "json.h"
#include "simplexnoise.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CellRecord.h"
#include "LandRecord.h"

ModFile::ModFile() {
	// TODO Auto-generated constructor stub

}

ModFile::ModFile(const char *fileName) {
	int result = setRawDataFromFile(fileName);
	if (result < 0)
	{
		printf("Failed to create File class!\n");
	}
}

ModFile::~ModFile() {
	// TODO Auto-generated destructor stub
	int result = freeRawDataBuffer();
	if (result < 0)
	{
		printf("Failed to destroy file object!\n");
	}
	result = freeRecords();
	if (result < 0)
	{
		printf("Failed to destroy file object!\n");
	}
}

int ModFile::parseRawData()
{
	printf("Beginning the parseRawData() call...\n");

	char *ptr, *start;
	ptr = this->rawData;
	start = ptr;
	while (*ptr != '\0' && (ptr - start) < this->nBytesRawData)
	{
		// Let record point to our current record
		ModRecord record;
		memset(&record, 0, sizeof(ModRecord));

		// Grab the name first
		strncpy(record.name, ptr, 4);
		printf("name: %s\n", record.name);
		ptr += 4;

		// Then the record size
		memcpy(&(record.size), ptr, 4);
		printf("size: %d\n", record.size);
		ptr += 4;

		// Then the header1
		memcpy(&(record.header1), ptr, 4);
		printf("header1: %d\n", record.header1);
		ptr += 4;

		// And finally the flags
		memcpy(&(record.flags), ptr, 4);
		printf("flags: %d\n", record.flags);
		ptr += 4;

		// Now take the rest of the record and toss it into the rawData blob for that record...
		for (int i = 0; i < record.size; i++)
		{
			record.rawData.push_back(ptr[i]);
		}

		// And tell it to parse that into the sub record data for itself
		record.parseSubRecordsFromRawData();

		// Now just increment the pointer to the next record in the raw data
		this->records.push_back(record);
		ptr += record.size;
	}

	for (unsigned int i = 0; i < this->records.size(); i++)
	{
		if (strcmp(this->records[i].name, "LAND") == 0)
		{
			printf("Found a LAND record!\n");
		}
	}

	printf("Completed parseRawData() call!\n");

	return 1;
}

int ModFile::printToReadableFile()
{
	JsonNode *output = json_mkarray();

	for (unsigned int i = 0; i < this->records.size(); i++)
	{
		// Make a new object for the record, and populate it
		JsonNode *record = json_mkobject();
		this->records[i].encodeToJSON(record);

		// Add the record to the array of vectors
		json_append_element(output, record);
	}

	char *outBuf = json_stringify(output, "\t");
	//printf("%s\n", outBuf);
	int nChars = strlen(outBuf);
	//printf("size: %d\n", nChars);

	// Free up the json array
	json_delete(output);

	const char fileName[32] = "ModFile.json";
	FILE *fid = fopen(fileName, "w");
	fwrite(outBuf, sizeof(char), nChars, fid);
	fclose(fid);

	free(outBuf);

	printf("Wrote to %s successfully!\n", fileName);

	return 1;
}

int ModFile::generateNewLand(int cellX, int cellY, unsigned int seed)
{
	// First create the CELL record(s)
	CellRecord cellRecord;
	cellRecord.setIdString("");
	cellRecord.setGridAndFlags(-14, -3, 2);
	cellRecord.setRegionName("Created Region");

	// Now make the LAND record(s)
	LandRecord landRecord;
	landRecord.setCell(-14, -3);
	landRecord.setUnknown();

	// Create and set the height map
	float offset = 0;
	const float octaves = 4;
	const float persistence = 1;
	const float scale = 1;
	const float loBound = -128;
	const float hiBound = 128;
	signed char heightmap[65][65];
	for (int i = 0; i < 65; i++)
	{
		for (int j = 0; j < 65; j++)
		{
			const float x = floor(i / 65);
			const float y = i % 65;
			float value = scaled_octave_noise_2d(octaves, persistence, scale, loBound, hiBound, x, y);
			heightmap[i][j] = round(value);
		}
	}
	landRecord.setHeightMap(heightmap, offset);

	// Create and set the normal map from the height map data
	LandRecord::normals normalmap;
	// TODO: Algorithm to go from height map to normal map
	landRecord.setNormalMap(normalmap);

	// Set the world map pixels... all to whatever 0 is for now.
	std::vector< std::vector<char> > pixelMap;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			pixelMap[i][j] = 0;
		}
	}
	landRecord.setWorldMapPixels(pixelMap);

	// TODO: Now write landRecord to the active mod file


	return 1;
}

int ModFile::setRawDataFromFile(const char *fileName)
{
	// Setup the file
	FILE *fid = fopen(fileName, "rb");
	if (fid == 0) {
		printf("File not found!\n");
		return -1;
	}

	// Find the file length
	fseek (fid, 0, SEEK_END);
	long lSize = ftell(fid);
	rewind(fid);

	// Allocate memory
	this->rawData = (char *) malloc (sizeof(char) * lSize);
	if (this->rawData == 0) {
		printf("Memory allocation error!\n");
		return -1;
	}

	// Read in the file
	long result = fread(this->rawData, 1, lSize, fid);
	if (result != lSize) {
		printf("Reading error!\n");
		return -1;
	}

	this->nBytesRawData = (int) lSize;

	printf("Successfully populated rawData!\n");

	char recordType[5];
	memset(recordType, 0, 5);
	strncpy(recordType, this->rawData, 4);
	printf("First record type: %s\n", recordType);

	// Tidy up
	fclose(fid);

	return 1;
}

int ModFile::freeRawDataBuffer()
{
	free(this->rawData);
	this->nBytesRawData = 0;

	return 1;
}

int ModFile::freeRecords()
{
	this->records.clear();

	return 1;
}
