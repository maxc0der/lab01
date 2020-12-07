// Copyright 2020 maxim_nasachenko <maximka5406@gmail.com>
#include <fstream>
#include <header.hpp>
#include <sstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

std::string getName(const json &j) { return j.get<std::string>(); }

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

Student::Student(const json &j) {
  name = getName(j.at("name"));
  group = getGroup(j.at("group"));
  avg = getAvg(j.at("avg"));
  debt = getDebt(j.at("debt"));
}

void Student::PrintName(std::ostream &out) const { out << name; }

void Student::PrintDebt(std::ostream &out) const {
  if (debt.type() == typeid(std::string)) {
    out << std::any_cast<std::string>(debt);
  } else if (debt.type() == typeid(std::vector<std::string>)) {
    out << std::any_cast<std::vector<std::string>>(debt).size() << " items";
  } else if (debt.type() == typeid(std::nullptr_t)) {
    out << "none";
  } else {
    out << "err";
  }
}

void Student::PrintAvg(std::ostream &out) const {
  if (avg.type() == typeid(int)) {
    out << std::any_cast<int>(avg);
  } else if (avg.type() == typeid(float)) {
    out.precision(3);
    out << std::any_cast<float>(avg);
    out.precision(0);
  } else if (avg.type() == typeid(std::string)) {
    out << std::any_cast<std::string>(avg);
  } else {
    out << "err";
  }
}

void Student::PrintGroup(std::ostream &out) const {
  if (group.type() == typeid(int)) {
    out << std::any_cast<int>(group);
  } else if (group.type() == typeid(std::string)) {
    out << std::any_cast<std::string>(group);
  } else {
    out << "err";
  }
}

std::vector<Student> LoadFromFile(const std::string &filepath) {
  std::ifstream file;
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

  for (auto i = j.at("items").cbegin(); i != j.at("items").cend(); ++i) {
    result.emplace_back(i.value());
  }

  return result;
}

int Student::NameLen() const { return name.length(); }

int Student::GroupLen() const {
  if (group.type() == typeid(std::string)) {
    return std::any_cast<std::string>(group).length();
  } else if (group.type() == typeid(int)) {
    return std::to_string(std::any_cast<int>(group)).length();
  } else {
    return 0;
  }
}

int Student::AvgLen() const {
  if (avg.type() == typeid(int)) {
    return std::to_string(std::any_cast<int>(avg)).length();
  } else if (avg.type() == typeid(float)) {
    std::stringstream ss;
    ss.precision(3);
    ss << std::any_cast<float>(avg);
    return ss.str().length();
  } else if (avg.type() == typeid(std::string)) {
    return std::any_cast<std::string>(avg).length();
  } else {
    return 0;
  }
}

int Student::DebtLen() const {
  if (debt.type() == typeid(std::string)) {
    return std::any_cast<std::string>(debt).length();
  } else if (debt.type() == typeid(std::vector<std::string>)) {
    return std::to_string(std::any_cast<std::vector<std::string>>(debt).size())
               .length() +
           6;
  } else if (debt.type() == typeid(std::nullptr_t)) {
    return 4;
  } else {
    return 0;
  }
}

void PrintTable(const std::vector<Student> &students, std::ostream &out) {
  int nameColWidth = 6, groupColWidth = 5, avgColWidth = 7, debtColWidth = 6;

  for (auto const &student : students) {
    nameColWidth = std::max(student.NameLen(), nameColWidth + 2);
    groupColWidth = std::max(student.GroupLen(), groupColWidth + 2);
    avgColWidth = std::max(student.AvgLen(), avgColWidth + 2);
    debtColWidth = std::max(student.DebtLen(), debtColWidth + 2);
  }

  std::vector<int> colWidths{nameColWidth, groupColWidth, avgColWidth,
                             debtColWidth};

  std::string separator = "|";
  for (auto &colWidth : colWidths) {
    for (int i = 0; i < colWidth; i++) {
      separator += "-";
    }
    separator += "|";
  }

  out << "| name";
  for (int i = 0; i < nameColWidth - 5; i++) out << " ";
  out << "| group";
  for (int i = 0; i < groupColWidth - 6; i++) out << " ";
  out << "| avg";
  for (int i = 0; i < avgColWidth - 4; i++) out << " ";
  out << "| debt";
  for (int i = 0; i < debtColWidth - 5; i++) out << " ";
  out << "|";
  out << std::endl << separator << std::endl;

  for (auto const &s : students) {
    out << "|";
    s.PrintName(out);
    for (int i = 0; i < nameColWidth - s.NameLen() - 1; i++) out << " ";
    out << " |";

    out << " ";
    s.PrintGroup(out);
    for (int i = 0; i < groupColWidth - s.GroupLen() - 1; i++) out << " ";
    out << "|";

    out << " ";
    s.PrintAvg(out);
    for (int i = 0; i < avgColWidth - s.AvgLen() - 1; i++) out << " ";
    out << "|";

    out << " ";
    s.PrintDebt(out);
    for (int i = 0; i < debtColWidth - s.DebtLen() - 1; i++) out << " ";
    out << "|";
    out << std::endl << separator << std::endl;
  }
}
