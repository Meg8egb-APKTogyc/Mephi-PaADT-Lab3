#pragma once
#include "Deque.hpp"
#include "Person.hpp"
#include <functional>
#include <iostream>


// Простые тестовые функции
static int inc1(int x) { return x + 1; }
static int inc2(int x) { return x + 2; }
static int inc3(int x) { return x + 3; }
static int double_val(int x) { return x * 2; }
static int square(int x) { return x * x; }

// Тип для указателя на функцию
using IntFunctionPtr = int(*)(int);


class ManualDequeTester {
public:
    void runInteractiveTest() {
        while(true) {
            printMainMenu();
            int typeChoice;
            std::cin >> typeChoice;
            
            if(typeChoice == 7) break;
            
            switch(typeChoice) {
                case 1: testWithType<int>(); break;
                case 2: testWithType<double>(); break;
                case 3: testWithType<std::string>(); break;
                case 4: testWithType<IntFunctionPtr>(); break;
                case 5: testWithType<Student>(); break;
                case 6: testWithType<Teacher>(); break;
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
                  << "4. Function\n"
                  << "5. Student\n"
                  << "6. Teacher\n"
                  << "7. Exit\n"
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

template <>
inline Student ManualDequeTester::getSampleValue<Student>(size_t index) {
    static PersonID ids[] = {
        PersonID("1000", "200000"),
        PersonID("2000", "300000"),
        PersonID("3000", "400000"),
        PersonID("4000", "500000"),
        PersonID("5000", "600000")
    };
    static Student students[] = {
        Student(ids[0], "Ivan", "Ivanovich", "Ivanov", time(nullptr), "S001"),
        Student(ids[1], "Petr", "Petrovich", "Petrov", time(nullptr), "S002"),
        Student(ids[2], "Anna", "Andreevna", "Andreeva", time(nullptr), "S003"),
        Student(ids[3], "Maria", "Sergeevna", "Sidorova", time(nullptr), "S004"),
        Student(ids[4], "Alex", "Alexeevich", "Alexeev", time(nullptr), "S005")
    };
    return students[index % 5];
}

template <>
inline Teacher ManualDequeTester::getSampleValue<Teacher>(size_t index) {
    static PersonID ids[] = {
        PersonID("T100", "100000"),
        PersonID("T200", "200000"),
        PersonID("T300", "300000"),
        PersonID("T400", "400000"),
        PersonID("T500", "500000")
    };
    static Teacher teachers[] = {
        Teacher(ids[0], "Sergey", "Vasilievich", "Smirnov", time(nullptr), "Mathematics"),
        Teacher(ids[1], "Olga", "Pavlovna", "Ivanova", time(nullptr), "Physics"),
        Teacher(ids[2], "Dmitry", "Nikolaevich", "Petrov", time(nullptr), "Computer Science"),
        Teacher(ids[3], "Elena", "Vladimirovna", "Sidorova", time(nullptr), "Chemistry"),
        Teacher(ids[4], "Andrey", "Igorevich", "Kuznetsov", time(nullptr), "Biology")
    };
    return teachers[index % 5];
}


template <>
inline std::function<Student(Student)> ManualDequeTester::getMapFunction<Student>() {
    return [](Student s) -> Student {
        // Пример преобразования: добавляем "_mapped" к studentId
        return Student(s.GetID(), 
                     s.GetFirstName(), 
                     s.GetMiddleName(), 
                     s.GetLastName(), 
                     s.GetBirthDate(), 
                     s.GetStudentId() + "_mapped");
    };
}


template <>
inline std::function<bool(Student)> ManualDequeTester::getWhereFunction<Student>() {
    return [](Student s) -> bool {
        // Фильтруем студентов, у которых длина имени > 4
        return s.GetFirstName().length() > 4;
    };
}

template <>
inline std::function<Teacher(Teacher)> ManualDequeTester::getMapFunction<Teacher>() {
    return [](Teacher t) -> Teacher {
        // Пример преобразования: добавляем "_mapped" к department
        return Teacher(t.GetID(), 
                      t.GetFirstName(), 
                      t.GetMiddleName(), 
                      t.GetLastName(), 
                      t.GetBirthDate(), 
                      t.GetDepartment() + "_mapped");
    };
}

template <>
inline std::function<bool(Teacher)> ManualDequeTester::getWhereFunction<Teacher>() {
    return [](Teacher t) -> bool {
        // Фильтруем преподавателей, у которых длина фамилии > 6
        return t.GetLastName().length() > 6;
    };
}


template <>
inline IntFunctionPtr ManualDequeTester::getSampleValue<IntFunctionPtr>(size_t index) {
    static IntFunctionPtr functions[] = {inc1, inc2, inc3, double_val, square};
    return functions[index % 5];
}

using IntFunctionPtr = int(*)(int);

template <>
inline std::function<IntFunctionPtr(IntFunctionPtr)> ManualDequeTester::getMapFunction<IntFunctionPtr>() {
    return [](IntFunctionPtr func) -> IntFunctionPtr {
        return func;
    };

}

template <>
inline std::function<bool(IntFunctionPtr)> ManualDequeTester::getWhereFunction<IntFunctionPtr>() {
    return [](IntFunctionPtr func) { 
        // Фильтруем функции, которые при применении к 0 дают результат > 2
        return func(0) > 2; 
    };
}

// Специализация для вывода функции
template <>
inline void ManualDequeTester::printDeque(const Deque<IntFunctionPtr>* deque, std::ostream& os) const {
    if (!deque) {
        os << "Deque is null\n";
        return;
    }

    Deque<IntFunctionPtr>* temp = deque->clone();
    
    os << "Deque (size: " << deque->Size() << ", type: " 
       << (dynamic_cast<const SequenceDeque<IntFunctionPtr>*>(deque) ? "SequenceDeque" : "SegmentedDeque")
       << "): [";
    
    bool first = true;
    while (!temp->isEmpty()) {
        if (!first) os << ", ";
        IntFunctionPtr func = temp->Front();
        os << "func(0)=" << func(0); // Демонстрационный вывод
        temp->PopFront();
        first = false;
    }
    
    os << "]\n";
    delete temp;
}