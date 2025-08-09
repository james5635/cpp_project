#include "expense.h"
#include <vector>

class ExpenseManager {

public:
  ExpenseManager();
  void AddExpense(std::string description, size_t amount);
  void ListExpense();
  void SummaryExpense();
  void DeleteExpense(size_t id);
private:
  std::vector<Expense> expenses_;
};