/*
 * LandRecord.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#ifndef LANDRECORD_H_
#define LANDRECORD_H_

#include "ModRecord.h"
#include "ModSubRecord.h"

#include <stdio.h>

#include <cstdint>
#include <string>
#include <vector>

class LandRecord: public ModRecord
{
public:

  typedef struct {
    signed char X;
    signed char Y;
    signed char Z;
  } normals[65][65];

  LandRecord();

  // Set values from subrecords
  int setDataValues(ModSubRecord subRecord);

  // Set Cell location
  int setCell(std::int32_t CellX, std::int32_t CellY);

  // Set the VTEX indices subrecord data
  void setVtexIndices(std::uint16_t indices[16][16]);

  // Set the Unknown to 0x09
  int setUnknown();

  // Set the height map as specified
  int genFlatHeightMap(float offset);
  int genCornerCaseTest(float offset);
  int setHeightMap(std::int32_t heightmap[65][65]);
  int printHeightMap(bool asciiHeightMapActive);
  void asciiHeightToChar(char num, char *buf);

  // Converts the height map from absolute values to differential
  int convertHeightMapToDiff();

  // Set the normal map as specified
  int setNormalMap(normals normalmap);

  // Set the world map pixel array
  int setWorldMapPixels(std::string pixelArray);

  // Set the record size
  int setRecordSize();

  // Write the object to the raw mod data format
  size_t exportToModFile(FILE *fid);

  // Data
  std::int32_t CellX;
  std::int32_t CellY;

  std::uint32_t Unknown;

  normals NormalMap;

  float Unknown1;
  char Unknown2;
  std::int32_t AbsHeightMap[65][65];
  signed char DiffHeightMap[65][65];
  short Unknown3;

  // This is something to do with the world map color palette... a
  // 9x9 grid of bytes, so 81 pixels.  0 is blue.
  std::string WorldMapPixels;

  // Optional VTEX texture indices
  bool UsingVtex = false;
  std::uint16_t Vtex[16][16];
};

#endif /* LANDRECORD_H_ */
