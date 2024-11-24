/*
 * ModFile.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Kyle
 */

#include "ModFile.h"
#include "simplexnoise.h"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "CellRecord.h"
#include "FileHeaderRecord.h"
#include "LandRecord.h"
#include "LtexRecord.h"
#include "Resources.h"

ModFile::ModFile() {}

ModFile::ModFile(const char *fileName) {
  int result = setRawDataFromFile(fileName);
  if (result < 0)
  {
    std::cout << "Failed to create File class!" << std::endl;
  }
}

int ModFile::parseRawData()
{
  std::cout << "Beginning the parseRawData() call..." << std::endl;

  char *ptr = this->rawData.data();
  char *start = ptr;
  while (*ptr != '\0' && (ptr - start) < this->rawData.size())
  {
    // Let record point to our current record
    ModRecord record;
    memset((void *)(&record), 0, sizeof(ModRecord));

    // Grab the name first
    strncpy(record.name, ptr, 4);
    std::cout << "name: " << record.name << std::endl;
    ptr += 4;

    // Then the record size
    memcpy(&(record.size), ptr, 4);
    std::cout << "size: " << record.size << std::endl;
    ptr += 4;

    // Then the header1
    memcpy(&(record.header1), ptr, 4);
    std::cout << "header1: " << record.header1 << std::endl;
    ptr += 4;

    // And finally the flags
    memcpy(&(record.flags), ptr, 4);
    std::cout << "flags: " << record.flags << std::endl;
    ptr += 4;

    // Now take the rest of the record and toss it into the rawData blob for that record...
    for (int i = 0; i < record.size; i++)
    {
      record.rawData.push_back(ptr[i]);
    }

    // And tell it to parse that into the sub record data for itself
    record.parseSubRecordsFromRawData();

    // Now just increment the pointer to the next record in the raw data
    this->records.push_back(record);
    ptr += record.size;
  }

  for (unsigned int i = 0; i < this->records.size(); i++)
  {
    if (strcmp(this->records[i].name, "LAND") == 0)
    {
      std::cout << "Found a LAND record!" << std::endl;
    }
  }

  std::cout << "Completed parseRawData() call!" << std::endl;

  return 1;
}

std::vector<CellRecord> ModFile::generateCellRecords(int cellXstart,
                                                     int cellXstop,
                                                     int cellYstart,
                                                     int cellYstop,
                                                     int flags,
                                                     std::string region_name,
                                                     NoiseType type,
                                                     RegionType region_type)
{
  std::vector<CellRecord> cellRecords;
  int lenX = abs(cellXstop - cellXstart) + 1;
  int lenY = abs(cellYstop - cellYstart) + 1;

  int lowX = cellXstart;
  if (cellXstart > cellXstop) {
    lowX = cellXstop;
  }

  int lowY = cellYstart;
  if (cellYstart > cellYstop) {
    lowY = cellYstop;
  }

  for (int i = 0; i < lenX; i++)
  {
    for (int j = 0; j < lenY; j++)
    {
      CellRecord cellRecord = generateCellRecord("", lowX + i, lowY + j, flags,
                                                 region_name, type,
                                                 region_type);
      cellRecords.push_back(cellRecord);
    }
  }

  return cellRecords;
}

float uniform_random(void)
{
  return (static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX));
}

float heightmap_noise(float x, float y, NoiseType type)
{
  // Default
  float octaves = 6.0;
  float persistence = 0.5;
  float scale = 0.25;
  float loBound = -4.0;
  float hiBound = 4.0;

  // Noise types
  if (type == NoiseType::shallow_large_islands)
  {
    octaves = 6.0;
    persistence = 0.5;
    scale = 0.25;
    loBound = -4.0;
    hiBound = 4.0;
  }
  else if(type == NoiseType::mountains_with_lakes)
  {
    octaves = 6.0;
    persistence = 0.5;
    scale = 0.25;
    loBound = -4.0;
    hiBound = 12.0;
  }
  else if(type == NoiseType::broad_low_hills)
  {
    octaves = 5.0;
    persistence = 0.5;
    scale = 0.25;
    loBound = -1.0;
    hiBound = 3.0;
  }

  return scaled_octave_noise_2d(octaves, persistence, scale, loBound, hiBound,
                                x, y);
}

