/*
 * ModFile.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModFile.h"
#include "json.h"
#include "simplexnoise.h"

#include <cstdint>
#include <cstdlib>
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

std::vector<CellRecord> ModFile::generateCellRecords(int cellXstart,
                                                     int cellXstop,
                                                     int cellYstart,
                                                     int cellYstop,
                                                     int flags,
                                                     std::string region_name)
{
  std::vector<CellRecord> cellRecords;
  int lenX = abs(cellXstop - cellXstart) + 1;
  int lenY = abs(cellYstop - cellYstart) + 1;

  int lowX = cellXstart;
  if (cellXstart > cellXstop) {
    lowX = cellXstop;
  }

  int lowY = cellYstart;
  if (cellYstart > cellYstop) {
    lowY = cellYstop;
  }

  for (int i = 0; i < lenX; i++)
  {
    for (int j = 0; j < lenY; j++)
    {
      CellRecord cellRecord = generateCellRecord("", lowX + i, lowY + j, flags,
                                                 region_name);
      cellRecords.push_back(cellRecord);
    }
  }

  return cellRecords;
}

CellRecord ModFile::generateCellRecord(const char *id, int cellX, int cellY,
                                       int flags, std::string region_name)
{
  CellRecord cellRecord;
  cellRecord.setIdString(id);
  cellRecord.setGridAndFlags(cellX, cellY, flags);
  cellRecord.setRegionName(region_name);
  cellRecord.setRecordSize();

  return cellRecord;
}

std::vector<LandRecord> ModFile::generateLandRecords(int cellXstart,
                                                     int cellXstop,
                                                     int cellYstart,
                                                     int cellYstop)
{
  std::vector<LandRecord> landRecords;
  int lenX = abs(cellXstop - cellXstart) + 1;
  int lenY = abs(cellYstop - cellYstart) + 1;

  int lowX = cellXstart;
  if (cellXstart > cellXstop) {
    lowX = cellXstop;
  }

  int lowY = cellYstart;
  if (cellYstart > cellYstop) {
    lowY = cellYstop;
  }

  for (int i = 0; i < lenX; i++)
  {
    for (int j = 0; j < lenY; j++)
    {
      LandRecord landRecord = generateLandRecord(lowX + i, lowY + j);
      landRecords.push_back(landRecord);
    }
  }

  return landRecords;
}

LandRecord ModFile::generateLandRecord(int cellX, int cellY)
{
  LandRecord landRecord;
  landRecord.setCell(cellX, cellY);
  landRecord.setUnknown();

  // Create and set the height map
  const float octaves = 7;
  const float persistence = 0.5;
  const float scale = 1;
  const float loBound = 0;
  const float hiBound = 128;
  signed char heightmap[65][65];
  for (int i = 0; i < 65; i++)
  {
    const float x = i / 64.0 + cellY;
    for (int j = 0; j < 65; j++)
    {
      const float y = j / 64.0 + cellX;
      float value = scaled_octave_noise_2d(octaves, persistence, scale, loBound, hiBound, x, y);
      heightmap[i][j] = round(value);
    }
  }
  // TODO: fix this so that heightmap is converted from actual heights to differential heights.
  landRecord.setHeightMap(heightmap);
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

  return landRecord;
}

int ModFile::generateNewLand(const char *filename, int cellX, int cellY,
                             unsigned int seed)
{
  // BUT EVEN BEFORE THAT create the TES3 file header
  FileHeaderRecord header = generateHeader(filename);
  FILE *fid = fopen(filename, "wb");
  size_t totalSize = header.exportToModFile(fid);

  // First create the CELL record(s)
  std::vector<CellRecord> cellRecords = generateCellRecords(-15, -14, 2, 3, 2,
                                          std::string("Bitter Coast Region"));

  for (unsigned int i=0; i < cellRecords.size(); i++) {
    totalSize += cellRecords[i].exportToModFile(fid);
  }

  // Now make the LAND record(s)
  std::vector<LandRecord> landRecords = generateLandRecords(-18, -14, 0, 3);

  // Now write landRecord to the active mod file
  for (unsigned int i=0; i < landRecords.size(); i++) {
    totalSize += landRecords[i].exportToModFile(fid);
  }

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

FileHeaderRecord ModFile::generateHeader(const char *filename)
{
  FileHeaderRecord header;
  std::string asdf;
  asdf.assign (256, 0);
  header.setHedrSubRecord(1.3, 1, "asdf", asdf, 3);
  std::vector< std::string > masterEsms;
  masterEsms.push_back("Morrowind.esm");
  masterEsms.push_back("Bloodmoon.esm");
  masterEsms.push_back("Tribunal.esm");
  header.setMastSubRecord(masterEsms);
  std::vector<std::uint64_t> masterSizes;
  masterSizes.push_back(79837557);
  masterSizes.push_back(9631798);
  masterSizes.push_back(4565686);
  header.setDataSubRecord(masterSizes);
  header.setRecordSize();

  return header;
}
