#ifndef TLO_CPP_FILESYSTEM_HPP
#define TLO_CPP_FILESYSTEM_HPP

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
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

// Will not include duplicate paths and will preserve order of paths. Throws
// std::runtime_error if a string refers to a path that doesn't exist. Gets the
// canonical path of each constructed path using std::filesystem::canonical() to
// help with removing duplicate paths. If pathType is PathType::INPUT, will add
// the constructed paths themselves to the container of paths. If pathType is
// PathType::CANONICAL, will add the canonical paths returned by
// std::filesystem::canonical() instead.
std::vector<std::filesystem::path> stringsToPaths(
    const std::vector<std::string> &strings,
    PathType pathType = PathType::INPUT);

// Like above but will retrieve canonical paths from canonicalPaths. If
// canonicalPaths does not contain the canonical path for a path, will use
// std::filesystem::canonical() and insert the canonical path to canonicalPaths.
std::vector<std::filesystem::path> stringsToPaths(
    const std::vector<std::string> &strings,
    std::unordered_map<std::filesystem::path, std::filesystem::path, HashPath>
        &canonicalPaths,
    PathType pathType = PathType::INPUT);

// If all paths in container are files, returns (true, end iterator). Otherwise,
// returns (false, iterator to non-file path).
template <class PathContainer,
          class Iterator = typename PathContainer::const_iterator>
std::pair<bool, Iterator> allFiles(const PathContainer &paths) {
  Iterator iterator = std::find_if_not(
      paths.cbegin(), paths.cend(),
      [](const auto &path) { return std::filesystem::is_regular_file(path); });

  return std::pair(iterator == paths.cend(), iterator);
}

// If all paths in container are files or directories, returns (true, end
// iterator). Otherwise, returns (false, iterator to non-file and non-directory
// path).
template <class PathContainer,
          class Iterator = typename PathContainer::const_iterator>
std::pair<bool, Iterator> allFilesOrDirectories(const PathContainer &paths) {
  Iterator iterator =
      std::find_if_not(paths.cbegin(), paths.cend(), [](const auto &path) {
        return std::filesystem::is_regular_file(path) ||
               std::filesystem::is_directory(path);
      });

  return std::pair(iterator == paths.cend(), iterator);
}

// Assumes paths contains paths to files or directories. Throws
// std::runtime_error if a path is not a file or directory. If a path is a file,
// adds it the file list. If a path is a directory, adds to the file list all
// files in the directory and all its subdirectories. Returns the resulting file
// list. Also, will not add duplicate paths and will preserve order of paths. If
// pathsAreCanonical is true, will assume paths contains only canonical paths.
// If pathsAreCanonical is false, gets the canonical path of each path using
// std::filesystem::canonical() to help with removing duplicate paths.
std::vector<std::filesystem::path> buildFileList(
    const std::vector<std::filesystem::path> &paths,
    bool pathsAreCanonical = false);

// Like above but will retrieve canonical paths from canonicalPaths. If
// canonicalPaths does not contain the canonical path for a path, will use
// std::filesystem::canonical() and insert the canonical path to canonicalPaths.
std::vector<std::filesystem::path> buildFileList(
    const std::vector<std::filesystem::path> &paths,
    std::unordered_map<std::filesystem::path, std::filesystem::path, HashPath>
        &canonicalPaths,
    bool pathsAreCanonical = false);
}  // namespace tlo

#endif  // TLO_CPP_FILESYSTEM_HPP