CellRecord ModFile::generateCellRecord(const char *id, int cellX, int cellY,
                                       int flags, std::string region_name,
                                       NoiseType type, RegionType region_type)
{
  CellRecord cellRecord;
  cellRecord.setIdString(id);
  cellRecord.setGridAndFlags(cellX, cellY, flags);
  cellRecord.setRegionName(region_name);

  // Determine tree_set and n_tress based on region type
  std::vector<StaticObject> tree_set;
  std::vector<StaticObject> building_set;
  int n_trees = 10;
  switch(region_type) {
    case(RegionType::ASCADIAN_ISLES):
      tree_set = TreeSets::AI;
      n_trees = 50;
      building_set = BuildingSets::Shanty;
      break;
    case(RegionType::BITTER_COAST):
      tree_set = TreeSets::BC;
      n_trees = 100;
      building_set = BuildingSets::Shanty;
      break;
    case(RegionType::GRAZELANDS):
      tree_set = TreeSets::GL;
      n_trees = 10;
      building_set = BuildingSets::Shanty;
      break;
  }

  // Randomly position trees around the cell
  for (int i = 0; i < n_trees; i++)
  {
    PosRotData prdata;
    prdata.posX = 8192.0 * cellX + 8192.0 * uniform_random();
    prdata.posY = 8192.0 * cellY + 8192.0 * uniform_random();
    float value = heightmap_noise(prdata.posY / 8192.0, prdata.posX / 8192.0, type);
    prdata.posZ = value * 1024.0;
    prdata.rotX = 0.0;
    prdata.rotY = 0.0;
    prdata.rotZ = 2 * M_PI * uniform_random();

    // If the land here is at least close to the shoreline
    if (prdata.posZ > -10.0) {
      StaticObject tree = tree_set[std::rand() % tree_set.size()];
      prdata.posZ += tree.zOffset;
      cellRecord.addObjectToCell(tree.Id, prdata);
    }
  }

  cellRecord.setRecordSize();

  /*
  // Put a village in the cell with some low probability
  if (uniform_random() > 0.95) {

    float value = -1.0;
    do {
      // Generate a position within the cell to act as the village center point
      float xglobal = 8192.0 * (cellX + uniform_random());
      float yglobal = 8192.0 * (cellY + uniform_random());

      // Look up the height at this location with our generator function
      value = 1024.0 * heightmap_noise(yglobal / 8192.0, xglobal / 8192.0, type);
    } while (value <= 0.0);

    // Now above the waterline
    // TODO: Generate village logic here
  }
  */
  return cellRecord;
}

void create_mod_from_ir()
{
    // TODO: Create a whole mod file from a possibly large json file.
    ModFile mf = ModFile();

    // TODO: Load cell properties
    char filename[] = "NewLandMod.esp";
    int cell_x = -11;
    int cell_y = -11;
    std::vector<CellRecord> cells = mf.generateCellRecords(cell_x, cell_x, cell_y, cell_y, 2,
            std::string("region_name"), NoiseType::shallow_large_islands, RegionType::ASCADIAN_ISLES);

    // TODO: Load heightmap
    int32_t heightmap[65][65] = {0};

    // TODO: Load texture map
    uint16_t texturemap[16][16] = {0};
    std::vector<LtexRecord> ltexs = mf.generateLtexRecords(TextureSets::AI);

    std::vector<LandRecord> lands;
    lands.push_back(land_record_from_maps(cell_x, cell_y, heightmap, texturemap));

    // Write out mod file
    int nRecords = cells.size() + lands.size() + ltexs.size();
    FileHeaderRecord header = mf.generateHeader(filename, nRecords);

    // Write to the file
    FILE *fid = fopen(filename, "wb");
    size_t totalSize = header.exportToModFile(fid);

    for (unsigned int i=0; i < cells.size(); i++) {
        totalSize += cells[i].exportToModFile(fid);
    }

    for (unsigned int i=0; i < lands.size(); i++) {
        totalSize += lands[i].exportToModFile(fid);
    }

    for (unsigned int i=0; i < ltexs.size(); i++) {
        totalSize += ltexs[i].exportToModFile(fid);
    }

    fclose(fid);
    std::cout << "Wrote " << totalSize << " bytes!" << std::endl;
}

