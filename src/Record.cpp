#include "Record.hpp"

#include <iostream>
#include <iterator>
#include <sstream>

bool Record::isValid() const
{
  const auto elevationsMatch = this->elevations.size() == this->numElevations;

  const auto [minIt, maxIt] = std::minmax_element(this->elevations.begin(), this->elevations.end());

  const auto doubleEquals = [](double x, double y) -> bool {
    return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
  };

  const auto maxElevationMatch = doubleEquals(this->maximumElevation, *maxIt);
  const auto minElevationMatch = doubleEquals(this->minimumElevation, *minIt);

  return elevationsMatch && maxElevationMatch && minElevationMatch;
}

void Record::dump(std::ostream& os, bool verbose) const
{
  os << "Record " << this->recordId << ":" << std::endl;
  os << "\tsize in file vs parsed size: " << this->numElevations;
  os << ", " << this->elevations.size() << std::endl;
  os << "\tbegin lat (arc seconds): " << this->beginLatitude << std::endl;
  os << "\tbegin lon (arc seconds): " << this->beginLongitude << std::endl;
  os << "\tmin elevation (m): " << this->minimumElevation << std::endl;
  os << "\tmax elevation (m): " << this->maximumElevation << std::endl;

  if(verbose)
  {
    // Unfortunately, the second parametre to the ostream_iterator is treated as a postfix. Write a
    // tab to the output stream so that first elevation matches the column number of the rest.
    os << "\t----------------\n\t";
    std::copy(this->elevations.begin(), this->elevations.end(),
              std::ostream_iterator<double>(os, "\n\t"));
  }
}
