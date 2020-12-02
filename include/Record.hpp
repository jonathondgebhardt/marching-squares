#pragma once

#include <cmath>
#include <ostream>
#include <string>
#include <vector>

struct Record
{
  bool isValid() const;
  bool numElevationsMatch() const;
  bool minElevationsMatch() const;
  bool maxElevationsMatch() const;

  void dump(std::ostream& os, bool verbose = false) const;

  std::vector<double> elevations;
  double beginLatitude{-1.0};
  double beginLongitude{-1.0};
  double minimumElevation{-1.0};
  double maximumElevation{-1.0};
  int numElevations{-1};
  size_t recordId{0};

private:
  inline bool doubleEquals(double x, double y) const
  {
    return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
  };
};