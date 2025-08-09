#pragma once
#include <cstddef>
#include <string>
class Expense {
public:
  Expense() = default;
  Expense(size_t id, std::string date, std::string description, size_t amount);

  size_t id;
  std::string date;
  std::string description;
  size_t amount;
};