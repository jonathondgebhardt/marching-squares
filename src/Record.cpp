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
  os << "file size: " << this->numElevations << std::endl;
  os << "parsed size: " << this->elevations.size() << std::endl;
  os << "begin lat (arc seconds): " << this->beginLatitude << std::endl;
  os << "begin lon (arc seconds): " << this->beginLongitude << std::endl;
  os << "min elevation: " << this->minimumElevation << std::endl;
  os << "max elevation: " << this->maximumElevation << std::endl;

  if (verbose)
  {
    std::copy(this->elevations.begin(), this->elevations.end(),
              std::ostream_iterator<double>(os, "\n"));
  }
}

std::vector<Record> Record::MakeRecords(const std::vector<std::string>& x)
{
  std::vector<Record> records;

  auto it = x.begin();
  while (it != x.end())
  {
    Record r;

    std::optional<double> numElevationsOpt;

    try
    {
      constexpr auto numElevationsOffset = 2;
      numElevationsOpt = std::stoi(*(it + numElevationsOffset));
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << ": could not parse number of elevations" << std::endl;
      numElevationsOpt = std::nullopt;
    }

    // The conversion described in the standard doesn't make sense.
    // 0.242970000000000D+06 -> 242970 -> 67.4917
    // 0.440412000000000D+07 -> 4.40412e+06 -> 1223.37
    // The latitude is within range [-90, 90] but is much farther North than expected. The longitude
    // is well outside range [-180, 180].
    // We'll just store fractional arc seconds for now.

    constexpr auto beginLatitudeOffset = 4;
    r.beginLatitude = stringTo<double>(*(it + beginLatitudeOffset)).value_or(-1.0);

    constexpr auto beginLongitudeOffset = 5;
    r.beginLongitude = stringTo<double>(*(it + beginLongitudeOffset)).value_or(-1.0);

    const auto stringToElevation = [](const auto& x) -> double {
      const auto powerStrIndex = x.find("+");
      if (powerStrIndex != std::string::npos)
      {
        const auto powerStr = x.substr(powerStrIndex + 1, x.size());

        try
        {
          int power = std::stoi(powerStr);

          const auto elevationOpt = stringTo<double>(x);
          if (elevationOpt.has_value())
          {
            // Why do we have to add one?
            return elevationOpt.value() * std::pow(10, power + 1);
          }
        }
        catch (const std::exception& e)
        {
          std::cerr << e.what() << ": could not convert power substring for '" << x << "'"
                    << std::endl;
        }
      }

      return -1.0;
    };

    constexpr auto minimumElevationOffset = 7;
    r.minimumElevation = stringToElevation(*(it + minimumElevationOffset));

    constexpr auto maximumElevationOffset = 8;
    r.maximumElevation = stringToElevation(*(it + maximumElevationOffset));

    if (numElevationsOpt.has_value())
    {
      r.numElevations = numElevationsOpt.value();

      constexpr auto elevationsOffset = 9;
      const auto beginIt = it + elevationsOffset;
      const auto endIt = beginIt + r.numElevations;

      std::vector<double> elevations;

      try
      {
        std::transform(beginIt, endIt, std::back_inserter(elevations),
                       [](const auto& x) { return std::stoi(x); });

        r.elevations = elevations;
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << ": could not parse elevations" << std::endl;
        r.numElevations = 0;
        r.elevations.clear();
      }

      records.push_back(r);

      it = endIt;
    }
  }

  return records;
}

template <class T> std::optional<T> Record::stringTo(const std::string& x)
{
  std::optional<T> resultOpt;

  try
  {
    T result;
    std::stringstream ss(x);
    ss >> result;

    resultOpt = result;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Could not convert " << x << " to requested type" << std::endl;
  }

  return resultOpt;
}
