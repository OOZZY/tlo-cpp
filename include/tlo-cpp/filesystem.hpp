#ifndef TLO_CPP_FILESYSTEM_HPP
#define TLO_CPP_FILESYSTEM_HPP

#include <cstdint>
#include <ctime>
#include <filesystem>
#include <string>
#include <vector>

namespace tlo {
// On MinGW-w64, sometimes std::filesystem::file_size() returns the wrong size
// for large files. Returns file size. Throws std::runtime_error on error.
std::uintmax_t getFileSize(const std::filesystem::path &filePath);

std::time_t getLastWriteTime(const std::filesystem::path &path);

struct HashPath {
  std::size_t operator()(const std::filesystem::path &path) const;
};

enum class PathType { INPUT, CANONICAL };

// Also removes duplicate paths while preserving order of items. Throws
// std::runtime_error if a string refers to a path that doesn't exist. If
// pathType is PathType::INPUT, will not further modify paths after they have
// been constructed. If pathType is PathType::CANONICAL, will convert each
// constructed path to a canonical path using std::filesystem::canonical().
std::vector<std::filesystem::path> stringsToPaths(
    const std::vector<std::string> &strings,
    PathType pathType = PathType::INPUT);
}  // namespace tlo

#endif  // TLO_CPP_FILESYSTEM_HPP
