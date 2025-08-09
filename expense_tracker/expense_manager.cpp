#include "expense_manager.h"
#include "expense.h"
#include <fmt/base.h>
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
ExpenseManager::ExpenseManager() {
  std::ifstream ifs("expenses.json");
  std::string expense_data{std::istreambuf_iterator<char>(ifs),

                           std::istreambuf_iterator<char>()};
  nlohmann::json json_data = nlohmann::json::parse(expense_data);
  for (const auto &e : json_data) {
    Expense expense{e["id"], e["date"], e["description"], e["amount"]};
    expenses_.push_back(expense);
  }
}
void ExpenseManager::AddExpense(std::string description, size_t amount) {
  std::chrono::time_point<std::chrono::system_clock> tp{
      std::chrono::system_clock::now()};
  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  char *time = std::ctime(&tt);

  Expense e{expenses_.size() + 1, std::string(time), description, amount};
}
void ExpenseManager::ListExpense() {
  fmt::println("ID\tDate\tDescription\tAmount");
  for (const auto &e : expenses_) {
    fmt::println("{}\t{}\t{}\t${}", e.id, e.date, e.description, e.amount);
  }
}
void ExpenseManager::DeleteExpense(size_t id) {}
void ExpenseManager::SummaryExpense() {}