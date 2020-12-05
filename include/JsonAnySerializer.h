//
// Created by ubuntu on 05.12.2020.
//

#ifndef PARSER_JSONANYSERIALIZER_H
#define PARSER_JSONANYSERIALIZER_H

#include <any>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace nlohmann {
template <>
struct adl_serializer<std::any> {
  static void to_json(json &j, const std::any &a_any) {
    if (a_any.type() == typeid(std::string)) {
      j = std::any_cast<std::string>(a_any);
    } else if (a_any.type() == typeid(int)) {
      j = std::any_cast<int>(a_any);
    } else if (a_any.type() == typeid(float)) {
      j = std::any_cast<float>(a_any);
    } else if (a_any.type() == typeid(std::vector<std::string>)) {
      j = std::any_cast < std::vector < std::string>>(a_any);
    } else {
      j = nullptr;
    }
  }

  static void from_json(const json &j, std::any &a_any) {
    if (j.is_string()) {
      a_any = j.get<std::string>();
    } else if (j.is_number_integer()) {
      a_any = j.get<int>();
    } else if (j.is_number_float()) {
      a_any = j.get<float>();
    } else if (j.is_null()) {
      a_any = nullptr;
    } else if (j.is_array()) {
      std::vector<std::string> v;
      for (json::const_iterator i = j.cbegin(); i != j.cend(); i++) {
        v.push_back(i.value());
      }
      a_any = v;
    }
  }
};
}  // namespace nlohmann