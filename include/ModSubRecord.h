/*
 * ModSubRecord.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#ifndef MODSUBRECORD_H_
#define MODSUBRECORD_H_

#include "json.h"

#include <vector>

class ModSubRecord {
public:
  ModSubRecord();
  virtual ~ModSubRecord();

  // Encodes the subrecord to human readable json format
  int encodeToJSON(JsonNode *subRecord);

  // Exports the subrecord to raw mod data format
  std::vector<char> exportToModData();

  // Sub record name string (not null terminated)
  char name[5];

  // Size of the sub record excluding the 8 bytes of header data
  int size;

  // Sub record data
  std::vector<char> data;

};

#endif /* MODSUBRECORD_H_ */
