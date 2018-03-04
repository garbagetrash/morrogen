/*
 * LtexRecord.h
 *
 *  Created on: Mar 04, 2018
 *      Author: Kyle
 */

#ifndef LTEXRECORD_H_
#define LTEXRECORD_H_

#include "ModRecord.h"
#include "ModSubRecord.h"

#include <stdio.h>

#include <cstdint>
#include <string>
#include <vector>

typedef struct LtexPair {
  std::string Id;
  std::string TexturePath;
} LtexPair;

const LtexPair BC_Road        {std::string("Tx_BC_mainroad_01.tga"),
                               std::string("Tx_BC_mainroad_01.tga")};
const LtexPair BC_Sand        {std::string("Sand_02"),
                               std::string("Tx_sand_02.tga")};
const LtexPair BC_Grass       {std::string("Tx_BC_grass.tga"),
                               std::string("Tx_BC_grass.tga")};
const LtexPair BC_Muck        {std::string("Tx_BC_muck.tga"),
                               std::string("Tx_BC_muck.tga")};
const LtexPair BC_Muck1       {std::string("Tx_BC_muck_01.tga"),
                               std::string("Tx_BC_muck_01.tga")};
const LtexPair BC_Mud         {std::string("Tx_BC_mud.tga"),
                               std::string("Tx_BC_mud.tga")};
const LtexPair BC_Undergrowth {std::string("Tx_BC_undergrowth.tga"),
                               std::string("Tx_BC_undergrowth.tga")};
const LtexPair BC_Moss        {std::string("Tx_BC_moss.tga"),
                               std::string("Tx_BC_moss.tga")};
const LtexPair BC_Scrub       {std::string("Tx_BC_scrub.tga"),
                               std::string("Tx_BC_scrub.tga")};
const LtexPair BC_Dirt        {std::string("Tx_BC_dirt.tga"),
                               std::string("Tx_BC_dirt.tga")};
const LtexPair BC_Bank        {std::string("Tx_BC_bank.tga"),
                               std::string("Tx_BC_bank.tga")};
const LtexPair BC_Rock1       {std::string("Tx_BC_rock_01.tga"),
                               std::string("Tx_BC_rock_01.tga")};
const LtexPair BC_Rock2       {std::string("Tx_BC_rock_02.tga"),
                               std::string("Tx_BC_rock_02.tga")};
const LtexPair BC_Rock3       {std::string("Tx_BC_rock_03.tga"),
                               std::string("Tx_BC_rock_03.tga")};
const LtexPair BC_Rock4       {std::string("Tx_BC_rock_04.tga"),
                               std::string("Tx_BC_rock_04.tga")};
const LtexPair BC_RockyScrub  {std::string("Tx_BC_rockyscrub.tga"),
                               std::string("Tx_BC_rockyscrub.tga")};

static std::vector<LtexPair> BC_Textures = {
  BC_Road,
  BC_Sand,
  BC_Grass,
  BC_Muck,
  BC_Muck1,
  BC_Mud,
  BC_Undergrowth,
  BC_Moss,
  BC_Scrub,
  BC_Dirt,
  BC_Bank,
  BC_Rock1,
  BC_Rock2,
  BC_Rock3,
  BC_Rock4,
  BC_RockyScrub
};

class LtexRecord: public ModRecord
{
public:

  // Set values from subrecords
  int setDataValues(ModSubRecord subRecord);

  // Set the ID
  void setId(std::string id);

  // Set the index
  void setIndex(std::uint32_t index);

  // Set the texture path
  void setTexturePath(std::string texturePath);

  // Set the record size
  int setRecordSize();

  // Write the object to the raw mod data format
  size_t exportToModFile(FILE *fid);

  // Data
  std::uint32_t header1;
  std::uint32_t flags;

  std::string Id;
  std::uint32_t Index;
  std::string TexturePath;
};

#endif /* LTEXRECORD_H_ */
