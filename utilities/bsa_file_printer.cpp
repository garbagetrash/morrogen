#include <iostream>
#include "BsaLoader.hpp"

using namespace Bsa;

int main(int argc, char *argv[]) {

  // Input parameter checking
  if (argc < 2) {
    std::cout << "Usage: ./bsa_file_printer <bsa_archive.bsa>" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Load the specified BSA
  bsa_t bsa = Bsa::load_bsa_metadata(std::string(argv[1]), true);

  // Now print the files in the archive
  print_file_names(bsa);
}
