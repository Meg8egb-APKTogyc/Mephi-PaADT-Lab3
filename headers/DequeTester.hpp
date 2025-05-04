#pragma once
#include "Deque.hpp"
#include <functional>
#include <iostream>

class ManualDequeTester {
public:
    void runInteractiveTest() {
        while(true) {
            printMainMenu();
            int typeChoice;
            std::cin >> typeChoice;
            
            if(typeChoice == 4) break;
            
            switch(typeChoice) {
                case 1: testWithType<int>(); break;
                case 2: testWithType<double>(); break;
                case 3: testWithType<std::string>(); break;
                default: std::cout << "Invalid choice!\n";
            }
        }
    }

private:
    template <typename T>
    void printDeque(const Deque<T>* deque, std::ostream& os = std::cout) const {
        if (!deque) {
            os << "Deque is null\n";
            return;
        }

        Deque<T>* temp = deque->clone();
        
        os << "Deque (size: " << deque->Size() << ", type: " 
        << (dynamic_cast<const SequenceDeque<T>*>(deque) ? "SequenceDeque" : "SegmentedDeque")
        << "): [";
        
        bool first = true;
        while (!temp->isEmpty()) {
            if (!first) os << ", ";
            os << temp->Front();
            temp->PopFront();
            first = false;
        }
        
        os << "]\n";
        delete temp;
    }
    template <typename T>
    void testWithType() {
        std::cout << "\n=== Testing Deque ===\n";
        printImplementationMenu();
        
        int implChoice;
        std::cin >> implChoice;
        
        Deque<T>* deque = createDeque<T>(implChoice);
        if(deque) {
            testImplementation(deque);
            delete deque;
        } else {
            std::cout << "Invalid implementation choice!\n";
        }
    }

    template <typename T>
    void testImplementation(Deque<T>* deque) {
        std::cout << "\n=== Testing Deque Operations ===\n";
        
        // Базовые операции
        printTestResult(deque->isEmpty(), "Initial deque is empty");
        printTestResult(deque->Size() == 0, "Initial size is 0");

        // Тестирование Push/Pop
        T sample1 = getSampleValue<T>(0);
        deque->PushBack(sample1);
        printTestResult(!deque->isEmpty(), "After PushBack, deque is not empty");
        printTestResult(deque->Back() == sample1, "Back() after PushBack");

        T sample2 = getSampleValue<T>(1);
        deque->PushFront(sample2);
        printTestResult(deque->Front() == sample2, "Front() after PushFront");

        // Тестирование Pop
        T frontItem = deque->PopFront();
        printTestResult(frontItem == sample2, "PopFront returns correct value");
        printTestResult(deque->Size() == 1, "Size after PopFront");

        T backItem = deque->PopBack();
        printTestResult(backItem == sample1, "PopBack returns correct value");
        printTestResult(deque->isEmpty(), "Deque is empty after pops");

        // Тестирование сложных операций
        testComplexOperations(deque);
    }

    template <typename T>
    void testComplexOperations(Deque<T>* deque) {
        // Заполняем дек тестовыми данными
        for(int i = 0; i < 5; i++) {
            deque->PushBack(getSampleValue<T>(i));
        }
        printDeque(deque);

        // Тестирование where
        Deque<T>* filtered = deque->where(getWhereFunction<T>());
        printTestResult(filtered->Size() <= deque->Size(), 
                       "Where result size: " + std::to_string(filtered->Size()));
        printDeque(filtered);
        delete filtered;

        // Тестирование map
        Deque<T>* mapped = deque->map(getMapFunction<T>());
        printTestResult(mapped->Size() == deque->Size(), 
                       "Map result size: " + std::to_string(mapped->Size()));
        printDeque(mapped);
        delete mapped;

        // Тестирование concat
        Deque<T>* other = deque->clone();
        Deque<T>* concatenated = deque->concat(other);
        printTestResult(concatenated->Size() == deque->Size() * 2, 
                       "Concat result size: " + std::to_string(concatenated->Size()));
        printDeque(concatenated);
        delete other;
        delete concatenated;

        // Тестирование subSequence
        Deque<T>* subSeq = deque->subSequence(1, 3);
        printTestResult(subSeq->Size() == 2, 
                       "SubSequence size: " + std::to_string(subSeq->Size()));
        printDeque(subSeq);
        delete subSeq;
    }

    template <typename T>
    T getSampleValue(size_t index) {
        static_assert(sizeof(T) == 0, "Not implemented for this type");
        return T();
    }

    template <typename T>
    std::function<T(T)> getMapFunction() {
        static_assert(sizeof(T) == 0, "Not implemented for this type");
        return [](T x) { return x; };
    }

    template <typename T>
    std::function<bool(T)> getWhereFunction() {
        static_assert(sizeof(T) == 0, "Not implemented for this type");
        return [](T x) { return true; };
    }

    void printMainMenu() {
        std::cout << "\n=== Main Menu ===\n"
                  << "Choose data type:\n"
                  << "1. int\n"
                  << "2. double\n"
                  << "3. std::string\n"
                  << "4. Exit\n"
                  << "Your choice: ";
    }

    void printImplementationMenu() {
        std::cout << "Choose implementation:\n"
                  << "1. SequenceDeque\n"
                  << "2. SegmentedDeque\n"
                  << "Your choice: ";
    }

    template <typename T>
    Deque<T>* createDeque(int choice) {
        switch(choice) {
            case 1: return new SequenceDeque<T>();
            case 2: return new SegmentedDeque<T>();
            default: return nullptr;
        }
    }

    void printTestResult(bool success, const std::string& message) {
        std::cout << (success ? "[PASS] " : "[FAIL] ") << message << "\n";
    }
};

template <>
inline int ManualDequeTester::getSampleValue<int>(size_t index) {
    static int samples[] = {10, 20, 30, 40, 50};
    return samples[index % 5];
}

template <>
inline double ManualDequeTester::getSampleValue<double>(size_t index) {
    static double samples[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    return samples[index % 5];
}

template <>
inline std::string ManualDequeTester::getSampleValue<std::string>(size_t index) {
    static std::string samples[] = {"apple", "banana", "cherry", "date", "fig"};
    return samples[index % 5];
}

template <>
inline std::function<int(int)> ManualDequeTester::getMapFunction<int>() {
    return [](int x) { return x * 2; };
}

template <>
inline std::function<double(double)> ManualDequeTester::getMapFunction<double>() {
    return [](double x) { return x + 1.0; };
}

template <>
inline std::function<std::string(std::string)> ManualDequeTester::getMapFunction<std::string>() {
    return [](std::string s) { return s + "!"; };
}

template <>
inline std::function<bool(int)> ManualDequeTester::getWhereFunction<int>() {
    return [](int x) { return x > 20; };
}

template <>
inline std::function<bool(double)> ManualDequeTester::getWhereFunction<double>() {
    return [](double x) { return x < 3.0; };
}

template <>
inline std::function<bool(std::string)> ManualDequeTester::getWhereFunction<std::string>() {
    return [](std::string s) { return s.length() > 5; };
}