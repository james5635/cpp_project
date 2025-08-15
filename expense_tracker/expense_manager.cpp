#include "expense_manager.h"
#include "expense.h"
#include <algorithm>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <nlohmann/json.hpp>
#include <sstream>
ExpenseManager::ExpenseManager() {

  std::ifstream ifs("expenses.json");
  if (!ifs)
    return;
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

  std::stringstream ss;
  ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %X");

  Expense e{expenses_.size() + 1, ss.str(), description, amount};
  expenses_.push_back(e);
  Save();
}
void ExpenseManager::ListExpense() {
  // fmt::println("ID\tDate\tDescription\tAmount");
  std::cout << std::left << std::setw(3) << "ID" << std::setw(20) << "Date"
            << std::setw(25) << "Description" << std::setw(6) << "Amount"
            << '\n';
  for (const auto &e : expenses_) {
    // fmt::println("{}\t{}\t{}\t${}", e.id, e.date, e.description, e.amount);
    std::cout << std::setw(3) << e.id << std::setw(20) << e.date
              << std::setw(25) << e.description << '$' << std::setw(6)
              << e.amount << '\n';
  }
}
void ExpenseManager::DeleteExpense(size_t id) {
  auto it = std::find_if(expenses_.begin(), expenses_.end(),
                         [&id](const Expense &e) { return e.id == id; });
  if (it != expenses_.end()) {
    expenses_.erase(it);
    Save();
  }
}

void ExpenseManager::SummaryExpense() {
  size_t sum{0};
  for (auto &e : expenses_)
    sum += e.amount;
  std::cout << "Total expenses: " << '$' << sum << '\n';
}

void ExpenseManager::SummaryExpense(short month) {
  size_t sum{0};
  for (auto &e : expenses_) {
    if (e.date.substr(5, 2).find(std::to_string(month)) != std::string::npos)
      sum += e.amount;
  }
  std::cout << "Total expenses: " << '$' << sum << '\n';
}
void ExpenseManager::Save() {
  nlohmann::json j = expenses_;
  std::ofstream ofs("expenses.json");
  ofs << j.dump();
}