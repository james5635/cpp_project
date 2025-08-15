#include "CLI/CLI.hpp"
#include "expense_manager.h"
#include <CLI/CLI.hpp>
#include <iostream>
void usage() {}
int main(int argc, char *argv[]) {
  ExpenseManager em{};
  CLI::App app{"Track and manage personal/business expenses"};

  auto add = app.add_subcommand("add", "add an expense");
  std::string description;
  size_t amount;
  add->add_option("--description", description, "description of expense")
      ->required();
  add->add_option("--amount", amount, "amount of expense")->required();
  add->callback([&]() { em.AddExpense(description, amount); });

  auto list = app.add_subcommand("list", "list all expenses");
  list->callback([&]() { em.ListExpense(); });
  short month = 0;
  auto summary = app.add_subcommand("summary", "summary expenses");
  summary->add_option("--month", month, "month for expenses");
  summary->callback([&]() {
    if (!month)
      em.SummaryExpense();
    else
      em.SummaryExpense(month);
  });
  auto delete_ = app.add_subcommand("delete", "delete an expense");
  size_t id;
  delete_->add_option("--id", id, "id for the expense to be deleted");
  delete_->callback([&]() { em.DeleteExpense(id); });
  CLI11_PARSE(app, argc, argv);
}
