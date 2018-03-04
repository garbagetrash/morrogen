/*
 * LtexRecord.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#include "LtexRecord.h"

#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

template < typename T > std::string to_string(T value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}

void LtexRecord::setId(std::string id)
{
  this->Id = id;
}

void LtexRecord::setIndex(std::uint32_t index)
{
  this->Index = index;
}

void LtexRecord::setTexturePath(std::string texturePath)
{
  this->TexturePath = texturePath;
}

int LtexRecord::setRecordSize()
{
  std::uint32_t size = 0;

  // NAME - subrecord header + string length Id
  size += this->Id.size() + 8;

  // INTV - subrecord header + uint32_t Index
  size += sizeof(std::uint32_t) + 8;

  // DATA - subrecord header + string length TexturePath
  size += this->TexturePath.size() + 8;

  this->size = size;

  return 1;
}

size_t LtexRecord::exportToModFile(FILE *fid)
{
  size_t totalSize = 0;

  // LTEX record header
  totalSize += fwrite("LTEX", sizeof(char), 4, fid);
  totalSize += fwrite(&(this->size), sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->header1), sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->flags), sizeof(std::uint32_t), 1, fid);

  // NAME subrecord
  totalSize += fwrite("NAME", sizeof(char), 4, fid);
  std::uint32_t size = this->Id.size() * sizeof(char);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(this->Id.c_str(), sizeof(char), this->Id.size(), fid);

  // INTV subrecord
  totalSize += fwrite("INTV", sizeof(char), 4, fid);
  size = sizeof(std::uint32_t);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->Index), sizeof(std::uint32_t), 1, fid);

  // DATA subrecord
  totalSize += fwrite("DATA", sizeof(char), 4, fid);
  size = this->TexturePath.size() * sizeof(char);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(this->TexturePath.c_str(), sizeof(char), this->TexturePath.size(), fid);

  return totalSize;
}
