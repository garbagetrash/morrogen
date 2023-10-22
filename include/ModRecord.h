/*
 * ModRecord.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#ifndef MODRECORD_H_
#define MODRECORD_H_

#include "ModSubRecord.h"
#include "json.h"

#include <vector>

class ModRecord {
public:
	// 4 byte record name string
	char name[5] = "";

	// Size of the record not including 16 bytes of header data
	int size = 0;

	// Unknown value... usually 0;
	int header1 = 0;

	// Record flags
	int flags = 0x2000; // 0x2000 - Blocked, 0x400 - Persistent

	ModRecord();
	virtual ~ModRecord();

	int parseSubRecordsFromRawData();
	int encodeToJSON(JsonNode *record);

	// TODO: Is this something I want here vs in record types?
	std::vector<char> exportToModData();

	// Raw data blob (for subrecords)
	std::vector<char> rawData;

	// Subrecords
	std::vector<ModSubRecord> subRecords;
};

#endif /* MODRECORD_H_ */
