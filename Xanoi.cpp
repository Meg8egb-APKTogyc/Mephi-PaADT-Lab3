#include "headers/Xahoi.hpp"
#include <iostream>
#include <vector>
#include <sstream>


MutableArraySequence<Ring> createDisksFromInput(const std::string& input) {
  MutableArraySequence<Ring> disks;
  std::istringstream iss(input);
  int size;
  std::string color;
  
  while (iss >> size >> color) {
    disks.Append(Ring(size, color));
  }
  
  return disks;
}


int main() {
  HanoiTower tower;

  std::cout << "\nEnter custom test (format: size1 color1 size2 color2 ...), start from least: ";
  std::string input;
  std::getline(std::cin, input);
  MutableArraySequence<Ring> customDisks = createDisksFromInput(input);
  
  int startRod;
  std::cout << "Enter starting rod (1-3): ";
  std::cin >> startRod;

  int finalRod;
  std::cout << "Enter final rod (1-3): ";
  std::cin >> finalRod;
  
  tower.solve(customDisks, startRod - 1, finalRod - 1);


  return 0;
}