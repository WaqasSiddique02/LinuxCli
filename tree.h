#pragma once
#include <iostream>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include "list.h"
#include "hashTable.h"
using namespace std;

string curr_time();

class TreeNode
{
public:
    string name;
    list contents;
    char type;
    string cdate;
    string mdate;
    int permission;
    TreeNode *parent;
    TreeNode *link;
    TreeNode *child;

    TreeNode(TreeNode *pwd, const string &name)
    {
        this->name = name;
        parent = pwd;
        permission = 6;
        cdate = curr_time();
        mdate = curr_time();
        link = nullptr;
        child = nullptr;
    }

    string get_permission() const
    {
        static HashTable permissions = []()
        {
            HashTable ht;
            ht.insert(0, "---");
            ht.insert(1, "--x");
            ht.insert(2, "-w-");
            ht.insert(3, "-wx");
            ht.insert(4, "r--");
            ht.insert(5, "r-x");
            ht.insert(6, "rw-");
            ht.insert(7, "rwx");
            return ht;
        }();

        return permissions.find(permission);
    }

    ~TreeNode() = default;
};

// Function Declarations
    string curr_time();
    TreeNode *create(TreeNode *root, TreeNode *pwd, string path, char type);
    void linux_tree(TreeNode *root);
    void print_tree(TreeNode *pwd, string prev);
    void print_ls(TreeNode *pwd);
    void print_stat(TreeNode *root, TreeNode *pwd, string path);
    string pwd_str(TreeNode *pwd);
    TreeNode *find_node(TreeNode *root, TreeNode *pwd, string path);
    TreeNode *find_on_pwd(TreeNode *pwd, string name);
    list split(string str, char delim);
    string join(const list &str, char delim);
    string *split_name(string str);
    TreeNode *cd(TreeNode *root, TreeNode *pwd, string path);
    void remove(TreeNode *root, TreeNode *pwd, string path);
    void dupl(TreeNode *root, TreeNode *pwd, string src, string dst, int keep);
    void edit(TreeNode *root, TreeNode *pwd, string path);
    void cat(TreeNode *root, TreeNode *pwd, string path);
    void chmod(TreeNode *root, TreeNode *pwd, string path, string new_modes);
    void clear_screen();

