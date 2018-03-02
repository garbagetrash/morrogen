/*
 * FileHeader.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: Kyle
 */
#include "FileHeaderRecord.h"

#include <string.h>

#include <cstdint>
#include <string>
#include <sstream>

template < typename T > std::string to_string(T value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}

int FileHeaderRecord::setHedrSubRecord(float version, std::uint32_t unknown,
                                       const char companyName[32],
                                       std::string fileDesc, std::uint32_t nRecords)
{
  this->Version = version;
  this->Unknown = unknown;
  this->CompanyName.assign(32, 0);
  this->CompanyName = companyName;
  this->CompanyName.assign(256, 0);
  this->EsmFileDesc = fileDesc;
  this->NumRecords = nRecords;

  return 1;
}

int FileHeaderRecord::setMastSubRecord(std::vector< std::string > fileNames)
{
  this->MasterFiles.clear();

  for (unsigned int i = 0; i < fileNames.size(); i++)
  {
    fileNames[i].push_back(0);
  }

  this->MasterFiles = fileNames;

  return 1;
}

int FileHeaderRecord::setDataSubRecord(std::vector<std::uint64_t> masterSize)
{
  this->MasterSize.clear();
  this->MasterSize = masterSize;

  return 1;
}

// Finds the size of the TES3 record (after header 16 bytes)
int FileHeaderRecord::setRecordSize()
{
  long size = 0;

  // HEDR subrecord is always 308 bytes long
  // Float version + long Unknown + 32 bytes Company name + 256 bytes ESM file desc.
  // + long nRecords + 8 bytes subrecord header
  size += 308;

  // Sum size of MAST and DATA subrecords
  for (unsigned int i = 0; i < this->MasterFiles.size(); i++)
  {
    // MAST - string length + subrecord header
    size += this->MasterFiles[i].size() + 8;

    // DATA - long64 + subrecord header
    size += 16;
  }

  this->size = size;

  return 1;
}

std::string FileHeaderRecord::exportToModData()
{
  // TES3 Header Record
  std::string output("TES3");
  output += to_string(this->size);
  output += to_string(this->header1);
  output += to_string(this->flags);

  // HEDR subrecord
  output += "HEDR";
  output += to_string(this->Version);
  output += to_string(this->Unknown);
  output += to_string(this->CompanyName);
  output += to_string(this->EsmFileDesc);
  output += to_string(this->NumRecords);

  // MAST and DATA subrecord(s)
  for (unsigned int i = 0; i < this->MasterFiles.size(); i++)
  {
    output += "MAST";
    output += to_string(this->MasterFiles[i]);
    output += "DATA";
    output += to_string(this->MasterSize[i]);
  }

  return output;
}

size_t FileHeaderRecord::exportToModFile(FILE *outFile)
{
  // TES3 Header Record
  size_t totalSize = 0;
  totalSize += fwrite("TES3", sizeof(char), 4, outFile);
  totalSize += fwrite(&(this->size), sizeof(std::uint32_t), 1, outFile);
  totalSize += fwrite(&(this->header1), sizeof(std::uint32_t), 1, outFile);
  totalSize += fwrite(&(this->flags), sizeof(std::uint32_t), 1, outFile);

  // HEDR subrecord
  totalSize += fwrite("HEDR", sizeof(char), 4, outFile);
  std::uint32_t size = sizeof(float) + 2 * sizeof(std::uint32_t) + sizeof(char) * (32 + 256);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, outFile);
  totalSize += fwrite(&(this->Version), sizeof(float), 1, outFile);
  totalSize += fwrite(&(this->Unknown), sizeof(std::uint32_t), 1, outFile);
  totalSize += fwrite(this->CompanyName.c_str(), sizeof(char), 32, outFile);
  totalSize += fwrite(this->EsmFileDesc.c_str(), sizeof(char), 256, outFile);
  totalSize += fwrite(&(this->NumRecords), sizeof(std::uint32_t), 1, outFile);

  // MAST and DATA subrecord(s)
  for (unsigned int i = 0; i < this->MasterFiles.size(); i++)
  {
    totalSize += fwrite("MAST", sizeof(char), 4, outFile);
    size = this->MasterFiles[i].size() * sizeof(char);
    printf("%d\n", (int) size);
    fflush(stdout);
    totalSize += fwrite(&size, sizeof(std::uint32_t), 1, outFile);
    totalSize += fwrite(this->MasterFiles[i].c_str(), sizeof(char), size, outFile);
    totalSize += fwrite("DATA", sizeof(char), 4, outFile);
    size = sizeof(std::uint64_t);
    totalSize += fwrite(&size, sizeof(std::uint32_t), 1, outFile);
    totalSize += fwrite(&(this->MasterSize[i]), sizeof(std::uint64_t), 1, outFile);
  }

  return totalSize;
}
