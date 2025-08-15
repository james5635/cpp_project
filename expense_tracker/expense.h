#pragma once
#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>
class Expense {
public:
  Expense() = default;
  Expense(size_t id, std::string date, std::string description, size_t amount);

  size_t id;
  std::string date;
  std::string description;
  size_t amount;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Expense, id, date, description, amount);
};