void load_heightmap_ppm(const char *filename)
{
    // Load in .ppm file, which is dead simple RGB u8 data for the most part.
    FILE *fid;
    fid = fopen(filename, "rb");
    int xdim, ydim;
    fscanf(fid, "P6\n%d %d\n255\n", &xdim, &ydim);
    printf("X: %d\nY: %d\n", xdim, ydim);
    uint8_t *rgb = new uint8_t[3 * xdim * ydim];
    fread(rgb, sizeof(uint8_t), 3 * xdim * ydim, fid);
    fclose(fid);

    // TODO: Divide into 65x65 Cells starting from ???

    delete[] rgb;
}

std::vector<LandRecord> ModFile::generateLandRecords(int cellXstart,
                                                     int cellXstop,
                                                     int cellYstart,
                                                     int cellYstop,
                                                     NoiseType type)
{
  std::vector<LandRecord> landRecords;
  int lenX = abs(cellXstop - cellXstart) + 1;
  int lenY = abs(cellYstop - cellYstart) + 1;

  int lowX = cellXstart;
  if (cellXstart > cellXstop) {
    lowX = cellXstop;
  }

  int lowY = cellYstart;
  if (cellYstart > cellYstop) {
    lowY = cellYstop;
  }

  for (int i = 0; i < lenX; i++)
  {
    for (int j = 0; j < lenY; j++)
    {
      LandRecord landRecord = generateLandRecord(lowX + i, lowY + j, type);
      landRecords.push_back(landRecord);
    }
  }

  return landRecords;
}

void apply_heightmap_layer(int32_t layer[65][65], int32_t heightmap[65][65])
{
    for (size_t x = 0; x < 65; x++) {
        for (size_t y = 0; y < 65; y++) {
            heightmap[x][y] += layer[x][y];
        }
    }
}

std::vector<Point> get_neighbors(Point p)
{
    std::vector<Point> output;
    if (p.x > 0) {
        output.push_back({p.x-1, p.y});
        if (p.y > 0) {
            output.push_back({p.x-1, p.y-1});
            output.push_back({p.x, p.y-1});
        }
        if (p.y < 64) {
            output.push_back({p.x-1, p.y+1});
            output.push_back({p.x, p.y+1});
        }
    }
    if (p.x < 64) {
        output.push_back({p.x+1, p.y});
        if (p.y > 0) {
            output.push_back({p.x+1, p.y-1});
        }
        if (p.y < 64) {
            output.push_back({p.x+1, p.y+1});
        }
    }
    return output;
}

void generate_road_from_cell_layout(std::vector<Point> layout,
        int32_t heightmap[65][65],
        uint16_t land_texture_map[16][16])
{
    // layout is a rasturized grid of _where_ to place road. `heightmap` and
    // `land_texture_map` are what we modify to make the road.
    
    int32_t layer[65][65] = {0};
    for (size_t i = 0; i < layout.size(); i++)
    {
        std::vector<Point> neighbors = get_neighbors(layout[i]);
        for (auto &neighbor : neighbors) {
            heightmap[neighbor.x][neighbor.y] -= 2;
        }
        size_t x = layout[i].x;
        size_t y = layout[i].y;
        heightmap[x][y] -= 2;

        size_t xx = x / 4;
        size_t yy = y / 4;
        if (xx >= 16) {
            xx = 15;
        }
        if (yy >= 16) {
            yy = 15;
        }
        land_texture_map[yy][xx] = 2; // sand for now...
    }
}


