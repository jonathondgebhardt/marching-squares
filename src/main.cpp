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
  void dump(bool verbose = false) const
  {
    std::cout << "numElevations: " << this->numElevations << std::endl;
    std::cout << "elevations.size(): " << this->elevations.size() << std::endl;

    if (verbose)
    {
      std::copy(this->elevations.begin(), this->elevations.end(),
                std::ostream_iterator<double>(std::cout, "\n"));
    }
  }

  std::vector<double> elevations;
  size_t numElevations;
};

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
      std::cerr << e.what() << '\n';
      numElevationsOpt = std::nullopt;
    }

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
        std::cerr << e.what() << '\n';
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

    std::ofstream ofs("dump.txt");
    std::copy(contents.begin(), contents.end(), std::ostream_iterator<std::string>(ofs, "\n"));

    const auto records = makeRecords(contents);
  }
}