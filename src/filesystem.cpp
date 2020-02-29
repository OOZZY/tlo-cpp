#include "tlo-cpp/filesystem.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_set>

#include "tlo-cpp/chrono.hpp"

namespace fs = std::filesystem;

namespace tlo {
std::uintmax_t getFileSize(const fs::path &filePath) {
  std::ifstream ifstream(filePath, std::ifstream::in | std::ifstream::binary);

  if (!ifstream.is_open()) {
    throw std::runtime_error("Error: Failed to open \"" + filePath.string() +
                             "\".");
  }

  ifstream.seekg(0, std::ifstream::end);

  auto size = ifstream.tellg();

  if (size < 0) {
    throw std::runtime_error("Error: Failed to get size of \"" +
                             filePath.string() + "\".");
  }

  return static_cast<std::uintmax_t>(size);
}

std::time_t getLastWriteTime(const fs::path &path) {
  auto timeOnFileClock = fs::last_write_time(path);
  auto timeOnSystemClock =
      convertTimePoint<std::chrono::system_clock::time_point>(timeOnFileClock);

  return std::chrono::system_clock::to_time_t(timeOnSystemClock);
}

std::size_t HashPath::operator()(const fs::path &path) const {
  return fs::hash_value(path);
}

std::vector<fs::path> stringsToPaths(const std::vector<std::string> &strings,
                                     PathType pathType) {
  std::vector<fs::path> paths;
  std::unordered_set<fs::path, HashPath> pathsAdded;

  for (const auto &string : strings) {
    fs::path path = string;

    if (!fs::exists(path)) {
      throw std::runtime_error("Error: Path \"" + string +
                               "\" does not exist.");
    }

    fs::path canonicalPath = fs::canonical(path);

    if (pathsAdded.find(canonicalPath) == pathsAdded.end()) {
      if (pathType == PathType::CANONICAL) {
        paths.push_back(canonicalPath);
      } else {
        paths.push_back(std::move(path));
      }

      pathsAdded.insert(std::move(canonicalPath));
    }
  }

  return paths;
}

std::vector<fs::path> buildFileList(const std::vector<fs::path> &paths) {
  std::vector<fs::path> fileList;
  std::unordered_set<fs::path, HashPath> pathsAdded;

  for (const auto &path : paths) {
    if (fs::is_regular_file(path)) {
      if (pathsAdded.find(path) == pathsAdded.end()) {
        fileList.push_back(path);
        pathsAdded.insert(path);
      }
    } else if (fs::is_directory(path)) {
      // When compiled with MSVC++ on Windows, accessing iterator->path() might
      // throw an exception if the path contains certain Unicode characters.
      // This exception can't be caught when using a ranged-based for loop, so
      // manual iteration is done instead.
      for (fs::recursive_directory_iterator iterator(path);
           iterator != fs::end(iterator); iterator++) {
        bool isFile;

        try {
          isFile = fs::is_regular_file(iterator->path());
        } catch (...) {
          continue;
        }

        if (isFile) {
          if (pathsAdded.find(iterator->path()) == pathsAdded.end()) {
            fileList.push_back(iterator->path());
            pathsAdded.insert(iterator->path());
          }
        }
      }
    } else {
      throw std::runtime_error("Error: \"" + path.string() +
                               "\" is not a file or directory.");
    }
  }

  return fileList;
}
}  // namespace tlo
