#include <filesystem>

// font info
struct fontinfo {
  std::filesystem::path path;
  int height;
  int width;
  int chars_count;
};

const fontinfo FONT = {std::filesystem::path(AP_PATH) / "alt-8x16.psf", 16, 8, 256};