LandRecord land_record_from_maps(
        int cellX, int cellY,
        int32_t heightmap[65][65],
        uint16_t texturemap[16][16])
{
  LandRecord landRecord;
  landRecord.setCell(cellX, cellY);
  landRecord.setUnknown();

  // Create and set the normal map from the height map data
  landRecord.setNormalsFromHeightmap(heightmap);

  // Set the world map pixels... all to whatever 0 is for now.
  std::string pixelMap;
  pixelMap.assign(81, 0);
  landRecord.setWorldMapPixels(pixelMap);

  // Bake the final heightmap.
  landRecord.setHeightMap(heightmap);

  // Bake the land textures once the indices are all sorted out.
  landRecord.setVtexIndices(texturemap);

  landRecord.setRecordSize();

  return landRecord;
}


LandRecord ModFile::generateLandRecord(int cellX, int cellY, NoiseType type)
{
  LandRecord landRecord;
  landRecord.setCell(cellX, cellY);
  landRecord.setUnknown();

  // Create and set the height map
  std::int32_t heightmap[65][65];
  for (int i = 0; i < 65; i++)
  {
    const float x = i / 64.0 + cellY;
    for (int j = 0; j < 65; j++)
    {
      const float y = j / 64.0 + cellX;
      float value = 128 * heightmap_noise(x, y, type);
      heightmap[i][j] = round(value);
    }
  }

  // Create and set the normal map from the height map data
  landRecord.setNormalsFromHeightmap(heightmap);
  // landRecord.printHeightMap(false);

  // Set the world map pixels... all to whatever 0 is for now.
  std::string pixelMap;
  pixelMap.assign(81, 0);
  landRecord.setWorldMapPixels(pixelMap);

  // Set the texture indices
  std::uint16_t land_texture_map[16][16];
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      std::int32_t height = heightmap[4 * i + 2][4 * j + 2];
      if (height < 10)
      {
        // Sand
        land_texture_map[i][j] = 2;
      } else {
        // Not sand
        land_texture_map[i][j] = 1;
      }
    }
  }

  // TODO: Add logic to lay down roads here
  // 1.) Should put a divet in the heightmap like a well worn road in the game.
  // 2.) Should put down appropriate texturing.
  //
  // 1.) Generate a road network globally.
  // 2.) Break down network into cells, and create cell road layout structs.
  // 3.) Generate roads per cell using the corresponding cell road layout.
  std::vector<Point> layout;
  for (int32_t i = 0; i < 65; i++)
  {
      Point p = {i, i};
      layout.push_back(p);

      if (i < 64) {
          Point p1 = {i+1, i};
          layout.push_back(p1);
      }

      if (i < 63) {
          Point p1a = {i + 2, i};
          layout.push_back(p1a);
      }

      if (i > 0) {
          Point p2 = {i-1, i};
          layout.push_back(p2);
      }

      if (i > 1) {
          Point p2a = {i-2, i};
          layout.push_back(p2a);
      }
  }
  generate_road_from_cell_layout(layout, heightmap, land_texture_map);

  // Bake the final heightmap.
  landRecord.setHeightMap(heightmap);

  // Bake the land textures once the indices are all sorted out.
  landRecord.setVtexIndices(land_texture_map);

  landRecord.setRecordSize();

  return landRecord;
}

std::vector<LtexRecord> ModFile::generateLtexRecords(std::vector<LtexPair> textureSet)
{
  std::vector<LtexRecord> ltexRecords;

  for (int i = 0; i < textureSet.size(); i++)
  {
    LtexRecord ltexRecord = generateLtexRecord(textureSet[i], i);
    ltexRecords.push_back(ltexRecord);
  }

  return ltexRecords;
}

