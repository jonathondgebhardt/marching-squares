#pragma once

#include <algorithm>
#include <cmath>
#include <ostream>
#include <string>
#include <vector>

struct Record
{
  bool isValid() const;
  void dump(std::ostream& stream, bool verbose = false) const;

  static std::vector<Record> MakeRecords(const std::vector<std::string>& x);
  template <class T> static std::optional<T> stringTo(const std::string& x);

  std::vector<double> elevations;
  double beginLatitude{-1.0};
  double beginLongitude{-1.0};
  double minimumElevation{-1.0};
  double maximumElevation{-1.0};
  size_t numElevations{0};
  size_t recordId{0};
};