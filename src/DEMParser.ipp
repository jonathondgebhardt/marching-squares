#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Record.hpp"

class DEMParser
{
public:
  static std::vector<Record> MakeRecords(const std::string& x);

private:
  static std::vector<std::string> Parse(const std::string& x);

  template <class T> static std::optional<T> StringTo(const std::string& x)
  {
    std::optional<T> resultOpt;

    try
    {
      T result;
      std::stringstream ss(x);
      ss >> result;

      resultOpt = result;
    }
    catch(const std::exception& e)
    {
      std::cerr << "Could not convert " << x << " to requested type" << std::endl;
    }

    return resultOpt;
  }
};