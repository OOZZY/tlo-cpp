#ifndef TLO_CPP_CONTAINER_HPP
#define TLO_CPP_CONTAINER_HPP

namespace tlo {
template <class KeySequence, class Map>
KeySequence getKeySequence(const Map &map) {
  KeySequence keySequence;

  for (const auto &pair : map) {
    keySequence.push_back(pair.first);
  }

  return keySequence;
}

template <class ValueSequence, class Map>
ValueSequence getValueSequence(const Map &map) {
  ValueSequence valueSequence;

  for (const auto &pair : map) {
    valueSequence.push_back(pair.second);
  }

  return valueSequence;
}

template <class KeySet, class Map>
KeySet getKeySet(const Map &map) {
  KeySet keySet;

  for (const auto &pair : map) {
    keySet.insert(pair.first);
  }

  return keySet;
}

template <class ValueSet, class Map>
ValueSet getValueSet(const Map &map) {
  ValueSet valueSet;

  for (const auto &pair : map) {
    valueSet.insert(pair.second);
  }

  return valueSet;
}
}  // namespace tlo

#endif  // TLO_CPP_CONTAINER_HPP
