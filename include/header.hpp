// Copyright 2020 maxim_nasachenko <maximka5406@gmail.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <any>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using nlohmann::json;

struct Student {
  std::string name;
  std::any group;
  std::any avg;
  std::any debt;
};



void Print(const Student &student, std::ostream &os);
void Print(const std::vector<Student> &students, std::ostream &os);

std::vector<Student> LoadFromFile(const std::string &filepath);

std::string GetName(const json &j);
std::any GetGroup(const json &j);
std::any GetAvg(const json &j);
std::any GetDebt(const json &j);

#endif  // INCLUDE_HEADER_HPP_
