#include <algorithm>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Record
{
  bool isValid() const
  {
    const auto elevationsMatch = this->elevations.size() == this->numElevations;

    const auto [minIt, maxIt] =
        std::minmax_element(this->elevations.begin(), this->elevations.end());
    assert(minIt != this->elevations.end());
    assert(maxIt != this->elevations.end());

    const auto doubleEquals = [](double x, double y) -> bool {
      return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
    };

    const auto maxElevationMatch = doubleEquals(this->maximumElevation, *maxIt);
    const auto minElevationMatch = doubleEquals(this->minimumElevation, *minIt);

    return elevationsMatch && maxElevationMatch && minElevationMatch;
  }

  void dump(bool verbose = false) const
  {
    std::cout << "numElevations: " << this->numElevations << std::endl;
    std::cout << "elevations.size(): " << this->elevations.size() << std::endl;
    std::cout << "begin lat (degrees): " << this->beginLatitude << std::endl;
    std::cout << "begin lon (degrees): " << this->beginLongitude << std::endl;
    std::cout << "min elevation: " << this->minimumElevation << std::endl;
    std::cout << "max elevation: " << this->maximumElevation << std::endl;

    if (verbose)
    {
      std::copy(this->elevations.begin(), this->elevations.end(),
                std::ostream_iterator<double>(std::cout, "\n"));
    }
  }

  std::vector<double> elevations;
  double beginLatitude;
  double beginLongitude;
  double minimumElevation;
  double maximumElevation;
  size_t numElevations;
};

std::optional<double> scientific2double(const std::string &x, bool offByOne = true)
{
  std::optional<double> number;

  try
  {
    double d;

    {
      std::stringstream ss(x);
      ss >> d;
      ss.clear();
    }

    const auto powerStrIndex = x.find("+");
    if (powerStrIndex != std::string::npos)
    {
      const auto powerStr = x.substr(powerStrIndex + 1, x.size());
      int power;
      std::stringstream ss(powerStr);
      ss << powerStr;
      ss >> power;

      // Why do we have to add one?
      int offByOneVal = offByOne ? 1 : 0;

      d *= std::pow(10, power + offByOneVal);
    }

    number = d;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << ": could not parse '" << x << "'" << '\n';
    number = std::nullopt;
  }

  return number;
}

std::vector<Record> makeRecords(const std::vector<std::string> &x)
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
    catch (const std::exception &e)
    {
      std::cerr << e.what() << ": could not parse number of elevations" << std::endl;
      numElevationsOpt = std::nullopt;
    }

    // This conversion doesn't seem to be correct.
    // 0.242970000000000D+06 -> 242970 -> 67.4917
    // 0.440412000000000D+07 -> 4.40412e+06 -> 1223.37
    // The latitude is within range [-90, 90] but is much farther North than expected. The longitude
    // is well outside range [-180, 180].
    const auto arcSeconds2Degrees = [](const auto &x) -> double {
      const auto arcSeconds = scientific2double(x, false);
      if (arcSeconds.has_value())
      {
        std::cout << arcSeconds.value() << std::endl;
        return arcSeconds.value() / 3600.0;
      }

      return -1.0;
    };

    constexpr auto beginLatitudeOffset = 4;
    r.beginLatitude = arcSeconds2Degrees(*(it + beginLatitudeOffset));

    constexpr auto beginLongitudeOffset = 5;
    r.beginLongitude = arcSeconds2Degrees(*(it + beginLongitudeOffset));

    constexpr auto minimumElevationOffset = 7;
    r.minimumElevation = scientific2double(*(it + minimumElevationOffset)).value_or(-1.0);

    constexpr auto maximumElevationOffset = 8;
    r.maximumElevation = scientific2double(*(it + maximumElevationOffset)).value_or(-1.0);

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
                       [](const auto &x) { return std::stoi(x); });

        r.elevations = elevations;
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << ": could not parse elevations" << std::endl;
        r.numElevations = 0;
        r.elevations.clear();
      }

      assert(r.isValid());

      records.push_back(r);

      it = endIt;
    }
  }

  return records;
}

std::vector<std::string> split(const std::string &x, bool dump = false)
{
  std::stringstream ss(x);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> tokens(begin, end);

  if (dump == true)
  {
    std::cout << std::endl;
    std::copy(tokens.begin(), tokens.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
  }

  return tokens;
}

int main(int argc, char *argv[])
{
  std::vector<Record> records;

  const auto terrainPath = "../terrain/test_big.dem";
  std::ifstream ifs(terrainPath, std::ios::in);
  if (ifs.is_open() == true)
  {
    // Records are in multiples of 1024 bytes
    constexpr auto bufferSize = 1024;

    // Skip the "A" Record
    ifs.seekg(bufferSize);

    std::vector<std::string> contents;

    for (std::string line; std::getline(ifs, line);)
    {
      const auto tokens = split(line);
      std::copy(tokens.begin(), tokens.end(), std::back_inserter(contents));
    }

    const auto records = makeRecords(contents);
    for (const auto &r : records)
    {
      r.dump();
    }
  }
}