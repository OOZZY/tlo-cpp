#include "tlo-cpp/hash.hpp"

namespace tlo {
// Hash combining algorithm from web page "List (Java Platform SE 8)" available
// at: https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
// (Retrieved February 11, 2020)
JavaStyleHashCombiner &JavaStyleHashCombiner::combineWith(
    std::size_t nextHash) {
  hash = 31 * hash + nextHash;
  return *this;
}

std::size_t JavaStyleHashCombiner::getHash() { return hash; }

// Hash combining algorithm from web page "Header <boost/functional/hash.hpp>"
// available at:
// https://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
// (Retrieved February 11, 2020)
BoostStyleHashCombiner &BoostStyleHashCombiner::combineWith(
    std::size_t nextHash) {
  hash ^= nextHash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  return *this;
}

std::size_t BoostStyleHashCombiner::getHash() { return hash; }
}  // namespace tlo
