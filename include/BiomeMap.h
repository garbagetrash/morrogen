/*
 * BiomeMap.h
 *
 * Created on: Sep. 1, 2018
 *     Author: Kyle
 */


#ifndef BIOMEMAP_H_
#define BIOMEMAP_H_

#include <vector>


class BiomeMap {
public:
  int moisturemap[65][65];
  int temperaturemap[65][65];

private:

  void gen_moisture_cell(int cellX, int cellY);
  void gen_temperature_cell(int cellX, int cellY);

};

#endif // BIOMEMAP_H_
