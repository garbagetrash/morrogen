/*
 * CellRecord.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#include "CellRecord.h"

#include <stdio.h>
#include <string.h>

int CellRecord::setIdString(const char *name)
{
	this->IdString.clear();
	for (unsigned int i = 0; i < strlen(name); i++)
	{
		this->IdString[i] = name[i];
	}
	return 1;
}

int CellRecord::setRegionName(const char *name)
{
	this->RegionName.clear();
	for (unsigned int i = 0; i < strlen(name); i++)
	{
		this->RegionName[i] = name[i];
	}
	return 1;
}

int CellRecord::setGridAndFlags(int GridX, int GridY, int Flags)
{
	this->GridX = GridX;
	this->GridY = GridY;
	this->Flags = Flags;

	return 1;
}

int CellRecord::setDataValues(ModSubRecord subRecord)
{
	if (strcmp(subRecord.name, "NAME") == 0)
	{
		printf("size: %d\n", subRecord.size);
		char *data = (char *) subRecord.data.data();
		for (int i = 0; i < subRecord.size; i++)
		{
			this->IdString[i] = data[i];
		}
	}
	else if (strcmp(subRecord.name, "DATA") == 0)
	{
		long *data = (long *) subRecord.data.data();
		this->Flags = data[0];
		this->GridX = data[1];
		this->GridY = data[2];
	}
	else if (strcmp(subRecord.name, "RGNN") == 0)
	{
		char *data = (char *) subRecord.data.data();
		for (int i = 0; i < subRecord.size; i++)
		{
			this->RegionName[i] = data[i];
		}
	}
	else
	{
		// Didn't recognize the subrecord type...
		return -1;
	}

	return 1;
}

std::vector<char> CellRecord::exportToModData()
{
	std::vector<char> outputData;

	// TODO: This.

	return outputData;
}