// Function Definitions
string curr_time()
{
    time_t t = time(nullptr);
    struct tm tm;

    if (localtime_s(&tm, &t) != 0)
    {
        throw std::runtime_error("Failed to get local time");
    }

    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void linux_tree(TreeNode *root)
{
    // Create some directories and files for demonstration purposes
    TreeNode *home = create(root, root, "home", 'd');
    TreeNode *user = create(root, home, "user", 'd');
    TreeNode *docs = create(root, user, "documents", 'd');
    create(root, docs, "file1.txt", '-');
    create(root, docs, "file2.txt", '-');
    TreeNode *pics = create(root, user, "pictures", 'd');
    create(root, pics, "image1.jpg", '-');
    create(root, pics, "image2.png", '-');
}

void print_tree(TreeNode *pwd, string prev)
{
    if (pwd == nullptr)
    {
        return;
    }
    else if (pwd->name.empty())
    {
        print_tree(pwd->child, prev);
        return;
    }
    string name = prev + "/" + pwd->name;
    cout << name << endl;
    print_tree(pwd->child, name);
    print_tree(pwd->link, prev);
}

void print_ls(TreeNode *pwd)
{
    if (pwd == nullptr)
    {
        return;
    }

    // Print all immediate child nodes (files or directories)
    TreeNode *current = pwd->child;
    while (current != nullptr)
    {
        // Print details of the current file/folder in the directory
        cout << current->name << "\t" << current->type << current->get_permission() << "\t" << current->mdate << endl;

        // Move to the next sibling in the directory
        current = current->link;
    }
}

void print_stat(TreeNode *root, TreeNode *pwd, string path)
{
    TreeNode *temp = find_node(root, pwd, path);
    if (temp != nullptr)
    {
        cout << "File: " << temp->name << endl;
        cout << "Type: " << temp->type << endl;
        cout << "Permission: " << temp->get_permission() << endl;
        cout << "Created: " << temp->cdate << endl;
        cout << "Modified: " << temp->mdate << endl;
    }
    else
    {
        cout << "stat: cannot stat '" << path << "': No such file or directory" << endl;
    }
}

string pwd_str(TreeNode *pwd)
{
    string path = pwd->name;
    while (pwd->parent != nullptr)
    {
        pwd = pwd->parent;
        path = pwd->name + "/" + path;
    }
    if (path.empty())
    {
        return "/";
    }
    return path;
}

TreeNode *find_node(TreeNode *root, TreeNode *pwd, string path)
{
    TreeNode *temp = nullptr;

    // If the path is absolute, resolve from the root directory
    if (path[0] == '/')
    {
        temp = cd(root, nullptr, path); // Start from root if absolute path
    }
    else
    {
        // If the path is relative, resolve it from the current directory (pwd)
        temp = cd(root, pwd, path); // Start from current directory (pwd)
    }

    if (temp == nullptr)
    {
        return nullptr; // Return null if no matching directory or file found
    }

    // Now, find the file or directory from temp (pwd or root directory)
    string last = path.substr(path.find_last_of('/') + 1); // Extract last part (file or directory name)
    return find_on_pwd(temp, last);
}

list find_names(TreeNode *root, TreeNode *pwd, std::string name)
{
    list res; // List to store results

    if (pwd == nullptr)
    {
        return res;
    }

    if (pwd->name == name)
    {
        res.push_back(pwd_str(pwd));
    }

    // Recursively find names in child and link
    list res1 = find_names(root, pwd->child, name);
    list res2 = find_names(root, pwd->link, name);

    for (Node *temp = res1.begin(); temp != nullptr; temp = temp->next)
    {
        res.push_back(temp->data); // Add string data to res
    }
    for (Node *temp = res2.begin(); temp != nullptr; temp = temp->next)
    {
        res.push_back(temp->data); // Add string data to res
    }

    return res;
}

TreeNode *find_on_pwd(TreeNode *pwd, string name)
{
    if (pwd == nullptr)
    {
        return nullptr;
    }
    if (pwd->name == name)
    {
        return pwd;
    }
    return find_on_pwd(pwd->link, name);
}

list split(string str, char delim)
{
    list res;
    string temp;
    for (char ch : str)
    {
        if (ch == delim)
        {
            if (!temp.empty())
            {
                res.push_back(temp);
                temp.clear();
            }
        }
        else
        {
            temp.push_back(ch);
        }
    }
    if (!temp.empty())
    {
        res.push_back(temp);
    }
    return res;
}

string join(const list &str, char delim)
{
    string res;
    Node *temp = str.getHead(); // Start from the head of the list
    while (temp != nullptr)
    {
        if (!res.empty())
        {
            res.push_back(delim); // Add delimiter if not the first element
        }
        res += temp->data; // Add the data of the current node
        temp = temp->next; // Move to the next node
    }
    return res;
}

string *split_name(string str)
{
    size_t pos = str.find_last_of('/');
    if (pos == string::npos)
    {
        return new string[2]{"", str};
    }
    return new string[2]{str.substr(0, pos), str.substr(pos + 1)};
}

TreeNode *cd(TreeNode *root, TreeNode *pwd, string path)
{
    if (path.empty())
    {
        return pwd; // If path is empty, return the current directory
    }

    // If path starts with '/', reset to the root
    if (path[0] == '/')
    {
        pwd = root;
        path = path.substr(1); // Remove the leading '/'
    }

    // Split the path into directories
    list paths = split(path, '/');
    Node *temp = paths.getHead(); // Get the first directory component

    while (temp != nullptr)
    {
        string dir = temp->data; // Get the directory name from the list

        if (dir == ".")
        {
            // If the directory is ".", stay in the current directory
            temp = temp->next;
            continue;
        }

        if (dir == "..")
        {
            // If the directory is "..", go up to the parent
            if (pwd->parent != nullptr)
            {
                pwd = pwd->parent;
            }
            temp = temp->next;
            continue;
        }

        // Search for the child directory
        pwd = find_on_pwd(pwd->child, dir);
        if (pwd == nullptr)
        {
            // If directory is not found, print error and return nullptr
            cout << "cd: " << path << ": No such file or directory" << endl;
            return nullptr;
        }

        temp = temp->next; // Move to the next directory in the list
    }

    return pwd; // Return the final directory
}

TreeNode *create(TreeNode *root, TreeNode *pwd, string path, char type)
{
    string *paths = split_name(path);
    TreeNode *dir = cd(root, pwd, paths[0]);
    if (dir == nullptr)
    {
        return nullptr;
    }
    TreeNode *newNode = new TreeNode(dir, paths[1]);
    newNode->type = type;
    newNode->link = dir->child;
    dir->child = newNode;
    if (type == 'd')
    {
        cout << "mkdir: created directory '" << path << "'" << endl;
    }
    else
    {
        cout << "touch: created file '" << path << "'" << endl;
    }
    return newNode;
}

void remove(TreeNode *root, TreeNode *pwd, string path)
{
    string *paths = split_name(path);
    TreeNode *dir = cd(root, pwd, paths[0]);
    if (dir == nullptr)
    {
        return;
    }
    TreeNode *prev = nullptr;
    TreeNode *temp = dir->child;
    while (temp != nullptr)
    {
        if (temp->name == paths[1])
        {
            if (prev == nullptr)
            {
                dir->child = temp->link;
            }
            else
            {
                prev->link = temp->link;
            }
            delete temp;
            cout << "rm: removed '" << path << "'" << endl;
            return;
        }
        prev = temp;
        temp = temp->link;
    }
    cout << "rm: cannot remove '" << path << "': No such file or directory" << endl;
}

void dupl(TreeNode *root, TreeNode *pwd, string src, string dst, int keep)
{
    string *src_paths = split_name(src);
    string *dst_paths = split_name(dst);

    // Locate source and destination directories
    TreeNode *src_dir = cd(root, pwd, src_paths[0]);
    TreeNode *dst_dir = cd(root, pwd, dst_paths[0]);

    if (src_dir == nullptr || dst_dir == nullptr)
    {
        cout << "dupl: source or destination path invalid." << endl;
        return;
    }

    // Locate the source node to duplicate
    TreeNode *src_node = find_on_pwd(src_dir->child, src_paths[1]);
    if (src_node == nullptr)
    {
        cout << "dupl: source file/directory not found." << endl;
        return;
    }

    // Check if a node with the same name exists in the destination
    TreeNode *dst_node = find_on_pwd(dst_dir->child, dst_paths[1]);
    if (dst_node != nullptr && keep == 0)
    {
        // If keep is 0, overwrite the destination node
        remove(root, pwd, dst);
    }
    else if (dst_node != nullptr && keep == 1)
    {
        // If keep is 1, generate a new name
        dst_paths[1] += "_copy";
    }

    // Create a duplicate of the source node
    TreeNode *new_node = new TreeNode(dst_dir, dst_paths[1]);
    new_node->type = src_node->type;
    new_node->permission = src_node->permission;
    new_node->cdate = curr_time();
    new_node->mdate = curr_time();

    // Link the new node in the destination directory
    new_node->link = dst_dir->child;
    dst_dir->child = new_node;

    cout << "dupl: '" << src << "' duplicated to '" << dst << "'" << endl;
}

void chmod(TreeNode *root, TreeNode *pwd, string path, string new_modes)
{
    TreeNode *temp = find_node(root, pwd, path);
    if (temp != nullptr)
    {
        temp->permission = stoi(new_modes);
    }
    else
    {
        cout << "chmod: cannot access '" << path << "': No such file or directory" << endl;
    }
}

// Remove a directory if it is empty
void rmdir(TreeNode *root, TreeNode *pwd, string path)
{
    TreeNode *target = find_node(root, pwd, path);
    if (target == nullptr)
    {
        cout << "rmdir: cannot remove '" << path << "': No such file or directory" << endl;
        return;
    }
    if (target->type != 'd')
    {
        cout << "rmdir: cannot remove '" << path << "': Not a directory" << endl;
        return;
    }
    if (target->child != nullptr)
    {
        cout << "rmdir: cannot remove '" << path << "': Directory not empty" << endl;
        return;
    }
    remove(root, pwd, path);
}

// Copy a file or directory from src to dst
void cp(TreeNode *root, TreeNode *pwd, string src, string dst) {
    // Find source node
    TreeNode *srcNode = find_node(root, pwd, src);
    if (srcNode == nullptr) {
        cout << "cp: cannot stat '" << src << "': No such file or directory" << endl;
        return;
    }

    // Extract the destination directory path and filename
    string dstPath = dst;
    string dstName = srcNode->name;  // Default to source name

    // If dst contains a new filename (ends with non-slash)
    size_t lastSlash = dst.find_last_of('/');
    if (lastSlash != string::npos) {
        // If dst path ends with '/', use source filename
        if (lastSlash == dst.length() - 1) {
            dstPath = dst.substr(0, dst.length() - 1);
        } else {
            // Use the provided destination filename
            dstName = dst.substr(lastSlash + 1);
            dstPath = dst.substr(0, lastSlash);
        }
    }

    // Find destination directory
    TreeNode *dstDir = cd(root, pwd, dstPath);
    if (dstDir == nullptr) {
        cout << "cp: cannot create '" << dst << "': No such file or directory" << endl;
        return;
    }

    // Check if destination already exists
    TreeNode *existing = find_node(root, dstDir, dstName);
    if (existing != nullptr) {
        cout << "cp: '" << dst << "': File already exists" << endl;
        return;
    }

    // Create new node in destination directory
    TreeNode *newCopy = new TreeNode(dstDir, dstName);
    newCopy->type = srcNode->type;
    newCopy->link = dstDir->child;
    dstDir->child = newCopy;

    cout << "cp: copied '" << src << "' to '" << dst << "'" << endl;
}

// Move a file or directory from src to dst
void mv(TreeNode *root, TreeNode *pwd, string src, string dst) {
    // Find source node
    TreeNode *srcNode = find_node(root, pwd, src);
    if (srcNode == nullptr) {
        cout << "mv: cannot stat '" << src << "': No such file or directory" << endl;
        return;
    }

    // Extract destination directory path and filename
    string dstPath = dst;
    string dstName = srcNode->name;

    size_t lastSlash = dst.find_last_of('/');
    if (lastSlash != string::npos) {
        if (lastSlash == dst.length() - 1) {
            dstPath = dst.substr(0, dst.length() - 1);
        } else {
            dstName = dst.substr(lastSlash + 1);
            dstPath = dst.substr(0, lastSlash);
        }
    }

    // Find destination directory
    TreeNode *dstDir = cd(root, pwd, dstPath);
    if (dstDir == nullptr) {
        cout << "mv: cannot move to '" << dst << "': No such file or directory" << endl;
        return;
    }

    // Check if destination already exists
    TreeNode *existing = find_node(root, dstDir, dstName);
    if (existing != nullptr) {
        cout << "mv: '" << dst << "': File already exists" << endl;
        return;
    }

    // Get source node's parent
    TreeNode *srcParent = srcNode->parent;
    
    // Remove source node from its parent's child list
    if (srcParent->child == srcNode) {
        srcParent->child = srcNode->link;
    } else {
        TreeNode *curr = srcParent->child;
        while (curr->link != srcNode) {
            curr = curr->link;
        }
        curr->link = srcNode->link;
    }

    // Update source node's properties
    srcNode->name = dstName;
    srcNode->parent = dstDir;
    srcNode->link = dstDir->child;
    dstDir->child = srcNode;

    cout << "mv: moved '" << src << "' to '" << dst << "'" << endl;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void edit(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* fileNode = find_node(root, pwd, path);
    if (fileNode == nullptr) {
        cout << "edit: cannot edit '" << path << "': No such file or directory" << endl;
        return;
    }
    if (fileNode->type != '-') {
        cout << "edit: '" << path << "' is not a file" << endl;
        return;
    }

    while (true) {
        // Show current contents
        cout << "\nCurrent contents of '" << fileNode->name << "':" << endl;
        int lineNumber = 1;
        for (Node* current = fileNode->contents.begin(); current != nullptr; current = current->next) {
            cout << lineNumber++ << ". " << current->data << endl;
        }

        // Display edit menu
        cout << "\nEdit modes:" << endl;
        cout << "1. Append new content" << endl;
        cout << "2. Modify existing line" << endl;
        cout << "3. Delete a line" << endl;
        cout << "4. Exit editor" << endl;
        cout << "Enter choice (1-4): ";
        
        string choice;
        getline(cin, choice);
        choice = trim(choice);

        if (choice.empty()) {
            cout << "No choice entered. Please enter a number between 1-4." << endl;
            continue;
        }

        if (choice == "4") {
            cout << "Exiting editor" << endl;
            break;
        }

        if (choice == "1") {
            cout << "Enter content to append (or empty line to cancel): ";
            string content;
            getline(cin, content);
            if (!content.empty()) {
                fileNode->contents.push_back(content);
                cout << "Content appended successfully" << endl;
            }
        }
        else if (choice == "2") {
            if (fileNode->contents.empty()) {
                cout << "File is empty. Nothing to modify." << endl;
                continue;
            }
            
            cout << "Enter line number to modify (1-" << fileNode->contents.size() << "): ";
            string lineNum;
            getline(cin, lineNum);
            lineNum = trim(lineNum);
            
            try {
                int line = stoi(lineNum);
                if (line < 1 || line > static_cast<int>(fileNode->contents.size())) {
                    cout << "Invalid line number" << endl;
                    continue;
                }
                
                Node* current = fileNode->contents.begin();
                for (int i = 1; i < line && current != nullptr; i++) {
                    current = current->next;
                }
                
                if (current != nullptr) {
                    cout << "Current content: " << current->data << endl;
                    cout << "Enter new content (or empty line to cancel): ";
                    string newContent;
                    getline(cin, newContent);
                    if (!newContent.empty()) {
                        current->data = newContent;
                        cout << "Line " << line << " modified successfully" << endl;
                    }
                }
            }
            catch (const invalid_argument& e) {
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
        }
        else if (choice == "3") {
            if (fileNode->contents.empty()) {
                cout << "File is empty. Nothing to delete." << endl;
                continue;
            }
            
            cout << "Enter line number to delete (1-" << fileNode->contents.size() << "): ";
            string lineNum;
            getline(cin, lineNum);
            lineNum = trim(lineNum);
            
            try {
                int line = stoi(lineNum);
                if (line < 1 || line > static_cast<int>(fileNode->contents.size())) {
                    cout << "Invalid line number" << endl;
                    continue;
                }
                
                Node* current = fileNode->contents.begin();
                for (int i = 1; i < line && current != nullptr; i++) {
                    current = current->next;
                }
                
                if (current != nullptr) {
                    fileNode->contents.erase(current);
                    cout << "Line " << line << " deleted successfully" << endl;
                }
            }
            catch (const invalid_argument& e) {
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
        }
        else {
            cout << "Invalid choice '" << choice << "'. Please enter a number between 1-4." << endl;
            continue;
        }

        fileNode->mdate = curr_time();
    }
}

void cat(TreeNode *root, TreeNode *pwd, string path)
{
    // If the path is absolute (starts with '/'), resolve it from the root
    if (path[0] == '/')
    {
        TreeNode *fileNode = find_node(root, nullptr, path);
        if (fileNode == nullptr)
        {
            cout << "cat: cannot open '" << path << "': No such file or directory" << endl;
            return;
        }
        if (fileNode->type != '-')
        {
            cout << "cat: '" << path << "' is not a file" << endl;
            return;
        }
        fileNode->contents.displayForward();
    }
    else
    {
        // If the path is relative, resolve it from the current working directory (pwd)
        TreeNode *fileNode = find_node(root, pwd, path);
        if (fileNode == nullptr)
        {
            cout << "cat: cannot open '" << path << "': No such file or directory" << endl;
            return;
        }
        if (fileNode->type != '-')
        {
            cout << "cat: '" << path << "' is not a file" << endl;
            return;
        }
        fileNode->contents.displayForward();
    }
}

void clear_screen()
{
    system("CLS");
}