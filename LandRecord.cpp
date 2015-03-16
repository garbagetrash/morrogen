/*
 * LandRecord.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#include "LandRecord.h"

#include <stdio.h>
#include <string.h>

int LandRecord::setUnknown()
{
	this->Unknown = 0x09;

	return 1;
}

int LandRecord::setCell(long CellX, long CellY)
{
	this->CellX = CellX;
	this->CellY = CellY;

	return 1;
}

int LandRecord::setHeightMap(signed char heightmap[65][65], float offset)
{
	this->Unknown1 = offset;
	this->Unknown2 = 0x00;
	this->Unknown3 = 0x0000;
	memcpy(this->HeightMap, heightmap, 65*65);
	return 1;
}

int LandRecord::setNormalMap(normals normalmap)
{
	memcpy(this->NormalMap, normalmap, 65*65*3);
	return 1;
}

int LandRecord::setWorldMapPixels(std::vector< std::vector<char> > pixelArray)
{
	// Clear the 2d vector
	for (unsigned int i = 0; i < 9; i++)
	{
		this->WorldMapPixels[i].clear();
	}
	this->WorldMapPixels.clear();

	// Now assign to it
	for (unsigned int i = 0; i < 9; i++)
	{
		for (unsigned int j = 0; j < 9; j++)
		{
			this->WorldMapPixels[i][j] = pixelArray[i][j];
		}
	}

	return 1;
}

int LandRecord::setDataValues(ModSubRecord subRecord)
{
	if (strcmp(subRecord.name, "INTV") == 0)
	{
		printf("size: %d\n", subRecord.size);
		long *data = (long *) subRecord.data.data();
		this->CellX = data[0];
		this->CellY = data[1];
	}
	else if (strcmp(subRecord.name, "DATA") == 0)
	{
		long *data = (long *) subRecord.data.data();
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
		char *data2 = (char *) &(data[1]);
		this->Unknown2 = data2[0];
		signed char *data3 = (signed char *) &(data2[1]);
		memcpy(this->HeightMap, data3, 65*65);
		short *data4 = (short *) &(data3[65*65]);
		this->Unknown3 = data4[0];
	}
	else if (strcmp(subRecord.name, "WNAM") == 0)
	{
		char *data = (char *) subRecord.data.data();
		for (unsigned int i = 0; i < 9; i++)
		{
			memcpy(this->WorldMapPixels[i].data(), data, 9);
		}
	}
	else
	{
		// Didn't recognize the subrecord type...
		return -1;
	}

	return 1;
}
