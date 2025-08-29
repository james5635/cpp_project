#include <iostream>
#include <random>
void displayWelcome() {
  std::cout << "Welcome to the Number Guessing Game!\n";
  std::cout << "I'm thinking of a number between 1 and 100\n";
  std::cout << "\n";
  std::cout << "Please select the difficulty level:\n";
  std::cout << "1. Easy (10 chances)\n";
  std::cout << "2. Medium (5 chances)\n";
  std::cout << "3. Hard (3 chances)\n";
}
int main() {
  displayWelcome();
  int chance, choice;
  std::cout << "Enter your choice: ";
  std::cin >> choice;
  switch (choice) {
  case 1: {
    chance = 10;
    break;
  }
  case 2: {
    chance = 5;
    break;
  }
  case 3: {
    chance = 3;
    break;
  }
  default: {
    std::cerr << "Error: invalid choice";
    exit(1);
  }
  }
  std::cout << "Let's start the game!\n";
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100);
  int num = dist(mt);
  // std::cout << num;
  for (int i = 0; i < chance; i++) {
    std::cout << "Enter your guess: ";
    int guess;
    std::cin >> guess;
    if (num == guess) {
      std::cout << "Congratulations! You guessed the correct number in "
                << i + 1 << " attempts.\n";
      return 0;
    } else if (num > guess) {
      std::cout << "Incorrect! The number is greater than " << guess << ".\n";
    } else {
      std::cout << "Incorrect! The number is less than " << guess << ".\n";
    }
  }
  std::cout << "you lose the game! (answer: " << num << ")\n";
  return 0;
}