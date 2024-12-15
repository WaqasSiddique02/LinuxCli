#include <iostream>
#include <string>
using namespace std;

struct StackNode {
    string command;
    StackNode* next;
    
    StackNode(const string& cmd) : command(cmd), next(nullptr) {}
};

class CommandStack {
private:
    StackNode* top;

public:
    CommandStack() : top(nullptr) {}

    void push(const string& command) {
        StackNode* newNode = new StackNode(command);
        newNode->next = top;
        top = newNode; 
    }

    string pop() {
        if (!top) {
            cout << "No command in history!" << endl;
            return "";
        }
        StackNode* temp = top;
        string command = top->command;
        top = top->next;
        delete temp;
        return command;
    }

    string peek() const {
        if (!top) {
            return "";
        }
        return top->command;
    }

    bool is_empty() const {
        return top == nullptr;
    }

    ~CommandStack() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
};
