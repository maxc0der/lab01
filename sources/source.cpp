// Copyright 2020 maxim_nasachenko <maximka5406@gmail.com>

#include <fstream>
#include <header.hpp>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

const int RowSizes[4] = {14, 10, 12, 11};

std::string GetName(const json &j) { return j.get<std::string>(); }

std::any GetGroup(const json &j) {
  if (j.is_string())
    return j.get<std::string>();
  else
    return j.get<int>();
}

std::any GetAvg(const json &j) {
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

std::any GetDebt(const json &j) {
  if (j.is_null()) {
    return nullptr;
  } else if (j.is_string()) {
    return j.get<std::string>();
  } else {
    return j.get<std::vector<std::string>>();
  }
}

std::vector<Student> LoadFromFile(const std::string &filepath) {
  std::fstream file;
  file.open(filepath, std::ios::in);
  if (!file.is_open()) {
    throw std::runtime_error(filepath + " not open");
  }

  json j;
  file >> j;
  file.close();

  std::vector<Student> result;

  if (!j.at("items").is_array()) {
    throw std::runtime_error("Items most be array type");
  }

  if (j.at("items").size() != j.at("_meta").at("count")) {
    throw std::runtime_error("meta_: count and items size mismatch");
  }

  for (std::size_t i = 0; i < j.at("items").size(); i++) {
    Student student;
    student.name = GetName(j.at("items")[i].at("name"));
    student.group = GetGroup(j.at("items")[i].at("group"));
    student.avg = GetAvg(j.at("items")[i].at("avg"));
    student.debt = GetDebt(j.at("items")[i].at("debt"));
    result.push_back(student);
  }

  return result;
}

void Print(const Student &student, std::ostream &stream) {
  stream << "|" << std::left << std::setw(RowSizes[0] + 1) << student.name;

  if (student.group.type() == typeid(int)) {
    stream << "| " << std::setw(RowSizes[1]) << std::left
           << std::any_cast<int>(student.group);
  } else {
    stream << "| " << std::setw(RowSizes[1]) << std::left
           << std::any_cast<std::string>(student.group);
  }

  if (student.avg.type() == typeid(float)) {
    stream << "| " << std::setw(RowSizes[2]) << std::left
           << std::any_cast<float>(student.avg);
  } else if (student.avg.type() == typeid(int)) {
    stream << "| " << std::setw(RowSizes[2]) << std::left
           << std::any_cast<int>(student.avg);
  } else {
    stream << "| " << std::setw(RowSizes[2]) << std::left
           << std::any_cast<std::string>(student.avg);
  }

  if (student.debt.type() == typeid(std::nullptr_t)) {
    stream << "| " << std::setw(RowSizes[3]) << std::left << "none"
           << std::right << "|";
  } else if (student.debt.type() == typeid(std::string)) {
    stream << "| " << std::setw(RowSizes[3]) << std::left
           << std::any_cast<std::string>(student.debt) << std::right << "|";
  } else {
    stream
        << "| " << std::setw(RowSizes[3]) << std::left
        << (std::to_string(
                std::any_cast<std::vector<std::string>>(student.debt).size()) +
            " items")
        << "|";
  }
}

void Print(const std::vector<Student> &students, std::ostream &stream) {
  std::string TableLine;

  for (int j = 0; j < 4; ++j) {
    TableLine += "|-";
    for (int i = 0; i < RowSizes[j]; ++i) {
      TableLine += "-";
    }
  }
  TableLine += "|";

  stream << "| " << std::left << std::setw(RowSizes[0]) << "name";
  stream << "| " << std::left << std::setw(RowSizes[1]) << "group";
  stream << "| " << std::left << std::setw(RowSizes[2]) << "avg";
  stream << "| " << std::left << std::setw(RowSizes[3]) << "debt";
  stream << std::right << "|";
  stream << std::endl << TableLine << std::endl;

  for (auto &student : students) {
    Print(student, stream);
    stream << std::endl << TableLine << std::endl;
  }
}

