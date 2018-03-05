typedef struct Object {
  std::string Id;
  float zOffset;
} Object;

class TreeSets {
public:
  static const std::vector<Object> BC;
};

const std::vector<Object> TreeSets::BC = {
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
