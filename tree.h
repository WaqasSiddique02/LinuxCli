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

class TreeNode {
public:
    string name;
    list contents;
    char type;
    string cdate;
    string mdate;
    int permission;
    TreeNode* parent;
    TreeNode* link;
    TreeNode* child;

    TreeNode(TreeNode* pwd, const string& name) {
        this->name = name;
        parent = pwd;
        permission = 6;
        cdate = curr_time();
        mdate = curr_time();
        link = nullptr;
        child = nullptr;
    }

    string get_permission() const {
        static HashTable permissions = []() {
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
TreeNode* create(TreeNode* root, TreeNode* pwd, string path, char type);
void linux_tree(TreeNode* root);
void print_tree(TreeNode* pwd, string prev);
void print_ls(TreeNode* pwd);
void print_stat(TreeNode* root, TreeNode* pwd, string path);
string pwd_str(TreeNode* root, TreeNode* pwd);
TreeNode* find_node(TreeNode* root, TreeNode* pwd, string path);
TreeNode* find_on_pwd(TreeNode* pwd, string name);
list split(string str, char delim);
string join(const list& str, char delim);
string* split_name(string str);
TreeNode* cd(TreeNode* root, TreeNode* pwd, string path);
void remove(TreeNode* root, TreeNode* pwd, string path);
void dupl(TreeNode* root, TreeNode* pwd, string src, string dst, int keep);
void edit(TreeNode* root, TreeNode* pwd, string path);
void cat(TreeNode* root, TreeNode* pwd, string path);
void chmod(TreeNode* root, TreeNode* pwd, string path, string new_modes);
void clear_screen();


// Function Definitions
string curr_time() {
    time_t t = time(nullptr);
    struct tm tm;

    if (localtime_s(&tm, &t) != 0) {
        throw std::runtime_error("Failed to get local time");
    }

    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void linux_tree(TreeNode* root) {
    // Create some directories and files for demonstration purposes
    TreeNode* home = create(root, root, "home", 'd');
    TreeNode* user = create(root, home, "user", 'd');
    TreeNode* docs = create(root, user, "documents", 'd');
    create(root, docs, "file1.txt", '-');
    create(root, docs, "file2.txt", '-');
    TreeNode* pics = create(root, user, "pictures", 'd');
    create(root, pics, "image1.jpg", '-');
    create(root, pics, "image2.png", '-');
}

void print_tree(TreeNode* pwd, string prev) {
    if (pwd == nullptr) {
        return;
    } else if (pwd->name.empty()) {
        print_tree(pwd->child, prev);
        return;
    }
    string name = prev + "/" + pwd->name;
    cout << name << endl;
    print_tree(pwd->child, name);
    print_tree(pwd->link, prev);
}

void print_ls(TreeNode* pwd) {
    if (pwd == nullptr) {
        return;
    } else if (pwd->name.empty()) {
        print_ls(pwd->child);
        return;
    }
    cout << pwd->name << "\t" << pwd->type << pwd->get_permission() << "\t" << pwd->mdate << endl;
    print_ls(pwd->link);
}

void print_stat(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* temp = find_node(root, pwd, path);
    if (temp != nullptr) {
        cout << "File: " << temp->name << endl;
        cout << "Type: " << temp->type << endl;
        cout << "Permission: " << temp->get_permission() << endl;
        cout << "Created: " << temp->cdate << endl;
        cout << "Modified: " << temp->mdate << endl;
    } else {
        cout << "stat: cannot stat '" << path << "': No such file or directory" << endl;
    }
}

string pwd_str(TreeNode* pwd) {
    string path = pwd->name;
    while (pwd->parent != nullptr) {
        pwd = pwd->parent;
        path = pwd->name + "/" + path;
    }
    if (path.empty()) {
        return "/";
    }
    return path;
}

TreeNode* find_node(TreeNode* root, TreeNode* pwd, string path) {
    list paths = split(path, '/');
    string last = paths.back();
    paths.pop_back();
    string prev = join(paths, '/');
    TreeNode* temp = cd(root, pwd, prev);
    return find_on_pwd(temp, last);
}

TreeNode* find_on_pwd(TreeNode* pwd, string name) {
    if (pwd == nullptr) {
        return nullptr;
    }
    if (pwd->name == name) {
        return pwd;
    }
    return find_on_pwd(pwd->link, name);
}

list split(string str, char delim) {
    list res;
    string temp;
    for (char ch : str) {
        if (ch == delim) {
            if (!temp.empty()) {
                res.push_back(temp);
                temp.clear();
            }
        } else {
            temp.push_back(ch);
        }
    }
    if (!temp.empty()) {
        res.push_back(temp);
    }
    return res;
}

string join(const list& str, char delim) {
    string res;
    Node* temp = str.getHead();  // Start from the head of the list
    while (temp != nullptr) {
        if (!res.empty()) {
            res.push_back(delim);  // Add delimiter if not the first element
        }
        res += temp->data;  // Add the data of the current node
        temp = temp->next;   // Move to the next node
    }
    return res;
}

string* split_name(string str) {
    size_t pos = str.find_last_of('/');
    if (pos == string::npos) {
        return new string[2]{ "", str };
    }
    return new string[2]{ str.substr(0, pos), str.substr(pos + 1) };
}

TreeNode* cd(TreeNode* root, TreeNode* pwd, string path) {
    if (path.empty()) {
        return pwd;  // If path is empty, return the current directory
    }

    // If path starts with '/', reset to the root
    if (path[0] == '/') {
        pwd = root;
        path = path.substr(1);  // Remove the leading '/'
    }

    // Split the path into directories
    list paths = split(path, '/');
    Node* temp = paths.getHead();  // Get the first directory component

    while (temp != nullptr) {
        string dir = temp->data;  // Get the directory name from the list

        if (dir == ".") {
            // If the directory is ".", stay in the current directory
            temp = temp->next;
            continue;
        }

        if (dir == "..") {
            // If the directory is "..", go up to the parent
            if (pwd->parent != nullptr) {
                pwd = pwd->parent;
            }
            temp = temp->next;
            continue;
        }

        // Search for the child directory
        pwd = find_on_pwd(pwd->child, dir);
        if (pwd == nullptr) {
            // If directory is not found, print error and return nullptr
            cout << "cd: " << path << ": No such file or directory" << endl;
            return nullptr;
        }

        temp = temp->next;  // Move to the next directory in the list
    }

    return pwd;  // Return the final directory
}

TreeNode* create(TreeNode* root, TreeNode* pwd, string path, char type) {
    string* paths = split_name(path);
    TreeNode* dir = cd(root, pwd, paths[0]);
    if (dir == nullptr) {
        return nullptr;
    }
    TreeNode* newNode = new TreeNode(dir, paths[1]);
    newNode->type = type;
    newNode->link = dir->child;
    dir->child = newNode;
    if (type == 'd') {
        cout << "mkdir: created directory '" << path << "'" << endl;
    } else {
        cout << "touch: created file '" << path << "'" << endl;
    }
    return newNode;
}

void remove(TreeNode* root, TreeNode* pwd, string path) {
    string* paths = split_name(path);
    TreeNode* dir = cd(root, pwd, paths[0]);
    if (dir == nullptr) {
        return;
    }
    TreeNode* prev = nullptr;
    TreeNode* temp = dir->child;
    while (temp != nullptr) {
        if (temp->name == paths[1]) {
            if (prev == nullptr) {
                dir->child = temp->link;
            } else {
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

void dupl(TreeNode* root, TreeNode* pwd, string src, string dst, int keep) {
    TreeNode* srcNode = find_node(root, pwd, src);
    TreeNode* dstNode = find_node(root, pwd, dst);
    
    if (srcNode != nullptr && dstNode != nullptr) {
        if (keep) {
            create(root, pwd, dst, srcNode->type);
        }

        if (srcNode->type == 'd') {
            dupl(root, srcNode, src, dst + "/" + srcNode->name, keep);
        }
    } else {
        cout << "Error: Invalid paths" << endl;
    }
}

void chmod(TreeNode* root, TreeNode* pwd, string path, string new_modes) {
    TreeNode* temp = find_node(root, pwd, path);
    if (temp != nullptr) {
        temp->permission = stoi(new_modes);
    } else {
        cout << "chmod: cannot access '" << path << "': No such file or directory" << endl;
    }
}

void clear_screen() {
    system("CLS");
}
