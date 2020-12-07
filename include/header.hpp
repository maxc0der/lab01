// Copyright 2020 maxim_nasachenko <maximka5406@gmail.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <any>

#include "JsonAnySerializer.h"

using nlohmann::json;

struct Student {
  explicit Student(const json &j);

  void PrintName(std::ostream &out) const;
  void PrintGroup(std::ostream &out) const;
  void PrintAvg(std::ostream &out) const;
  void PrintDebt(std::ostream &out) const;

  int NameLen() const;
  int GroupLen() const;
  int AvgLen() const;
  int DebtLen() const;

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

#endif  // INCLUDE_HEADER_HPP_
