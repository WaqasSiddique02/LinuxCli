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

class List{
private:
    Node *head; // Pointer to the first node
    Node *tail; // Pointer to the last node

public:
 List(){
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

    // Destructor to free up memory
     List()
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            Node *nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }

    // Get the head of the list
    Node *getHead() const
    {
        return head;
    }

    // Get the tail of the list
    Node *getTail() const
    {
        return tail;
    }
};

void print_help()
{
    cout << "*** Follows the syntax of Linux shell commands ***" << endl
        << endl;
    cout << "\thelp      -   print this message" << endl;
    cout << "\tls        -   list contents of the current directory" << endl;
    cout << "\ttree      -   list contents of the current directory in a tree-like format" << endl;
    cout << "\tpwd       -   print the current working directory" << endl;
    cout << "\tcd DIR    -   change directory to DIR" << endl;
    cout << "\tfind N    -   find file or directory named N" << endl;
    cout << "\tstat P    -   print metadata of file or directory at path P" << endl;
    cout << "\tmkdir D   -   create a directory named D" << endl;
    cout << "\ttouch F   -   create a file named F" << endl;
    cout << "\trm P      -   remove the file or directory at path P" << endl;
    cout << "\trmdir P   -   remove the directory at path P" << endl;
    cout << "\tcp S D    -   copy file or directory from S to D" << endl;
    cout << "\tmv S D    -   move file or directory from S to D" << endl;
    cout << "\tedit P    -   edit the file at path P" << endl;
    cout << "\tcat P     -   print the contents of the file at path P" << endl;
    cout << "\tchmod M P -   change permissions of the file at path P to mode M" << endl;
    cout << "\tclear     -   clear the console screen" << endl;
    cout << "\texit      -   exit the shell" << endl;
}

int main()
{
    cout << "Hellow world" << endl;
    return 0;
}