LtexRecord ModFile::generateLtexRecord(const LtexPair &pair, std::uint32_t index)
{
  LtexRecord ltexRecord;

  ltexRecord.setId(pair.Id);
  ltexRecord.setIndex(index);
  ltexRecord.setTexturePath(pair.TexturePath);

  ltexRecord.setRecordSize();

  return ltexRecord;
}

void ModFile::generateTestCell()
{
    // CELL record
  CellRecord cellRecord;
  cellRecord.setIdString("");
  cellRecord.setGridAndFlags(-11, -11, 2);
  cellRecord.setRegionName(std::string("Ascadian Isles Region"));
  cellRecord.setRecordSize();
  std::vector<CellRecord> cellRecords;
  cellRecords.push_back(cellRecord);

  // LAND record
  LandRecord landRecord;
  landRecord.setCell(-11, -11);
  landRecord.setUnknown();
  std::int32_t heightmap[65][65];
  for (int i = 0; i < 65; i++)
  {
    for (int j = 0; j < 65; j++)
    {
        heightmap[i][j] = 1;
    }
  }
  landRecord.setNormalsFromHeightmap(heightmap);
  std::string pixelMap;
  pixelMap.assign(81, 0);
  landRecord.setWorldMapPixels(pixelMap);
  // Set the texture indices
  std::uint16_t land_texture_map[16][16];
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      land_texture_map[i][j] = 1;
    }
  }
  for (int i = 0; i < 16; i++) {
      land_texture_map[0][i] = 2;
  }
  land_texture_map[1][4] = 2;
  land_texture_map[2][4] = 2;
  land_texture_map[3][4] = 2;
  land_texture_map[4][4] = 2;
  land_texture_map[5][4] = 2;
  landRecord.setHeightMap(heightmap);
  landRecord.setVtexIndices(land_texture_map);
  landRecord.setRecordSize();
  std::vector<LandRecord> landRecords;
  landRecords.push_back(landRecord);

  // Now make the LTEX record(s)
  std::vector<LtexRecord> ltexRecords = generateLtexRecords(TextureSets::AI);

  // Create header last, once we knew the number of records
  int nRecords = cellRecords.size() + landRecords.size() + ltexRecords.size();
  FileHeaderRecord header = generateHeader("NewLandMod.esp", nRecords);

  // Write to the file
  FILE *fid = fopen("NewLandMod.esp", "wb");
  size_t totalSize = header.exportToModFile(fid);

  for (unsigned int i=0; i < cellRecords.size(); i++) {
    totalSize += cellRecords[i].exportToModFile(fid);
  }

  for (unsigned int i=0; i < landRecords.size(); i++) {
    totalSize += landRecords[i].exportToModFile(fid);
  }

  for (unsigned int i=0; i < ltexRecords.size(); i++) {
    totalSize += ltexRecords[i].exportToModFile(fid);
  }

  fclose(fid);
  std::cout << "Wrote " << totalSize << " bytes!" << std::endl;
}

