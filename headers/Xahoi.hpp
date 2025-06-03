#pragma once
#include <iostream>
#include "Deque.hpp"


class Ring {
private:
    int size;
    std::string color;

public:
    Ring(int s, const std::string& c) : size(s), color(c) {}

    int getSize() const { return size; }
    std::string getColor() const { return color; }

    friend std::ostream& operator<<(std::ostream& os, const Ring& ring) {
        os << "Ring(size=" << ring.size << ", color=" << ring.color << ")";
        return os;
    }

    Ring() : size(0), color("") {}
};

class HanoiTower {
private:
    std::vector<Stack<Ring>*> rods;
    int totalDisks;
    int movesCount;

    void moveDisks(int n, int fromRod, int toRod, int auxRod) {
        if (n == 0) return;
        
        moveDisks(n - 1, fromRod, auxRod, toRod);
        
        Ring disk = rods[fromRod]->Pop();
        rods[toRod]->Push(disk);
        movesCount++;
        
        printState();
        
        moveDisks(n - 1, auxRod, toRod, fromRod);
    }

public:
    HanoiTower() : movesCount(0) {
        for (int i = 0; i < 3; i++) {
            rods.push_back(new Stack<Ring>());
        }
    }
    HanoiTower(const Sequence<Ring>& disks, int startRod) : movesCount(0) {
        for (int i = 0; i < 3; i++) {
            rods.push_back(new Stack<Ring>());
        }
        
        totalDisks = disks.GetLength();
        
        for (int i = disks.GetLength() - 1; i >= 0; i--) {
            rods[startRod]->Push(disks.Get(i));
        }
    }

    ~HanoiTower() {
        for (auto rod : rods) {
            delete rod;
        }
    }

    void solve(const Sequence<Ring>& disks, int startRod, int finalRod) {
        for (auto rod : rods) {
            while (!rod->isEmpty()) {
                rod->Pop();
            }
        }
        movesCount = 0;

        for (int i = disks.GetLength() - 1; i >= 0; i--) {
            rods[startRod]->Push(disks.Get(i));
        }

        std::cout << "=== Starting Hanoi Tower Test ===" << std::endl;
        std::cout << "Initial state:" << std::endl;
        printState();

        int auxRod = (startRod + 1) % 3 == finalRod ? (startRod + 2) % 3 : (startRod + 1) % 3;
        int toRod = finalRod;
        if (startRod != finalRod)
            moveDisks(disks.GetLength(), startRod, toRod, auxRod);

        std::cout << "=== Test Completed ===" << std::endl;
        std::cout << "Total moves: " << movesCount << std::endl;
    }

    void printState() const {
        std::cout << "Moves: " << movesCount << std::endl;
        for (int i = 0; i < 3; i++) {
            std::cout << "Rod " << i << ": ";
            
            Stack<Ring> tempStack;
            Stack<Ring>* original = rods[i];
            
            Stack<Ring>* tempDeque = original->clone();
            while (!tempDeque->isEmpty()) {
                tempStack.Push(tempDeque->Pop());
            }
            delete tempDeque;
            
            while (!tempStack.isEmpty()) {
                std::cout << tempStack.Pop() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------" << std::endl;
    }
};