#include "headers/Deque.hpp"
#include "headers/DequeTester.hpp"
#include "headers/Xahoi.hpp"
#include "headers/Station.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

Sequence<Wagon>* test1() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 1));
    input->Append(Wagon("A", 2));
    input->Append(Wagon("A", 3));
    return input;
}

Sequence<Wagon>* test2() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 1));
    input->Append(Wagon("A", 2));
    input->Append(Wagon("B", 3));
    input->Append(Wagon("B", 4));
    return input;
}

Sequence<Wagon>* test3() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 1));
    input->Append(Wagon("B", 2));
    input->Append(Wagon("A", 3));
    input->Append(Wagon("B", 4));
    return input;
}

Sequence<Wagon>* test4() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 3));
    input->Append(Wagon("B", 1));
    input->Append(Wagon("C", 2));
    input->Append(Wagon("A", 4));
    input->Append(Wagon("B", 5));
    input->Append(Wagon("C", 6));
    return input;
}

Sequence<Wagon>* test5() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 1));
    input->Append(Wagon("B", 2));
    input->Append(Wagon("C", 3));
    input->Append(Wagon("D", 4));
    input->Append(Wagon("A", 5));
    input->Append(Wagon("B", 6));
    input->Append(Wagon("C", 7));
    input->Append(Wagon("D", 8));
    return input;
}

Sequence<Wagon>* test6() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("A", 1));
    input->Append(Wagon("B", 2));
    input->Append(Wagon("C", 3));
    input->Append(Wagon("D", 4));
    input->Append(Wagon("E", 5));
    return input;
}


Sequence<Wagon>* test7() {
    Sequence<Wagon>* input = new MutableArraySequence<Wagon>();
    input->Append(Wagon("t2", 1));
    input->Append(Wagon("t3", 2));
    input->Append(Wagon("t3", 3));
    input->Append(Wagon("t1", 4));
    input->Append(Wagon("t2", 5));
    input->Append(Wagon("t3", 6));
    input->Append(Wagon("t2", 7));
    input->Append(Wagon("t1", 8));
    return input;
}


void runTest(Sequence<Wagon>* (*testFunc)(), const std::string& testName) {
    std::cout << "=== " << testName << " ===" << std::endl;
    Sequence<Wagon>* input = testFunc();
    std::cout << "Входные данные: ";
    for (int i = 0; i < input->GetLength(); ++i) {
        std::cout << input->Get(i) << " ";
    }
    std::cout << std::endl << std::endl;
    
    Sequence<Wagon>* result = sortWagonsByType(input);
    
    std::cout << "Результат: ";
    for (int i = 0; i < result->GetLength(); ++i) {
        std::cout << result->Get(i) << " ";
    }
    std::cout << std::endl << std::endl;
    
    delete input;
    delete result;
}


Sequence<Wagon>* inputWagonsFromUser() {
    Sequence<Wagon>* wagons = new MutableArraySequence<Wagon>();
    std::cout << "Введите последовательность вагонов в формате 'тип номер [тип номер ...]':\n";
    std::cout << "Пример: t2 1 t3 2 t3 3 t1 4 t2 5 t3 6 t2 7 t1 8\n";
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    std::istringstream iss(inputLine);
    
    std::string type;
    int number;
    
    while (iss >> type >> number) {
        wagons->Append(Wagon(type, number));
    }
    
    if (iss.fail() && !iss.eof()) {
        std::cout << "Ошибка ввода. Пожалуйста, проверьте формат данных.\n";
        iss.clear();
        iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    return wagons;
}

int main() {
    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Ввести последовательность вручную\n";
        std::cout << "2. Запустить стандартные тесты\n";
        std::cout << "3. Выход\n";
        std::cout << "Выберите вариант: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 1) {
            Sequence<Wagon>* input = inputWagonsFromUser();
            std::cout << "\nВведенная последовательность: ";
            for (int i = 0; i < input->GetLength(); ++i) {
                std::cout << input->Get(i) << " ";
            }
            std::cout << "\n\n";
            
            Sequence<Wagon>* result = sortWagonsByType(input);
            
            std::cout << "Результат сортировки: ";
            for (int i = 0; i < result->GetLength(); ++i) {
                std::cout << result->Get(i) << " ";
            }
            std::cout << "\n";
            
            delete input;
            delete result;
        }
        else if (choice == 2) {
            // Запуск стандартных тестов
            runTest(test1, "Тест 1: Один тип вагонов");
            runTest(test2, "Тест 2: Два типа, уже отсортированы");
            runTest(test3, "Тест 3: Два типа, перемешаны");
            runTest(test4, "Тест 4: Три типа с разной нумерацией");
            runTest(test5, "Тест 5: Четыре типа");
            runTest(test6, "Тест 6: Все вагоны разные");
            runTest(test7, "Тест 7: Пример из условия задачи");
        }
        else if (choice == 3) {
            break;
        }
        else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
    
    return 0;
}