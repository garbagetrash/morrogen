#include <stdint.h>
#include <vector>

namespace Bsa {

typedef struct {
  uint32_t magic_number;
  uint32_t hash_offset;
  uint32_t num_files;
} BsaHeader;

class BsaSubFile {
public:
  BsaSubFile() : size_bytes(0), offset_bytes(0), name_offset(0), filename_hash(0), name(std::string("")), raw_data(std::vector<uint8_t>()) {};

  uint32_t size_bytes;
  uint32_t offset_bytes;
  uint32_t name_offset;
  uint64_t filename_hash;
  std::string name;
  std::vector<uint8_t> raw_data;
};

typedef struct {
  BsaHeader header;
  std::vector<BsaSubFile> subfiles;
} bsa_t;

// Function prototypes
bsa_t load_bsa_metadata(const std::string filename, bool verbose);
void print_file_names(const bsa_t &bsa_object);

} // namespace Bsa
