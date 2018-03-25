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

enum RegionType {
  BITTER_COAST = 1,
  ASCADIAN_ISLES,
};

typedef struct StaticObject {
  std::string Id;
  float zOffset;
} StaticObject;

class TreeSets {
public:
  static const std::vector<StaticObject> BC;
  static const std::vector<StaticObject> AI;
};

typedef struct LtexPair {
  std::string Id;
  std::string TexturePath;
} LtexPair;

class TextureSets {
public:
  static const std::vector<LtexPair> BC;
  static const std::vector<LtexPair> AI;
};

#endif /* RESOURCES_H_ */