int ModFile::generateNewLand(const char *filename, int cellXstart,
                             int cellXstop, int cellYstart, int cellYstop,
                             NoiseType type, RegionType region_type,
                             unsigned int seed)
{
  // Set the region name
  std::string region_name;
  switch(region_type) {
    case(RegionType::ASCADIAN_ISLES):
      region_name = std::string("Ascadian Isles Region");
      break;
    case(RegionType::BITTER_COAST):
      region_name = std::string("Bitter Coast Region");
      break;
    case(RegionType::GRAZELANDS):
      region_name = std::string("Grazelands Region");
      break;
    default:
      // Just do the grazelands by default
      region_name = std::string("Grazelands Region");
  }
  // First create the CELL record(s)
  std::vector<CellRecord> cellRecords = generateCellRecords(cellXstart,
                                          cellXstop, cellYstart, cellYstop, 2,
                                          region_name,
                                          type, region_type);

  // Now make the LAND record(s)
  std::vector<LandRecord> landRecords = generateLandRecords(cellXstart,
                                          cellXstop, cellYstart, cellYstop,
                                          type);

  // Now make the LTEX record(s)
  std::vector<LtexRecord> ltexRecords;
  switch(region_type) {
    case(RegionType::ASCADIAN_ISLES):
      ltexRecords = generateLtexRecords(TextureSets::AI);
      break;
    case(RegionType::BITTER_COAST):
      ltexRecords = generateLtexRecords(TextureSets::BC);
      break;
    case(RegionType::GRAZELANDS):
      ltexRecords = generateLtexRecords(TextureSets::GL);
      break;
    default:
      // Just do the grazelands by default
      ltexRecords = generateLtexRecords(TextureSets::GL);
  }

  // Create header last, once we knew the number of records
  int nRecords = cellRecords.size() + landRecords.size() + ltexRecords.size();
  FileHeaderRecord header = generateHeader(filename, nRecords);

  // Write to the file
  FILE *fid = fopen(filename, "wb");
  size_t totalSize = header.exportToModFile(fid);

  for (unsigned int i=0; i < cellRecords.size(); i++) {
    totalSize += cellRecords[i].exportToModFile(fid);
  }

  for (unsigned int i=0; i < landRecords.size(); i++) {
    totalSize += landRecords[i].exportToModFile(fid);
  }

  for (unsigned int i=0; i < ltexRecords.size(); i++) {
    totalSize += ltexRecords[i].exportToModFile(fid);
  }

  fclose(fid);
  std::cout << "Wrote " << totalSize << " bytes!" << std::endl;

  return 1;
}

int ModFile::setRawDataFromFile(const char *fileName)
{
  // Setup the file
  FILE *fid = fopen(fileName, "rb");
  if (fid == 0) {
    std::cout << "File not found!" << std::endl;
    return -1;
  }

  // Find the file length
  fseek (fid, 0, SEEK_END);
  long lSize = ftell(fid);
  rewind(fid);

  // Allocate memory
  this->rawData = std::vector<char>(0, lSize);

  // Read in the file
  long result = fread(this->rawData.data(), 1, lSize, fid);
  if (result != lSize) {
    std::cout << "Reading error!" << std::endl;
    return -1;
  }

  std::cout << "Successfully populated rawData!" << std::endl;

  char recordType[5];
  memset(recordType, 0, 5);
  strncpy(recordType, this->rawData.data(), 4);
  std::cout << "First record type: " << recordType << std::endl;

  // Tidy up
  fclose(fid);

  return 1;
}

////////////////////////////////////////////////////////////////////
// Private Function Definitions
////////////////////////////////////////////////////////////////////

int ModFile::writeStringToFile(const char *fileName, std::string input)
{
  FILE *fid = fopen(fileName, "wb");
  fwrite(input.c_str(), sizeof(char), input.size(), fid);
  fclose(fid);

  return 1;
}

FileHeaderRecord ModFile::generateHeader(const char *filename,
                                         std::uint32_t nRecords)
{
  FileHeaderRecord header;
  std::string asdf;
  asdf.assign (256, 0);
  header.setHedrSubRecord(1.3, 1, "asdf", asdf, nRecords);
  std::vector< std::string > masterEsms;
  masterEsms.push_back("Morrowind.esm");
  masterEsms.push_back("Bloodmoon.esm");
  masterEsms.push_back("Tribunal.esm");
  header.setMastSubRecord(masterEsms);
  std::vector<std::uint64_t> masterSizes;
  masterSizes.push_back(79837557);
  masterSizes.push_back(9631798);
  masterSizes.push_back(4565686);
  header.setDataSubRecord(masterSizes);
  header.setRecordSize();

  return header;
}
