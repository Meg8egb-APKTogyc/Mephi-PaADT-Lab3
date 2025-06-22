#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>

class PersonID {
private:
    std::string series;
    std::string number;

public:
    PersonID() : series("0000"), number("000000") {}
    PersonID(const std::string& series, const std::string& number)
        : series(series), number(number) {}

    std::string GetSeries() const { return series; }
    std::string GetNumber() const { return number; }

    std::string ToString() const {
        return series + " " + number;
    }

    friend std::ostream& operator<<(std::ostream& os, const PersonID& id) {
        os << id.ToString();
        return os;
    }

    bool operator==(const PersonID& other) const {
        return series == other.series && number == other.number;
    }
};

class Person {
protected:
    PersonID id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    time_t birthDate;

public:
    Person() : id(), firstName(""), middleName(""), lastName(""), birthDate(0) {}
    Person(const PersonID& id, const std::string& firstName, 
           const std::string& middleName, const std::string& lastName, 
           time_t birthDate)
        : id(id), firstName(firstName), middleName(middleName), 
          lastName(lastName), birthDate(birthDate) {}

    const PersonID& GetID() const { return id; }
    std::string GetFirstName() const { return firstName; }
    std::string GetMiddleName() const { return middleName; }
    std::string GetLastName() const { return lastName; }
    std::string GetFullName() const { 
        return firstName + " " + middleName + " " + lastName; 
    }
    time_t GetBirthDate() const { return birthDate; }

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        std::tm tm = {};
        #ifdef _WIN32
                localtime_s(&tm, &person.birthDate);
        #else
                localtime_r(&person.birthDate, &tm);
        #endif
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
        os << person.GetFullName() << " (ID: " << person.id << ", Born: " << buffer << ")";
        return os;
    }

    bool operator==(const Person& other) const {
    return id.GetSeries() == other.id.GetSeries() &&
            id.GetNumber() == other.id.GetNumber() &&
            firstName == other.firstName &&
            lastName == other.lastName &&
            birthDate == other.birthDate;
    }
};

class Student : public Person {
private:
    std::string studentId;

public:
    Student() : Person(), studentId("") {}
    Student(const PersonID& id, const std::string& firstName, 
            const std::string& middleName, const std::string& lastName, 
            time_t birthDate, const std::string& studentId)
        : Person(id, firstName, middleName, lastName, birthDate), 
          studentId(studentId) {}

    std::string GetStudentId() const { return studentId; }

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << static_cast<const Person&>(student) 
           << ", Student ID: " << student.studentId;
        return os;
    }
    bool operator==(const Student& other) const {
        return Person::operator==(other) && 
               studentId == other.studentId;
    }
};

class Teacher : public Person {
private:
    std::string department;

public:
    Teacher() : Person(), department("") {}
    Teacher(const PersonID& id, const std::string& firstName, 
            const std::string& middleName, const std::string& lastName, 
            time_t birthDate, const std::string& department)
        : Person(id, firstName, middleName, lastName, birthDate), 
          department(department) {}

    std::string GetDepartment() const { return department; }

    friend std::ostream& operator<<(std::ostream& os, const Teacher& teacher) {
        os << static_cast<const Person&>(teacher) 
           << ", Department: " << teacher.department;
        return os;
    }

    bool operator==(const Teacher& other) const {
        return Person::operator==(other) && 
               department == other.department;
    }
};