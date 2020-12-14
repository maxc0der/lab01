// Copyright 2020 maxim_nasachenko <maximka5406@gmail.com>

#include <gtest/gtest.h>

#include <fstream>
#include <header.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

TEST(ErrorTests, UnableToOpenJSONFile) {
  std::vector<Student> students;
  EXPECT_THROW(students = LoadFromFile("path/to/incorrect/file.json"),
               std::runtime_error);
}

TEST(ErrorTests, MismatchingArraySizeWith_meta) {
  std::fstream file{"mismatching_array_size.json", std::ios::out};
  if (!file.is_open()) {
    FAIL() << "Unable to open \"mismatching_array_size.json\"" << std::endl;
  }
  std::string jsonWithIncorrectMeta = R"(
{
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 1
  }
}
)";
  file << jsonWithIncorrectMeta;
  file.close();

  EXPECT_THROW(auto students = LoadFromFile("mismatching_array_size.json"),
               std::runtime_error);
}

TEST(ErrorTests, ItemsIsNotArray) {
  std::fstream file{"items_is_not_array.json", std::ios::out};
  if (!file.is_open()) {
    FAIL() << "Unable to open \"items_is_not_array.json\"" << std::endl;
  }

  std::string itemsIsNotArray = R"({
  "items":
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
  "_meta": {
    "count": 1
  }
}
)";
  file << itemsIsNotArray;
  file.close();
  EXPECT_THROW(auto students = LoadFromFile("items_is_not_array.json"),
               std::runtime_error);
}

TEST(WorkTesting, TestTable) {
  std::fstream file{"test.json", std::ios::out};
  if (!file.is_open()) {
    FAIL() << "Unable to open \"test.json\"" << std::endl;
  }
  std::string jsonString = R"(
{
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
}
)";
  file << jsonString;
  file.close();
  std::string table =
      R"(| name          | group     | avg         | debt       |
|---------------|-----------|-------------|------------|
|Ivanov Petr    | 1         | 4.25        | none       |
|---------------|-----------|-------------|------------|
|Sidorov Ivan   | 31        | 4           | C++        |
|---------------|-----------|-------------|------------|
|Pertov Nikita  | IU8-31    | 3.33        | 3 items    |
|---------------|-----------|-------------|------------|
)";
  std::stringstream PrintTableStream;
  auto students = LoadFromFile("test.json");
  Print(students, PrintTableStream);
  EXPECT_EQ(PrintTableStream.str(), table);
}
