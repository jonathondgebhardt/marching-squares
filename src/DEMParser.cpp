#include <DEMParser.ipp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>

std::vector<Record> DEMParser::MakeRecords(const std::string& x)
{
  const auto tokens = DEMParser::Parse(x);

  std::vector<Record> records;
  size_t recordId = 1;

  auto it = tokens.begin();
  while(it != tokens.end())
  {
    Record r;
    r.recordId = recordId++;

    std::optional<double> numElevationsOpt;

    try
    {
      constexpr auto numElevationsOffset = 2;
      numElevationsOpt = std::stoi(*(it + numElevationsOffset));
    }
    catch(const std::exception& e)
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
    r.beginLatitude = DEMParser::StringTo<double>(*(it + beginLatitudeOffset)).value_or(-1.0);

    constexpr auto beginLongitudeOffset = 5;
    r.beginLongitude = DEMParser::StringTo<double>(*(it + beginLongitudeOffset)).value_or(-1.0);

    const auto stringToElevation = [](const auto& x) -> double {
      const auto powerStrIndex = x.find("+");
      if(powerStrIndex != std::string::npos)
      {
        const auto powerStr = x.substr(powerStrIndex + 1, x.size());

        try
        {
          int power = std::stoi(powerStr);

          const auto elevationOpt = DEMParser::StringTo<double>(x);
          if(elevationOpt.has_value())
          {
            // Why do we have to add one?
            return elevationOpt.value() * std::pow(10, power + 1);
          }
        }
        catch(const std::exception& e)
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

    if(numElevationsOpt.has_value())
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
      catch(const std::exception& e)
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

std::vector<std::string> DEMParser::Parse(const std::string& x)
{
  std::vector<std::string> contents;

  std::ifstream ifs(x, std::ios::in);
  if(ifs.is_open() == true)
  {
    // Records are in multiples of 1024 bytes
    constexpr auto bufferSize = 1024;

    // Skip the "A" Record
    ifs.seekg(bufferSize);

    const auto split = [](const std::string& x) -> std::vector<std::string> {
      std::stringstream ss(x);
      std::istream_iterator<std::string> begin(ss);
      std::istream_iterator<std::string> end;

      return {begin, end};
    };

    for(std::string line; std::getline(ifs, line);)
    {
      const auto tokens = split(line);
      std::copy(tokens.begin(), tokens.end(), std::back_inserter(contents));
    }
  }

  return contents;
}
