/*
 * LandRecord.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#include "LandRecord.h"

#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

LandRecord::LandRecord()
{
  this->CellX = 0;
  this->CellY = 0;
  this->Unknown = 0;
  //memset(&(this->NormalMap), 0, sizeof(normals));
  this->Unknown1 = 0.0f;
  this->Unknown2 = 0;
  memset(&(this->AbsHeightMap), 0, 65 * 65 * sizeof(std::int32_t));
  memset(&(this->DiffHeightMap), 0, 65 * 65 * sizeof(signed char));
  this->Unknown3 = 0;
  this->WorldMapPixels = std::string("");
  memset(&(this->Vtex), 0, 16 * 16 * sizeof(std::uint16_t));
}

template < typename T > std::string to_string(T value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}

int LandRecord::setUnknown()
{
  this->Unknown = 0x09;

  return 1;
}

int LandRecord::setCell(std::int32_t CellX, std::int32_t CellY)
{
  this->CellX = CellX;
  this->CellY = CellY;

  return 1;
}

void LandRecord::setVtexIndices(uint16_t indices[16][16])
{
  // First we convert from sanity to madness
  uint16_t madness[16][16];
  for (size_t x = 0; x < 16; x++) {
    for (size_t y = 0; y < 16; y++) {
      size_t macro_row = y / 4;
      size_t macro_col = x / 4;
      size_t inner = macro_col + 4 * macro_row;

      size_t micro_row = y % 4;
      size_t micro_col = x % 4;
      size_t outer = micro_col + 4 * micro_row;

      madness[inner][outer] = indices[x][y];
    }
  }

  // Now we write out the madness
  memcpy(this->Vtex, madness, (16 * 16) * sizeof(uint16_t));
  this->UsingVtex = true;
}

int LandRecord::genFlatHeightMap(float offset)
{
  std::int32_t temp[65][65];
  memset(temp, 0, 65*65*sizeof(std::int32_t));

  return setHeightMap(temp);
}

int LandRecord::genCornerCaseTest(float offset)
{
  std::int32_t temp[65][65];
  memset(temp, 0, 65*65*sizeof(std::int32_t));

  temp[0][0] = 20;
  temp[1][1] = 15;
  temp[0][1] = 15;
  temp[1][0] = 15;

  temp[64][0] = 20;
  temp[63][1] = 15;
  temp[63][0] = 15;
  temp[64][1] = 15;

  temp[64][64] = 20;
  temp[63][63] = 15;
  temp[64][63] = 15;
  temp[63][64] = 15;

  temp[0][64] = 20;
  temp[1][63] = 15;
  temp[0][63] = 15;
  temp[1][64] = 15;
  return setHeightMap(temp);
}

int LandRecord::setHeightMap(std::int32_t heightmap[65][65])
{
  this->Unknown1 = heightmap[0][0];
  this->Unknown2 = 0x00;
  this->Unknown3 = 0x0000;
  memcpy(this->AbsHeightMap, heightmap, 65*65 * sizeof(std::int32_t));

  // Now that the AbsHeightMap has been set, recalculate the differential height map
  if (this->convertHeightMapToDiff() != 1)
  {
      throw std::runtime_error("convertHeightMapToDiff() failed");
  }
  return 1;
}

int LandRecord::printHeightMap(bool asciiHeightMapActive)
{
  printf("HeightMap (N vvv):\n");
  for (unsigned int i = 0; i < 65; i++)
  {
    for (unsigned int j = 0; j < 65; j++)
    {
      if (asciiHeightMapActive)
      {
        char value[2];
        asciiHeightToChar(this->AbsHeightMap[i][j], value);
        printf("%s ", value);
      }
      else
      {
        printf("% 4d", this->AbsHeightMap[i][j]);
      }
    }
    printf("\n");
  }
  return 1;
}

void LandRecord::asciiHeightToChar(char num, char *buf)
{
  if (num < 5)
  {
    buf[0] = '.';
  }
  else if (num < 10)
  {
    buf[0] = ':';
  }
  else if (num < 15)
  {
    buf[0] = '&';
  }
  else
  {
    buf[0] = '#';
  }
  buf[1] = 0;
}

// Seems like for (i, j) j = 64 we have an differential offset vector that applies to a row at a time.
// SO height(i, j) = float offset + sum( height(n, 64), n = 0..i) ) + sum( height(i, n), n = 0..j) )
// So we have a 65x65 height map where row (i, 0) = float offset + sum( height(n, 64), n = 0..i )
//
// North is along increasing i, and so East is along increasing j.
int LandRecord::convertHeightMapToDiff()
{
  signed char temp[65][65];
  memset(temp, 0, 65 * 65 * sizeof(char));

  // point (0, 0) = float offset, soooo....
  this->Unknown1 = round(this->AbsHeightMap[0][0]);
  std::cout << "offset: " << this->Unknown1 << std::endl;

  // Now set the first column using the differential row offset vector...
  for (unsigned int i = 1; i < 65; i++)
  {
    temp[i-1][64] = this->AbsHeightMap[i][0] - this->AbsHeightMap[i-1][0];
  }

  // Now set all the other 64 columns using the differential row offset vector AND
  // each points differential summation vector.
  for (unsigned int i = 0; i < 65; i++)
  {
    for (unsigned int j = 0; j < 64; j++)
    {
      temp[i][j] = this->AbsHeightMap[i][j+1] - this->AbsHeightMap[i][j];
    }
  }

  memcpy(&(this->DiffHeightMap), temp, 65*65);

  return 1;
}

int LandRecord::setNormalsFromHeightmap(int32_t heightmap[65][65])
{
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
  this->setNormalMap(normalmap);

  return 1;
}

int LandRecord::setNormalMap(normals normalmap)
{
  memcpy(this->NormalMap, normalmap, 65*65*3);
  return 1;
}

int LandRecord::setWorldMapPixels(std::string pixelArray)
{
  this->WorldMapPixels.clear();
  this->WorldMapPixels = pixelArray;

  return 1;
}

int LandRecord::setDataValues(ModSubRecord subRecord)
{
  if (strcmp(subRecord.name, "INTV") == 0)
  {
    printf("size: %d\n", subRecord.size);
    std::int32_t *data = (std::int32_t *) subRecord.data.data();
    this->CellX = data[0];
    this->CellY = data[1];
  }
  else if (strcmp(subRecord.name, "DATA") == 0)
  {
    std::int32_t *data = (std::int32_t *) subRecord.data.data();
    this->Unknown = data[0];
  }
  else if (strcmp(subRecord.name, "VNML") == 0)
  {
    normals *data = (normals *) subRecord.data.data();
    memcpy(this->NormalMap, data, 65*65*3);
  }
  else if (strcmp(subRecord.name, "VHGT") == 0)
  {
    float *data = (float *) subRecord.data.data();
    this->Unknown1 = data[0];
    std::cout << "offset: " << this->Unknown1 << std::endl;
    char *data2 = (char *) &(data[1]);
    this->Unknown2 = data2[0];
    signed char *data3 = (signed char *) &(data2[1]);
    memcpy(this->DiffHeightMap, data3, 65*65);
    short *data4 = (short *) &(data3[65*65]);
    this->Unknown3 = data4[0];
  }
  else if (strcmp(subRecord.name, "WNAM") == 0)
  {
    char *data = (char *) subRecord.data.data();
    this->WorldMapPixels.clear();
    this->WorldMapPixels.assign(data);
  }
  else
  {
    // Didn't recognize the subrecord type...
    return -1;
  }

  return 1;
}

int LandRecord::setRecordSize()
{
  std::uint32_t size = 0;

  // INTV - subrecord header + int32_t CellX + int32_t CellY
  size += 2 * sizeof(std::int32_t) + 8;

  // DATA - subrecord header + uint32_t Unknown
  size += sizeof(std::uint32_t) + 8;

  // VNML - subrecord header + (65*65*3 bytes) normals
  size += sizeof(normals) + 8;

  // VHGT - subrecord header + float Unknown1 + char Unknown2 +
  //        (65*65 char) HeightMap + short Unknown3
  size += sizeof(float) + (65 * 65 + 1) * sizeof(char) + sizeof(short) + 8;

  // WNAM - subrecord header + (9*9 char) WorldMapPixels
  size += 81 * sizeof(char) + 8;

  // VTEX - subrecord header + (16*16 uint16_t) vtex
  if (this->UsingVtex)
  {
    size += (16 * 16) * sizeof(std::uint16_t) + 8;
  }

  this->size = size;

  return 1;
}

size_t LandRecord::exportToModFile(FILE *fid)
{
  size_t totalSize = 0;

  // LAND record header
  totalSize += fwrite("LAND", sizeof(char), 4, fid);
  totalSize += fwrite(&(this->size), sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->header1), sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->flags), sizeof(std::uint32_t), 1, fid);

  printf("land\n");
  fflush(stdout);

  // INTV subrecord
  totalSize += fwrite("INTV", sizeof(char), 4, fid);
  std::uint32_t size = 2 * sizeof(std::uint32_t);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->CellX), sizeof(std::int32_t), 1, fid);
  totalSize += fwrite(&(this->CellY), sizeof(std::int32_t), 1, fid);

  printf("intv\n");
  fflush(stdout);

  // DATA subrecord
  totalSize += fwrite("DATA", sizeof(char), 4, fid);
  size = sizeof(std::uint32_t);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->Unknown), sizeof(std::uint32_t), 1, fid);

  printf("data\n");
  fflush(stdout);

  // VNML subrecord
  totalSize += fwrite("VNML", sizeof(char), 4, fid);
  size = 1 * sizeof(normals);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->NormalMap), sizeof(normals), 1, fid);

  printf("vnml\n");
  fflush(stdout);

  // VHGT subrecord
  totalSize += fwrite("VHGT", sizeof(char), 4, fid);
  size = (65 * 65 + 1) * sizeof(char) + sizeof(short) + sizeof(float);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->Unknown1), sizeof(float), 1, fid);
  totalSize += fwrite(&(this->Unknown2), sizeof(char), 1, fid);
  totalSize += fwrite(&(this->DiffHeightMap), sizeof(char), 65 * 65, fid);
  totalSize += fwrite(&(this->Unknown3), sizeof(short), 1, fid);

  printf("vhgt\n");
  fflush(stdout);

  // WNAM subrecord
  totalSize += fwrite("WNAM", sizeof(char), 4, fid);
  size = 81 * sizeof(char);
  totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
  totalSize += fwrite(&(this->WorldMapPixels), sizeof(char), 81, fid);

  printf("wnam\n");
  fflush(stdout);

  // VTEX subrecord
  if (this->UsingVtex)
  {
    totalSize += fwrite("VTEX", sizeof(char), 4, fid);
    size = 16 * 16 * sizeof(std::uint16_t);
    totalSize += fwrite(&size, sizeof(std::uint32_t), 1, fid);
    totalSize += fwrite(&(this->Vtex), sizeof(std::uint16_t), 16 * 16, fid);
  }

  printf("vtex\n");
  fflush(stdout);

  return totalSize;
}
