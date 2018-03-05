/*
 * LtexRecord.h
 *
 *  Created on: Mar 04, 2018
 *      Author: Kyle
 */

#ifndef LTEXRECORD_H_
#define LTEXRECORD_H_

#include "ModRecord.h"
#include "ModSubRecord.h"

#include <stdio.h>

#include <cstdint>
#include <string>
#include <vector>

class LtexRecord: public ModRecord
{
public:

  // Set values from subrecords
  int setDataValues(ModSubRecord subRecord);

  // Set the ID
  void setId(std::string id);

  // Set the index
  void setIndex(std::uint32_t index);

  // Set the texture path
  void setTexturePath(std::string texturePath);

  // Set the record size
  int setRecordSize();

  // Write the object to the raw mod data format
  size_t exportToModFile(FILE *fid);

  // Data
  std::uint32_t header1;
  std::uint32_t flags;

  std::string Id;
  std::uint32_t Index;
  std::string TexturePath;
};

#endif /* LTEXRECORD_H_ */
