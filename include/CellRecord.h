/*
 * CellRecord.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#ifndef CELLRECORD_H_
#define CELLRECORD_H_

#include "ModRecord.h"

#include <string>
#include <vector>

class CellRecord: public ModRecord
{
public:

  // Set the Id string
  int setIdString(std::string name);

  // Set the region name string
  int setRegionName(std::string name);

  // Set the grid and flags (DATA subrecord)
  int setGridAndFlags(int GridX, int GridY, int Flags);

  // Set values from subrecords
  int setDataValues(ModSubRecord subRecord);

  // Set the record size
  int setRecordSize();

  // Export CellRecord to raw mod data format
  std::string exportToModData();
  size_t exportToModFile(FILE *fid);

  // Data
  std::string IdString;

  int Flags;
  int GridX;
  int GridY;

  std::string RegionName;
};

#endif /* CELLRECORD_H_ */
