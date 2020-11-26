#include "Record.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& x, bool verbose = false)
{
  std::stringstream ss(x);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;

  return {begin, end};
}

int main(int argc, char* argv[])
{
  std::vector<Record> records;

  const auto terrainPath = "../terrain/test.dem";
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

    const auto records = Record::MakeRecords(contents);
    for (const auto& r : records)
    {
      r.dump(std::cout);
    }
  }
}