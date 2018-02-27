/*
 * ModFile.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModFile.h"
#include "json.h"
#include "simplexnoise.h"

#include <fstream>
#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FileHeaderRecord.h"
#include "CellRecord.h"
#include "LandRecord.h"

ModFile::ModFile() {
  // TODO Auto-generated constructor stub

}

ModFile::ModFile(const char *fileName) {
  int result = setRawDataFromFile(fileName);
  if (result < 0)
  {
    std::cout << "Failed to create File class!" << std::endl;
  }
}

ModFile::~ModFile() {
  // TODO Auto-generated destructor stub
  int result = freeRawDataBuffer();
  if (result < 0)
  {
    std::cout << "Failed to destroy file object!" << std::endl;
  }
  result = freeRecords();
  if (result < 0)
  {
    std::cout << "Failed to destroy file object!" << std::endl;
  }
}

int ModFile::parseRawData()
{
  std::cout << "Beginning the parseRawData() call..." << std::endl;

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
    std::cout << "name: " << record.name << std::endl;
    ptr += 4;

    // Then the record size
    memcpy(&(record.size), ptr, 4);
    std::cout << "size: " << record.size << std::endl;
    ptr += 4;

    // Then the header1
    memcpy(&(record.header1), ptr, 4);
    std::cout << "header1: " << record.header1 << std::endl;
    ptr += 4;

    // And finally the flags
    memcpy(&(record.flags), ptr, 4);
    std::cout << "flags: " << record.flags << std::endl;
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
      std::cout << "Found a LAND record!" << std::endl;
    }
  }

  std::cout << "Completed parseRawData() call!" << std::endl;

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
  int nChars = strlen(outBuf);

  // Free up the json array
  json_delete(output);

  const char fileName[32] = "ModFile.json";
  FILE *fid = fopen(fileName, "w");
  fwrite(outBuf, sizeof(char), nChars, fid);
  fclose(fid);

  free(outBuf);

  std::cout << "Wrote to " << fileName << " successfully!" << std::endl;

  return 1;
}

int ModFile::generateNewLand(int cellX, int cellY, unsigned int seed)
{
  // BUT EVEN BEFORE THAT create the TES3 file header
  FileHeaderRecord header;
  std::string asdf;
  asdf.assign (256, 0);
  header.setHedrSubRecord(1.3, 1, "asdf", asdf, 3);
  std::vector< std::string > masterEsms;
  masterEsms.push_back("Morrowind.esm");
  masterEsms.push_back("Bloodmoon.esm");
  masterEsms.push_back("Tribunal.esm");
  header.setMastSubRecord(masterEsms);
  std::vector<long long> masterSizes;
  masterSizes.push_back(79837557);
  masterSizes.push_back(9631798);
  masterSizes.push_back(4565686);
  header.setDataSubRecord(masterSizes);
  header.setRecordSize();
  //std::string outputFile = header.exportToModData();
  FILE *fid = fopen("NewMod.esp", "wb");
  size_t totalSize = header.exportToModFile(fid);

  // First create the CELL record(s)
  CellRecord cellRecord;
  cellRecord.setIdString("");
  cellRecord.setGridAndFlags(-14, 2, 2);
  cellRecord.setRegionName(std::string("Bitter Coast Region"));
  cellRecord.setRecordSize();

  totalSize += cellRecord.exportToModFile(fid);

  // Now the 2nd one
  CellRecord cellRecord2;
  cellRecord2.setIdString("");
  cellRecord2.setGridAndFlags(-14, 3, 2);
  //cellRecord2.setRegionName(std::string());
  cellRecord2.setRecordSize();

  totalSize += cellRecord2.exportToModFile(fid);

  //outputFile += cellRecord.exportToModData();

  // Now make the LAND record(s)
  LandRecord landRecord;
  landRecord.setCell(-14, 2);
  landRecord.setUnknown();

  // Create and set the height map
  float offset = 5;
  const float octaves = 7;
  const float persistence = 0.5;
  const float scale = 1;
  const float loBound = 0;
  const float hiBound = 128;
  signed char heightmap[65][65];
  for (int i = 0; i < 65; i++)
  {
    for (int j = 0; j < 65; j++)
    {
      const float x = i / 64.0;
      const float y = j / 64.0;
      float value = scaled_octave_noise_2d(octaves, persistence, scale, loBound, hiBound, x, y);
      heightmap[i][j] = round(value);
    }
  }
  // TODO: fix this so that heightmap is converted from actual heights to differential heights.
  landRecord.setHeightMap(heightmap, offset);
  //landRecord.genFlatHeightMap(offset);
  landRecord.printHeightMap(false);

  // Create and set the normal map from the height map data
  LandRecord::normals normalmap;
  typedef struct {
    double X;
    double Y;
    double Z;
  } vect3;

  for (unsigned int i = 0; i < 65; i++)
  {
    for (unsigned int j = 0; j < 65; j++)
    {
      // t vect has x = 1, y = 0, always.
      double tz = heightmap[i < 64 ? i+1 : i][j] - heightmap[i > 0 ? i-1 : i][j];
      if (i == 0 || i == 64)
      {
        tz = tz * 2;
      }

      // beta vect has x = 0, y = 1, always.
      double betaz = heightmap[i][j < 64 ? j+1 : j] - heightmap[i][j > 0 ? j-1 : j];
      if (j == 0 || j == 64)
      {
        betaz = betaz * 2;
      }

      // Cross product of t and beta to get normal vector
      double factor = sqrt(tz*tz + betaz*betaz + 1);
      normalmap[i][j].X = round(127 * -tz / factor);
      normalmap[i][j].Y = round(127 * betaz / factor);
      normalmap[i][j].Z = round(127 * 1 / factor);
    }
  }
  landRecord.setNormalMap(normalmap);
  landRecord.convertHeightMapToDiff();
  landRecord.printHeightMap(false);

  // Set the world map pixels... all to whatever 0 is for now.
  std::string pixelMap;
  pixelMap.assign(81, 0);
  landRecord.setWorldMapPixels(pixelMap);
  landRecord.setRecordSize();

  // Now write landRecord to the active mod file
  totalSize += landRecord.exportToModFile(fid);

  fclose(fid);
  std::cout << "Wrote " << totalSize << " bytes!" << std::endl;

  return 1;
}

int ModFile::setRawDataFromFile(const char *fileName)
{
  // Setup the file
  FILE *fid = fopen(fileName, "rb");
  if (fid == 0) {
    std::cout << "File not found!" << std::endl;
    return -1;
  }

  // Find the file length
  fseek (fid, 0, SEEK_END);
  long lSize = ftell(fid);
  rewind(fid);

  // Allocate memory
  this->rawData = (char *) malloc (sizeof(char) * lSize);
  if (this->rawData == 0) {
    std::cout << "Memory allocation error!" << std::endl;
    return -1;
  }

  // Read in the file
  long result = fread(this->rawData, 1, lSize, fid);
  if (result != lSize) {
    std::cout << "Reading error!" << std::endl;
    return -1;
  }

  this->nBytesRawData = (int) lSize;

  std::cout << "Successfully populated rawData!" << std::endl;

  char recordType[5];
  memset(recordType, 0, 5);
  strncpy(recordType, this->rawData, 4);
  std::cout << "First record type: " << recordType << std::endl;

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

////////////////////////////////////////////////////////////////////
// Private Function Definitions
////////////////////////////////////////////////////////////////////

int ModFile::writeStringToFile(const char *fileName, std::string input)
{
  FILE *fid = fopen(fileName, "wb");
  fwrite(input.c_str(), sizeof(char), input.size(), fid);
  fclose(fid);

  return 1;
}
