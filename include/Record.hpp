#pragma once

#include <ostream>
#include <string>
#include <vector>

struct Record
{
  bool isValid() const;
  void dump(std::ostream& os, bool verbose = false) const;

  std::vector<double> elevations;
  double beginLatitude{-1.0};
  double beginLongitude{-1.0};
  double minimumElevation{-1.0};
  double maximumElevation{-1.0};
  size_t numElevations{0};
  size_t recordId{0};
};