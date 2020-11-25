#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Record
{
  bool isValid() const { return this->elevations.size() == this->numElevations; }
  void dump() const
  {
    std::cout << "numElevations: " << this->numElevations << std::endl;
    std::cout << "elevations.size(): " << this->elevations.size() << std::endl;
    // std::copy(this->elevations.begin(), this->elevations.end(),
    //           std::ostream_iterator<std::string>(std::cout, "\n"));

    // for (size_t i = 0; i < this->elevations.size(); ++i)
    // {
    //   std::cout << "\t[" << i << "]: " << this->elevations[i] << std::endl;
    // }
  }

  std::vector<double> elevations;
  size_t numElevations;
};

std::vector<double> extractElevations(const std::vector<std::string> &x, size_t offset = 0)
{
  std::vector<double> elevations;

  for (size_t i = offset; i < x.size(); ++i)
  {
    try
    {
      elevations.push_back(std::stof(x[i]));
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << ": could not interpret elevation" << std::endl;
    }
  }

  return elevations;
}

Record makeRecord(const std::vector<std::string> &x)
{
  Record r;

  try
  {
    r.numElevations = std::stoi(x[2]);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << ": could not interpret number of elevations" << std::endl;
  }

  constexpr size_t beginElevationIndex = 9;
  const auto elevations = extractElevations(x, beginElevationIndex);
  std::copy(elevations.begin(), elevations.end(), std::back_inserter(r.elevations));

  return r;
}

std::vector<Record> makeRecords(const std::vector<std::string> &x)
{
  std::vector<Record> records;

  auto it = x.begin();
  while (it != x.end())
  {
    try
    {
      Record r;

      constexpr auto numElevationsOffset = 2;
      auto numElevationsIt = it + numElevationsOffset;
      const auto &numElevationsStr = *numElevationsIt;
      const auto numElevations = std::stoi(numElevationsStr);

      r.numElevations = numElevations;

      std::vector<double> elevations;

      constexpr auto elevationsOffset = 9;
      auto beginIt = it + elevationsOffset;
      auto endIt = beginIt + numElevations;
      std::transform(beginIt, endIt, std::back_inserter(elevations),
                     [](const auto &x) { return std::stoi(x); });

      r.elevations = elevations;
      //   r.dump();

      records.push_back(r);

      it = endIt + 1;

      std::cout << "reiterating at " << std::distance(x.begin(), it) << std::endl;
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
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

    std::ofstream ofs("dump.txt");
    std::copy(contents.begin(), contents.end(), std::ostream_iterator<std::string>(ofs, "\n"));

    const auto records = makeRecords(contents);
  }
}