// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <any>
#include <JsonAnySerializer.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <vector>

using nlohmann::json;

struct Student {
  explicit Student(const json &j);
  std::string name;
  std::any group;
  std::any avg;
  std::any debt;
};

std::vector<Student> LoadFromFile(const std::string &filepath);

void PrintTable(const std::vector<Student> &students, std::ostream &out);

std::string getName(const json &j);
std::any getGroup(const json &j);
std::any getAvg(const json &j);
std::any getDebt(const json &j);

#endif // INCLUDE_HEADER_HPP_
