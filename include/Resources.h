/*
 * Resources.h
 *
 *  Created on: Mar 04, 2018
 *      Author: Kyle
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <string>
#include <vector>

typedef struct StaticObject {
  std::string Id;
  float zOffset;
} StaticObject;

class TreeSets {
public:
  static const std::vector<StaticObject> BC;
};

typedef struct LtexPair {
  std::string Id;
  std::string TexturePath;
} LtexPair;

class TextureSets {
public:
  static const std::vector<LtexPair> BC;
};

#endif /* RESOURCES_H_ */
