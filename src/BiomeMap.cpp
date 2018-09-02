/*
 * BiomeMap.cpp
 *
 * Created on: Sep. 1, 2018
 *     Author: Kyle
 */


#include "BiomeMap.h"
#include <math.h>
#include "simplexnoise.h"


void BiomeMap::gen_moisture_cell(int cellX, int cellY)
{
  float octaves = 6.0;
  float persistence = 0.5;
  float scale = 0.25;
  float loBound = -4.0;
  float hiBound = 4.0;
  for (int i = 0; i < 65; i++)
  {
    float x = i / 64.0 + cellY;
    for (int j = 0; j < 65; j++)
    {
      float y = j / 64.0 + cellX;
      float value = scaled_octave_noise_2d(octaves, persistence, scale,
          loBound, hiBound, i, j);
      this->moisturemap[i][j] = round(value);
    }
  }
}


void BiomeMap::gen_temperature_cell(int cellX, int cellY)
{
  float octaves = 6.0;
  float persistence = 0.5;
  float scale = 0.25;
  float loBound = -4.0;
  float hiBound = 4.0;
  for (int i = 0; i < 65; i++)
  {
    float x = i / 64.0 + cellY;
    for (int j = 0; j < 65; j++)
    {
      float y = j / 64.0 + cellX;
      float value = scaled_octave_noise_2d(octaves, persistence, scale,
          loBound, hiBound, i, j);
      this->temperaturemap[i][j] = round(value);
    }
  }
}
