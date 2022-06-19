#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#include "BsaLoader.hpp"

namespace Bsa {

Bsa load_bsa_metadata(const std::string filename, bool verbose) {
  std::fstream bsa_file;
  bsa_file.open(filename, std::ios::in);
  if (!bsa_file) {
    std::invalid_argument("File does not exist");
  }
  Bsa output;

  // Read the header to get the number of files present
  bsa_file.read((char *)(&output.header), sizeof(BsaHeader));

  if (verbose) {
    std::cout << "magic number: 0x" << std::hex << output.header.magic_number << std::endl;
    std::cout << "hash offset: 0x" << std::hex << output.header.hash_offset << std::endl;
    std::cout << "num files: " << std::dec << output.header.num_files << std::endl;
  }

  // Reserve memory for the sub files, then push back some empty instances
  output.subfiles.reserve(output.header.num_files);
  for (size_t i = 0; i < output.header.num_files; i++) {
    BsaSubFile temp;
    output.subfiles.push_back(temp);
  }

  // Populate subfiles filesize data
  for (size_t i = 0; i < output.header.num_files; i++) {
    bsa_file.read((char *)(&output.subfiles[i].size_bytes), sizeof(uint32_t));
    std::cout << bsa_file.tellg() << std::endl;
    bsa_file.read((char *)(&output.subfiles[i].offset_bytes), sizeof(uint32_t));
    std::cout << bsa_file.tellg() << std::endl;
    if (verbose) {
      std::cout << "subrecord " << std::dec << i << std::endl;
      std::cout << "size bytes: 0x" << std::hex << output.header.hash_offset << std::endl;
      std::cout << "offset bytes: " << std::dec << output.header.num_files << std::endl;
    }
  }

  // Next populate the name offsets
  for (size_t i = 0; i < output.header.num_files; i++) {
    bsa_file >> output.subfiles[i].name_offset;
  }

  // Names
  for (size_t i = 0; i < output.header.num_files; i++) {
    // TODO: Does this call advance the cursor in `bsa_file`?
    std::getline(bsa_file, output.subfiles[i].name);
  }

  // Filename hashes
  for (size_t i = 0; i < output.header.num_files; i++) {
    bsa_file >> output.subfiles[i].filename_hash;
  }

  /*
  // Raw file data
  for (size_t i = 0; i < output.header.num_files; i++) {
    output.subfiles[i].raw_data.resize(output.subfiles[i].size_bytes);
    bsa_file.read((char *)output.subfiles[i].raw_data.data(), output.subfiles[i].size_bytes);
  }
  */

  return output;
}

void print_file_names(const Bsa &bsa_object) {
  for (const BsaSubFile &subfile : bsa_object.subfiles) {
    std::cout << subfile.name << std::endl;
  }
}

} // namespace Bsa
