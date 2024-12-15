#pragma once
#include <iostream>
#include <string>
#include "list.h"

using namespace std;

class HashTable {
private:
    static const int SIZE = 10;
    list table[SIZE]; // Array of custom lists for handling collisions

    int hashFunction(int key) const {
        return key % SIZE; // Simple modulo-based hash function
    }

public:
    void insert(int key, const string& value) {
        int index = hashFunction(key);
        Node* existing = table[index].find(to_string(key));
        if (existing != nullptr) {
            existing->data = to_string(key) + ":" + value; // Update if key exists
        } else {
            table[index].push_back(to_string(key) + ":" + value);
        }
    }

    string find(int key) const {
        int index = hashFunction(key);
        Node* temp = table[index].getHead();
        while (temp != nullptr) {
            size_t pos = temp->data.find(':');
            if (pos != string::npos) {
                int storedKey = stoi(temp->data.substr(0, pos));
                if (storedKey == key) {
                    return temp->data.substr(pos + 1); 
                }
            }
            temp = temp->next;
        }
        return "---"; // Default value if not found
    }
};