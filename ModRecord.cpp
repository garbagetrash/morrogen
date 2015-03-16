/*
 * ModRecord.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModRecord.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ModRecord::ModRecord() {
	// TODO Auto-generated constructor stub

}

ModRecord::~ModRecord() {
	// TODO Auto-generated destructor stub
}

int ModRecord::parseSubRecordsFromRawData() {
	printf("Beginning the parseRawData() call in a record...\n");
	fflush(stdout);

	char *ptr = this->rawData.data();
	char *start;
	start = ptr;
	while (*ptr != '\0' && (unsigned int) (ptr - start) < this->rawData.size())
	{
		// Let record point to our current record
		ModSubRecord subRecord;
		memset(&subRecord, 0, sizeof(ModSubRecord));

		// Grab the name first
		strncpy(subRecord.name, ptr, 4);
		printf("sr name: %s\n", subRecord.name);
		ptr += 4;

		// Then the record size
		memcpy(&(subRecord.size), ptr, 4);
		printf("sr size: %d\n", subRecord.size);
		ptr += 4;
		fflush(stdout);

		// Now take the rest of the record and toss it into the rawData blob for that record...
		for (int i = 0; i < subRecord.size; i++)
		{
			subRecord.data.push_back(ptr[i]);
		}

		// Now just increment the pointer to the next record in the raw data
		this->subRecords.push_back(subRecord);
		ptr += subRecord.size;
	}

	printf("Finishing parsing record raw data into subrecords!\n");

	return 1;
}

int ModRecord::encodeToJSON(JsonNode *record)
{
	printf("nameIs: %s\n", this->name);

	char temp[10];
	memset(temp, 0, 10);
	json_append_member(record, "Name", json_mkstring(this->name));
	itoa(this->size, temp, 10);
	json_append_member(record, "Size", json_mkstring(temp));
	itoa(this->header1, temp, 16);
	json_append_member(record, "Header1", json_mkstring(temp));
	itoa(this->flags, temp, 16);
	json_append_member(record, "Flags", json_mkstring(temp));

	JsonNode *subRecords = json_mkarray();
	for (unsigned int i = 0; i < this->subRecords.size(); i++)
	{
		JsonNode *subRecord = json_mkobject();

		this->subRecords[i].encodeToJSON(subRecord);

		json_append_element(subRecords, subRecord);
	}

	json_append_member(record, "SubRecords", subRecords);

	return 1;
}
