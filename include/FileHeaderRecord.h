/*
 * FileHeader.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Kyle
 */

#ifndef FILEHEADER_H_
#define FILEHEADER_H_

#include "ModRecord.h"
#include "ModSubRecord.h"

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

class FileHeaderRecord: public ModRecord
{
public:
  int setHedrSubRecord(float version, std::uint32_t unknown,
                       const char *companyName,
                       std::string fileDesc, std::uint32_t nRecords);

  int setMastSubRecord(std::vector< std::string > fileNames);

  int setDataSubRecord(std::vector<std::uint64_t> masterSize);

  int setRecordSize();

  // Write the object to the raw mod data format
  std::string exportToModData();

  size_t exportToModFile(FILE *outFile);

  // Data
  // HEDR stuff
  float Version;
  std::uint32_t Unknown;
  std::string CompanyName;   // 32 bytes...
  std::string EsmFileDesc;   // 256 bytes...
  std::uint32_t NumRecords;  // 48227

  // MAST stuff
  std::vector< std::string > MasterFiles;

  // DATA stuff
  std::vector<std::uint64_t> MasterSize;
};

#endif /* FILEHEADER_H_ */
