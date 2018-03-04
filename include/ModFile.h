/*
 * ModFile.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#ifndef MODFILE_H_
#define MODFILE_H_

#include "CellRecord.h"
#include "FileHeaderRecord.h"
#include "LandRecord.h"
#include "LtexRecord.h"
#include "ModRecord.h"

#include <string>
#include <vector>

enum class NoiseType {
  shallow_large_islands,
  mountains_with_lakes,
};

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
  int generateNewLand(const char *filename, int cellXstart, int cellXstop,
                      int cellYstart, int cellYstop, NoiseType type,
                      unsigned int seed);

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

  FileHeaderRecord generateHeader(const char *filename,
                                  std::uint32_t nRecords);

  CellRecord generateCellRecord(const char *id, int cellX, int cellY,
                                int flags, std::string region_name,
                                NoiseType type);

  LandRecord generateLandRecord(int cellX, int cellY, NoiseType type);

  LtexRecord generateLtexRecord(const LtexPair &pair, std::uint32_t index);

  std::vector<LandRecord> generateLandRecords(int cellXstart,
                                              int cellXstop,
                                              int cellYstart,
                                              int cellYstop,
                                              NoiseType type);

  std::vector<CellRecord> generateCellRecords(int cellXstart,
                                              int cellXstop,
                                              int cellYstart,
                                              int cellYstop,
                                              int flags,
                                              std::string region_name,
                                              NoiseType type);

  std::vector<LtexRecord> generateLtexRecords(std::vector<LtexPair> textureSet);
};

#endif /* MODFILE_H_ */
