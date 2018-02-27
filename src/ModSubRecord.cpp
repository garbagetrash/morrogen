/*
 * ModSubRecord.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModSubRecord.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ModSubRecord::ModSubRecord() {
  // TODO Auto-generated constructor stub

}

ModSubRecord::~ModSubRecord() {
  // TODO Auto-generated destructor stub
}

int ModSubRecord::encodeToJSON(JsonNode *subRecord)
{
  json_append_member(subRecord, "Name", json_mkstring(this->name));

  char temp[10];
  memset(temp, 0, 10);
  sprintf(temp, "%d", this->size);
  json_append_member(subRecord, "Size", json_mkstring(temp));

  // Not all data is valid UTF-8... so it fails.  What do?
  JsonNode *dataArr = json_mkarray();
  for (unsigned int i = 0; i < this->data.size(); i++)
  {
    float tempNum = (float) (this->data[i]);
    json_append_element(dataArr, json_mknumber(tempNum));
  }

  json_append_member(subRecord, "Data", dataArr);

  return 1;
}

// TODO: this
std::vector<char> ModSubRecord::exportToModData()
{
  std::vector<char> outputData;



  return outputData;
}
