/*
 * CellRecord.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Kyle
 */

#ifndef CELLRECORD_H_
#define CELLRECORD_H_

#include "ModRecord.h"

#include <vector>

class CellRecord: public ModRecord
{
public:

	// Set the Id string
	int setIdString(const char *name);

	// Set the region name string
	int setRegionName(const char *name);

	// Set the grid and flags (DATA subrecord)
	int setGridAndFlags(int GridX, int GridY, int Flags);

	// Set values from subrecords
	int setDataValues(ModSubRecord subRecord);

	// Export CellRecord to raw mod data format
	std::vector<char> exportToModData();

	// Data
	std::vector<char> IdString;

	int Flags;
	int GridX;
	int GridY;

	std::vector<char> RegionName;
};

#endif /* CELLRECORD_H_ */
