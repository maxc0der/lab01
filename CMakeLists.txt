cmake_minimum_required(VERSION 3.4) #минимальная поддерживаемая версия симейка

set(CMAKE_CXX_STANDARD 17)  # стандарт языка си плюс плюс
set(CMAKE_CXX_STANDARD_REQUIRED ON) # не принимать другую версию языка

option(BUILD_COVERAGE "Build coverage" OFF)

set( #кеш хантергейта
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake") # включаю симейк с хантергейтом

HunterGate( # стягивает в проект хантер, т.е. менеджер пакетов. ссылка на актив и хеш-сумма
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)


project(parser) # имя проекта
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra") #флаги компилятора
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest) #скачать библеотеку Гтест
find_package(GTest CONFIG REQUIRED)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED) #настроить все пути в рамках проекта

add_library(parser STATIC # добавить библеотеку к проекту # статическая - компилируется вместе с программой
        # статическая включается, динамическая одна, и программы берут из нее нужные функции
  # enum your files and delete this comment
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/header.hpp #статическая библеотека собирается из этих файлов
        include/JsonAnySerializer.h)
add_executable(tests #добавляем исполняемый файл к проекту - тест, собирается из этого исходника.
        # туда пишем тесты, они скомпилируются и прогонятся

  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
)


target_include_directories(parser #можем инклудить в любой файл ПРОЕКТА ПАРСЕР  любые файлы из папки инклуд
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(tests # эту же папку разрешаем инклудить из тестс
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests GTest::gtest_main parser) #указываем линкеру какие библеотеки мы собираем
target_link_libraries(parser nlohmann_json)
# и линкуем к бинарнику

enable_testing()
add_test(NAME unit_tests COMMAND tests) #добавляем тесты с именем юнит-тестс который запускается командой тестс

if(BUILD_COVERAGE)
  function(ENABLE_PROJECT_COVERAGE PROJECT)
    target_compile_options(${PROJECT} PRIVATE --coverage)
    target_link_libraries(${PROJECT} --coverage)
    add_coverage(${PROJECT})
  endfunction()

  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)

  enable_project_coverage(parser)
  enable_project_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
