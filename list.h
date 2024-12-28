#pragma once
#include <iostream>
using namespace std;

class Node
{
public:
    string data; // The data stored in the node
    Node *prev;  // Pointer to the previous node
    Node *next;  // Pointer to the next node

    Node(const string &value){
        data=value;
        prev=nullptr;
        next=nullptr;
    }
};

class list{
private:
    Node *head; // Pointer to the first node
    Node *tail; // Pointer to the last node

public:
 list(){
    head=nullptr;
    tail=nullptr;
 }

    void push_front(const string &value)
    { // Push a value to the front
        Node *newNode = new Node(value);
        if (head == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void push_back(const string &value)
    { // Push a value to the back
        Node *newNode = new Node(value);
        if (tail == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void pop_front()
    { // Pop a value from the front
        if (head == nullptr)
        {
            return;
        }
        Node *temp = head;
        head = head->next;
        if (head != nullptr)
            head->prev = nullptr;
        delete temp;
        if (head == nullptr)
        {
            tail = nullptr;
        }
    }

    // Pop a value from the back
    void pop_back()
    {
        if (tail == nullptr)
        {
            return;
        }
        Node *temp = tail;
        tail = tail->prev;
        if (tail != nullptr)
            tail->next = nullptr;
        delete temp;
        if (tail == nullptr)
        {
            head = nullptr;
        } 
    }

    // Insert a value at a specific position
    void insert(Node *pos, const string &value)
    {
        if (pos == nullptr)
            return;
        Node *newNode = new Node(value);
        newNode->next = pos;
        newNode->prev = pos->prev;
        if (pos->prev != nullptr)
        {
            pos->prev->next = newNode;
        }
        else
        {
            head = newNode;
        }
        pos->prev = newNode;
    }

    // Erase a node at a specific position
    void erase(Node *pos)
    {
        if (pos == nullptr)
            return;
        if (pos->prev != nullptr)
        {
            pos->prev->next = pos->next;
        }
        else
        {
            head = pos->next;
        }
        if (pos->next != nullptr)
        {
            pos->next->prev = pos->prev;
        }
        else
        {
            tail = pos->prev;
        }
        delete pos;
    }

    // Find a node by its value
    Node *find(const string &value)
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            if (temp->data == value)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // Check if the list is empty
    bool empty() const
    {
        return head == nullptr;
    }

    // Get the size of the list (number of elements)
    size_t size() const
    {
        size_t count = 0;
        Node *temp = head;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Display the list from front to back
    void displayForward() const
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    // Display the list from back to front
    void displayBackward() const
    {
        Node *temp = tail;
        while (temp != nullptr)
        {
            cout << temp->data << " ";
            temp = temp->prev;
        }
        cout << endl;
    }

     string back() const {
        if (tail != nullptr) {
            return tail->data;
        }
        return "";
    }

    void remove_last() {
        if (tail == nullptr) {  // List is empty
            std::cout << "List is already empty!" << std::endl;
            return;
        }

        Node* temp = tail;      // Node to be deleted
        if (tail->prev) {       // If there is more than one node
            tail = tail->prev;  // Move tail to the previous node
            tail->next = nullptr;
        } else {                // If there is only one node
            head = tail = nullptr;
        }
        delete temp;  // Free the memory
    }

    void clear() {
        Node *temp = head;
        while (temp != nullptr) {
            Node *nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = tail = nullptr;  // Reset head and tail to nullptr
    }

    // Get the head of the list
    Node *getHead() const
    {
        return head;
    }

     string front() const {
        if (head != nullptr) {
            return head->data;  // Access the 'data' of the first Node
        }
        throw std::out_of_range("List is empty");
    }

    // Get the tail of the list
    Node *getTail() const
    {
        return tail;
    }

    Node* begin() {
        return head;
    }

    Node* end() {
        return nullptr;
    }

    class const_iterator {
    public:
        const_iterator(Node* node) : node_(node) {}

        const string& operator*() const {
            return node_->data; // Dereference to get the node's data
        }

        const_iterator& operator++() {
            node_ = node_->next; // Move to the next node
            return *this;
        }

        bool operator!=(const const_iterator& other) const {
            return node_ != other.node_; // Compare iterators
        }

    private:
        Node* node_;
    };

    // Return a const_iterator pointing to the head
    const_iterator begin() const {
        return const_iterator(head);
    }

    // Return a const_iterator pointing to the end (nullptr)
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    ~list(){
        Node *temp = head;
        while (temp != nullptr)
        {
            Node *nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }
};

// class CommandHistory {
// private:
//     list history;       // Doubly linked list for storing commands
//     Node* current;      // Pointer to the current command in history

// public:
//     CommandHistory() : current(nullptr) {}

//     // Add a new command to history
//     void add_command(const string& command) {
//         if (current && current->next) {
//             // Clear future history if a new command is added after undo
//             while (current->next) {
//                 history.remove_last();
//             }
//         }
//         history.push_back(command);
//         current = history.getTail();  // Move current to the latest command
//     }

//     // Undo the last command
//     void undo() {
//         if (current == nullptr || current->prev == nullptr) {
//             cout << "Nothing to undo!" << endl;
//             return;
//         }
//         cout << "Undoing: " << current->data << endl;
//         current = current->prev;
//     }

//     // Redo the next command
//     void redo() {
//         if (current == nullptr || current->next == nullptr) {
//             cout << "Nothing to redo!" << endl;
//             return;
//         }
//         current = current->next;
//         cout << "Redoing: " << current->data << endl;
//     }

//     void show_history() const {
//         Node* temp = history.getHead();
//         cout << "Command History:" << endl;
//         while (temp != nullptr) {
//             cout << temp->data;
//             if (temp == current) {
//                 cout << " <- current";
//             }
//             cout << endl;
//             temp = temp->next;
//         }
//     }
// };