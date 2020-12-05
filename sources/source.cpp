// Copyright 2020 Your Name <your_email>

#include <header.hpp>
#include <fstream>

std::string getName(const json &j) { return j.get<std::string>(); }
void PrintName(std::ostream &out);
void PrintGroup(std::ostream &out);
void PrintAvg(std::ostream &out);
void PrintDebt(std::ostream &out);
Student::Student(const json &j) {
  name = getName(j);
  group = getGroup(j);
  avg = getAvg(j);
  debt = getDebt(j);
}

std::vector<Student> LoadFromFile(const std::string &filepath) {
  std::ifstream file;
  file.open(filepath,std::ios::in);
  if (!file.is_open()) {
    throw std::runtime_error(filepath + " not opened");
  }
  json j;
  file >> j;

  std::vector<student> result;
  for (auto i = j.at("items").cbegin(); i !=j.at("items").cend(); ++i) {
    result.emplace_back(i.value());

  }

  return std::vector<Student>();
}

std::any getGroup(const json &j) {
  if (j.is_string())
    return j.get<std::string>();
  else
    return j.get<int>();
}

std::any getAvg(const json &j) {
  if (j.is_null()) {
    return nullptr;
  } else if (j.is_string()) {
    return j.get<std::string>();
  } else if (j.is_number_float()) {
    return j.get<float>();
  } else {
    return j.get<int>();
  }
}

std::any getDebt(const json &j) {
  if (j.is_null()) {
    return nullptr;
  } else if (j.is_string()) {
    return j.get<std::string>();
  } else {
    return j.get<std::vector<std::string>>();
  }
}

