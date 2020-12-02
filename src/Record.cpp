#include <Record.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <sstream>

bool Record::isValid() const
{
  if(this->elevations.empty())
  {
    return false;
  }

  return this->numElevationsMatch() && this->minElevationsMatch() && maxElevationsMatch();
}

bool Record::numElevationsMatch() const { return this->elevations.size() == this->numElevations; }

bool Record::minElevationsMatch() const
{
  const auto minIt = std::min_element(this->elevations.begin(), this->elevations.end());
  if(minIt != this->elevations.end())
  {
    return this->doubleEquals(this->minimumElevation, *minIt);
  }

  return false;
}

bool Record::maxElevationsMatch() const
{
  const auto maxIt = std::max_element(this->elevations.begin(), this->elevations.end());
  if(maxIt != this->elevations.end())
  {
    return this->doubleEquals(this->maximumElevation, *maxIt);
  }

  return false;
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
