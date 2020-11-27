#include <DEMParser.ipp>
#include <Record.hpp>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  const auto terrainPath = "../terrain/test.dem";

  const auto records = DEMParser::MakeRecords(terrainPath);
  for(const auto& r : records)
  {
    r.dump(std::cout, true);
  }
}