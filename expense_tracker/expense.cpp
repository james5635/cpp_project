#include "expense.h"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
Expense::Expense(size_t id, std::string date, std::string description,
                 size_t amount) {
  this->id = id;
  this->date = date;
  this->description = description;
  this->amount = amount;
}