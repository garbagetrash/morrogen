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

struct PosRotData {
  float posX;
  float posY;
  float posZ;
  float rotX;
  float rotY;
  float rotZ;
};

struct ObjectReference {
  std::string name;
  PosRotData data;
};

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

  // Add object to cell record
  void addObjectToCell(std::string name, PosRotData data);

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
  std::vector<ObjectReference> objectReferences;
};

#endif /* CELLRECORD_H_ */
