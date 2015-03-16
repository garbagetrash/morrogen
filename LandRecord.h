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

#include <vector>

class LandRecord: public ModRecord
{
public:

	typedef struct {
		signed char X;
		signed char Y;
		signed char Z;
	} normals[65][65];

	// Set values from subrecords
	int setDataValues(ModSubRecord subRecord);

	// Set Cell location
	int setCell(long CellX, long CellY);

	// Set the Unknown to 0x09
	int setUnknown();

	// Set the height map as specified
	int setHeightMap(signed char heightmap[65][65], float offset);

	// Set the normal map as specified
	int setNormalMap(normals normalmap);

	// Set the world map pixel array
	int setWorldMapPixels(std::vector< std::vector<char> > pixelArray);

	// Write the object to the raw mod data format
	int

	// Data
	int CellX;
	int CellY;

	int Unknown;

	normals NormalMap;

	float Unknown1;
	char Unknown2;
	signed char HeightMap[65][65];
	short Unknown3;

	// This is something to do with the world map color palette... a
	// 9x9 grid of bytes, so 81 pixels.  0 is blue.
	std::vector< std::vector<char> > WorldMapPixels;
};

#endif /* LANDRECORD_H_ */
