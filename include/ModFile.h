/*
 * ModFile.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#ifndef MODFILE_H_
#define MODFILE_H_

#include "ModRecord.h"

#include <string>
#include <vector>

class ModFile {
public:
  ModFile();
  ModFile(const char *fileName);
  virtual ~ModFile();

  // Populates the records given the raw data.
  int parseRawData();

  // Prints the mod file to a human readable file... similar to JSON
  // From my sample set of 1 this seems to blow up the file size by
  // about a factor of 10... so just keep that in mind.
  int printToReadableFile();

  // Populate the raw data buffer from a given file.
  int setRawDataFromFile(const char *fileName);

  // Makes a new land cell at the given x, y location provide.
  int generateNewLand(int cellX, int cellY, unsigned int seed);

  // Create a new blank header file mod
  int generateHeader(const char *filename);

  // Frees up the raw data buffer, and sets the size to 0.
  int freeRawDataBuffer();

  // Frees up the records, and sets the size to 0.
  int freeRecords();

  // Storage place for raw data file
  char *rawData;
  int nBytesRawData;

  // File is composed entirely of records
  std::vector<ModRecord> records;

private:
  int writeStringToFile(const char *fileName, std::string input);
};

#endif /* MODFILE_H_ */
