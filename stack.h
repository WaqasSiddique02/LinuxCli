#pragma once
#include <iostream>
#include <string>
using namespace std;

class stack {
private:
    class StackNode {
    public:
        string command;
        StackNode* next;

        // Constructor for StackNode
        StackNode(const string& cmd) {
            command = cmd;
            next = nullptr;
        }
    };

    StackNode* top;

public:
    // Constructor for stack
    stack() {
        top = nullptr;  // Stack starts as empty
    }

    // Method to push a command onto the stack
    void push(const string& command) {
        // Create a new node with the given command
        StackNode* newNode = new StackNode(command);

        // If the stack is empty, new node becomes the top
        if (top == nullptr) {
            top = newNode;
        } else {
            // Otherwise, insert the new node at the top of the stack
            newNode->next = top;
            top = newNode;
        }
    }

    // Method to pop the top command from the stack
    string pop() {
        if (top == nullptr) {
            // Stack is empty, no commands to pop
            cout << "No command in history!" << endl;
            return "";
        } else {
            // Store the command to return
            StackNode* temp = top;
            string command = top->command;

            // Move the top pointer to the next node
            top = top->next;

            // Delete the old top node
            delete temp;
            return command;
        }
    }

    // Method to peek at the top command without removing it
    string peek() const {
        if (top == nullptr) {
            // Stack is empty
            return "";
        } else {
            return top->command;
        }
    }

    // Method to check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }

    string getTop() const {
        if (top == nullptr) {
            // Stack is empty
            return "";
        } else {
            return top->command;
        }
    }

    // Destructor to clean up the stack memory
    ~stack() {
        StackNode* current = top;

        // Traverse and delete each node
        while (current != nullptr) {
            StackNode* temp = current;
            current = current->next;
            delete temp;
        }
        top = nullptr;  // Explicitly setting top to nullptr after deletion
    }
};