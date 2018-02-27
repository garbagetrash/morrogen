/*
 * FileHeader.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Kyle
 */

#ifndef FILEHEADER_H_
#define FILEHEADER_H_

#include "ModRecord.h"
#include "ModSubRecord.h"

#include <string>
#include <vector>
#include <fstream>

class FileHeaderRecord: public ModRecord
{
public:
	int setHedrSubRecord(float version, long unknown,
					     const char *companyName,
					     std::string fileDesc, long nRecords);

	int setMastSubRecord(std::vector< std::string > fileNames);

	int setDataSubRecord(std::vector<long long> masterSize);

	int setRecordSize();

	// Write the object to the raw mod data format
	std::string exportToModData();

	size_t exportToModFile(FILE *outFile);

	// Data
	// HEDR stuff
	float Version;
	long Unknown;
	std::string CompanyName;		// 32 bytes...
	std::string EsmFileDesc;		// 256 bytes...
	long NumRecords;				// 48227

	// MAST stuff
	std::vector< std::string > MasterFiles;

	// DATA stuff
	std::vector<long long> MasterSize;
};

#endif /* FILEHEADER_H_ */
