#include <iostream>
#include <unordered_map>
#include "Deque.hpp"
#include "Sequence/Sequence.hpp"

struct Wagon {
    std::string type;
    int number;
    
    Wagon() {}
    Wagon(const std::string& t, int n) : type(t), number(n) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Wagon& w) {
        return os << "(" << w.type << "," << w.number << ")";
    }
};


struct TypeInfo {
    int last;
    bool isSolid;

    TypeInfo() {}

    TypeInfo(int l, bool i) : last(l), isSolid(i) {}
};


void printState(const Deque<Wagon>* mainTrack, 
                const std::unordered_map<std::string, Stack<Wagon>*>& sidings,
                const std::string& action) {
    std::cout << "--- " << action << " ---" << std::endl;
    
    std::cout << "Основной путь: ";
    Deque<Wagon>* tempMain = mainTrack->clone();
    while (!tempMain->isEmpty()) {
        std::cout << tempMain->Front() << " ";
        tempMain->PopFront();
    }
    delete tempMain;
    std::cout << std::endl;

    for (const auto& pair : sidings) {
        std::cout << "Тупик " << pair.first << ": ";
        Stack<Wagon>* tempStack = pair.second->clone();
        Stack<Wagon> reversed;
        
        while (!tempStack->isEmpty()) {
            reversed.Push(tempStack->Pop());
        }
        
        while (!reversed.isEmpty()) {
            std::cout << reversed.Top() << " ";
            reversed.Pop();
        }
        delete tempStack;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Sequence<Wagon>* sortWagonsByType(const Sequence<Wagon>* input) {
    std::unordered_map<std::string, Stack<Wagon>*> sidings;
    std::unordered_map<std::string, TypeInfo> info;
    Deque<Wagon>* mainTrack = new SequenceDeque<Wagon>();

    for (int i = 0; i < input->GetLength(); ++i) {
        std::string type = input->Get(i).type;
        if (info.find(type) == info.end()) {
            info[type] = TypeInfo(i, true);
        } else {
            if (input->Get(i).type != input->Get(i - 1).type) {
                info[type].isSolid = false;
            }
            info[type].last = i;
        }

        mainTrack->PushBack(input->Get(i));
    }

    printState(mainTrack, sidings, "Начальное состояние");

    int i = 0;
    while (i < input->GetLength()) {
        Wagon wagon = mainTrack->PopFront();

        if (info[wagon.type].isSolid) {
            mainTrack->PushBack(wagon);
            printState(mainTrack, sidings, "Перемещаем в конец основного пути");
        } else if (info[wagon.type].last != i) {
            if (sidings.find(wagon.type) != sidings.end()) {
                sidings[wagon.type]->Push(wagon);
                printState(mainTrack, sidings, "Добавление в тупик");
            } else {
                sidings[wagon.type] = new Stack<Wagon>();
                sidings[wagon.type]->Push(wagon);
                printState(mainTrack, sidings, "Новый тупик для типа " + wagon.type + " и добавление в тупик");
            }
        } else {
            auto it = sidings.begin();
            if (it->first == wagon.type) {
                it = std::next(it);
                if (it == sidings.end()) {
                    sidings["alternate"] = new Stack<Wagon>();
                    it = sidings.find("alternate");
                }
            }

            sidings[it->first]->Push(wagon);
            printState(mainTrack, sidings, "Перенос из основного пути в тупик");

            while (!sidings[wagon.type]->isEmpty()) {
                sidings[it->first]->Push(sidings[wagon.type]->Pop());
                printState(mainTrack, sidings, "Перенос из тупика в тупик");
            }

            while (!sidings[it->first]->isEmpty() && sidings[it->first]->Top().type == wagon.type) {
                mainTrack->PushBack(sidings[it->first]->Pop());
                printState(mainTrack, sidings, "Перенос из тупика в основной путь");
            }

            if (sidings[it->first]->isEmpty()) {
                delete sidings[it->first];
                sidings.erase(it);
            }

            delete sidings[wagon.type];
            sidings.erase(sidings.find(wagon.type));
        }

        ++i;
    }

    Sequence<Wagon>* ret = new MutableArraySequence<Wagon>();
    while (!mainTrack->isEmpty()) {
        ret->Append(mainTrack->PopFront());
    }

    return ret;
}