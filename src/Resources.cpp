#include "Resources.h"

#include <string>
#include <vector>

// Ascadian Isles
const std::vector<StaticObject> TreeSets::AI = {
  {"flora_tree_ai_01", 260.0},
  {"flora_tree_ai_02", 480.0},
  {"flora_tree_ai_03", 375.0},
  {"flora_tree_ai_04", 630.0},
  {"flora_tree_ai_05", 350.0},
  {"flora_tree_ai_06", 460.0},
  {"flora_emp_parasol_01", 315.0},
  {"flora_emp_parasol_02", 430.0},
  {"flora_emp_parasol_03", 330.0},
};

const std::vector<StaticObject> RockSets::AI = {
  {"ex_ai_boulder06", 10.0},
  {"ex_ai_boulder07", 10.0},
  {"ex_ai_boulder08", 10.0},
  {"Terrain_rocks_AI_01", 10.0},
  {"Terrain_rocks_AI_03", 10.0},
  {"terrain_rock_ai_01", 10.0},
  {"terrain_rock_ai_02", 10.0},
  {"terrain_rock_ai_03", 10.0},
  {"terrain_rock_ai_04", 10.0},
  {"terrain_rock_ai_05", 10.0},
  {"terrain_rock_ai_06", 10.0},
  {"terrain_rock_ai_07", 10.0},
  {"terrain_rock_ai_08", 10.0},
  {"terrain_rock_ai_09", 10.0},
  {"terrain_rock_ai_10", 10.0},
  {"terrain_rock_ai_11", 10.0},
  {"terrain_rock_ai_12", 10.0},
};

const std::vector<LtexPair> TextureSets::AI = {
  {"Tx_AI_mainroad_01.tga", "Tx_AI_mainroad_01.tga"},
  {"Tx_AI_mudflats_01.tga", "Tx_AI_mudflats_01.tga"},
  {"AI_Tilled_Dirt", "Tx_AI_tilled_dirt_01.tga"},
  {"AI_Grass_02", "Tx_AI_grass_01.tga"},
  {"AI_Clover", "Tx_AI_clover_02.tga"},
  {"AI_Dirt", "Tx_AI_dirt_01.tga"},
  {"AI_Dirtroad", "Tx_AI_dirtroad_01.tga"},
  {"AI_Grass_Dirt", "Tx_AI_grass_dirt_01.tga"},
  {"AI_Grass", "Tx_grass_01.tga"},
  {"AI_Grass_Rocky", "Tx_grass_rocky_01.tga"},
  {"AI_Grass_Cobbles", "Tx_cobblestone_01.tga"},
};

const std::vector<StaticObject> BuildingSets::Shanty {
  {"ex_de_shack_01", 130.0},
  {"ex_de_shack_02", 120.0},
  {"ex_de_shack_03", 105.0},
  {"ex_de_shack_04",   0.0},
  {"ex_de_shack_05", -60.0},
};

// Bitter Coast
const std::vector<StaticObject> TreeSets::BC = {
  {"flora_bc_tree_01", 440.0},
  {"flora_bc_tree_02", 510.0},
  {"flora_bc_tree_03", 450.0},
  {"flora_bc_tree_04", 390.0},
  {"flora_bc_tree_05", 340.0},
  {"flora_bc_tree_06", 330.0},
  {"flora_bc_tree_07", 330.0},
  {"flora_bc_tree_08", 350.0},
  {"flora_bc_tree_09", 160.0},
  {"flora_bc_tree_10", 210.0},
  {"flora_bc_tree_11", 80.0},
  {"flora_bc_tree_12", 30.0},
  {"flora_bc_tree_13", 70.0},
};

const std::vector<StaticObject> RockSets::BC = {
  {"terrain_rock_gl_01", 10.0},
};

const std::vector<LtexPair> TextureSets::BC = {
  {"Tx_BC_mainroad_01.tga", "Tx_BC_mainroad_01.tga"},
  {"Sand_02",               "Tx_sand_02.tga"},
  {"Tx_BC_grass.tga",       "Tx_BC_grass.tga"},
  {"Tx_BC_muck.tga",        "Tx_BC_muck.tga"},
  {"Tx_BC_muck_01.tga",     "Tx_BC_muck_01.tga"},
  {"Tx_BC_mud.tga",         "Tx_BC_mud.tga"},
  {"Tx_BC_undergrowth.tga", "Tx_BC_undergrowth.tga"},
  {"Tx_BC_moss.tga",        "Tx_BC_moss.tga"},
  {"Tx_BC_scrub.tga",       "Tx_BC_scrub.tga"},
  {"Tx_BC_dirt.tga",        "Tx_BC_dirt.tga"},
  {"Tx_BC_bank.tga",        "Tx_BC_bank.tga"},
  {"Tx_BC_rock_01.tga",     "Tx_BC_rock_01.tga"},
  {"Tx_BC_rock_02.tga",     "Tx_BC_rock_02.tga"},
  {"Tx_BC_rock_03.tga",     "Tx_BC_rock_03.tga"},
  {"Tx_BC_rock_04.tga",     "Tx_BC_rock_04.tga"},
  {"Tx_BC_rockyscrub.tga",  "Tx_BC_rockyscrub.tga"},
};

// Grazelands
const std::vector<StaticObject> TreeSets::GL = {
  {"flora_tree_gl_01", 350.0},
  {"flora_tree_gl_02", 450.0},
  {"flora_tree_gl_03", 560.0},
  {"flora_tree_gl_04", 350.0},
  {"flora_tree_gl_05", 480.0},
  {"flora_tree_gl_06", 580.0},
  {"flora_tree_gl_07", 280.0},
  {"flora_tree_gl_08", 580.0},
  {"flora_tree_gl_09", 1140.0},
  {"flora_tree_gl_10", 280.0},
  {"flora_tree_gl_11", 470.0},
};

const std::vector<StaticObject> RockSets::GL = {
  {"terrain_rock_gl_01", 10.0},
  {"terrain_rock_gl_02", 30.0},
  {"terrain_rock_gl_03", 20.0},
  {"terrain_rock_gl_04", 20.0},
  {"terrain_rock_gl_05", 10.0},
  {"terrain_rock_gl_06", 50.0},
  {"terrain_rock_gl_07", 70.0},
  {"terrain_rock_gl_08", 80.0},
  {"terrain_rock_gl_09", 40.0},
  {"terrain_rock_gl_10", 120.0},
  {"terrain_rock_gl_11", 60.0},
  {"terrain_rock_gl_12", 40.0},
};

const std::vector<LtexPair> TextureSets::GL = {
  {"GL_Grass_01", "Tx_GL_grass_01.tga"},
  {"GL_Dirt_01", "Tx_GL_dirt_01.tga"},
  {"GL_Dirt_02", "Tx_GL_dirt_02.tga"},
  {"GL_Dirt_Grass_01", "Tx_GL_dirt_grass_01.tga"},
  {"GL_Dirt_Grass_02", "Tx_GL_dirt_grass_02.tga"},
  {"GL_Dirtroad", "Tx_GL_dirtroad_01.tga"},
  {"GL_Grass_02", "Tx_GL_grass_02.tga"},
  {"GL_Grass_03", "Tx_GL_grass_07.tga"},
  {"GL_Grass_05", "Tx_GL_grass_12.tga"},
  {"GL_Rock_01",  "Tx_GL_rock_01.tga"},
  {"GL_Rock_02",  "Tx_GL_rock_02.tga"},
};
