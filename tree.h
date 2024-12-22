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
string pwd_str(TreeNode* pwd);
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

list find_names(TreeNode* root, TreeNode* pwd, std::string name)
{
    list res;  // List to store results

    if (pwd == nullptr) {
        return res;
    }

    if (pwd->name == name) {
        res.push_back(pwd_str(pwd));
    }

    // Recursively find names in child and link
    list res1 = find_names(root, pwd->child, name);
    list res2 = find_names(root, pwd->link, name);

    for (Node* temp = res1.begin(); temp != nullptr; temp = temp->next) {
        res.push_back(temp->data);  // Add string data to res
    }
    for (Node* temp = res2.begin(); temp != nullptr; temp = temp->next) {
        res.push_back(temp->data);  // Add string data to res
    }

    return res;
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

void dupl(TreeNode* root, TreeNode* pwd, string src, string dst, int keep)
{
    string* src_paths = split_name(src);
    string* dst_paths = split_name(dst);
    TreeNode* src_dir = cd(root, pwd, src_paths[0]);
    TreeNode* dst_dir = cd(root, pwd, dst_paths[0]);
    if (src_dir == nullptr || dst_dir == nullptr)
    {
        return;
    }
    TreeNode* src_node = find_on_pwd(src_dir->child, src_paths[1]);
    if (src_node == nullptr)
    {
        cout << "cp: " << src << ": No such file or directory" << endl;
        return;
    }
    if (src_node->type == 'd')
    {
        cout << "cp: omitting directory '" << src << "'" << endl;
        return;
    }
    TreeNode* dst_node = find_on_pwd(dst_dir->child, dst_paths[1]);
    if (dst_node != nullptr)
    {
        cout << "cp: cannot overwrite non-directory '" << dst << "' with directory '" << src << "'" << endl;
        return;
    }
    TreeNode* newNode = new TreeNode(dst_dir, dst_paths[1]);
    newNode->type = src_node->type;
    newNode->contents = src_node->contents;
    newNode->permission = src_node->permission;
    newNode->cdate = src_node->cdate;
    newNode->mdate = src_node->mdate;
    newNode->link = dst_dir->child;
    dst_dir->child = newNode;
    if (keep == 0)
    {
        remove(root, pwd, src);
    }
    cout << "cp: copied '" << src << "' to '" << dst << "'" << endl;
}

void chmod(TreeNode* root, TreeNode* pwd, string path, string new_modes) {
    TreeNode* temp = find_node(root, pwd, path);
    if (temp != nullptr) {
        temp->permission = stoi(new_modes);
    } else {
        cout << "chmod: cannot access '" << path << "': No such file or directory" << endl;
    }
}

// Remove a directory if it is empty
void rmdir(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* target = find_node(root, pwd, path);
    if (target == nullptr) {
        cout << "rmdir: cannot remove '" << path << "': No such file or directory" << endl;
        return;
    }
    if (target->type != 'd') {
        cout << "rmdir: cannot remove '" << path << "': Not a directory" << endl;
        return;
    }
    if (target->child != nullptr) {
        cout << "rmdir: cannot remove '" << path << "': Directory not empty" << endl;
        return;
    }
    remove(root, pwd, path);
}

// Copy a file or directory from src to dst
void cp(TreeNode* root, TreeNode* pwd, string src, string dst) {
    TreeNode* srcNode = find_node(root, pwd, src);
    if (srcNode == nullptr) {
        cout << "cp: cannot stat '" << src << "': No such file or directory" << endl;
        return;
    }
    TreeNode* dstDir = cd(root, pwd, dst);
    if (dstDir == nullptr) {
        cout << "cp: cannot create '" << dst << "': No such file or directory" << endl;
        return;
    }
    TreeNode* newCopy = new TreeNode(dstDir, srcNode->name);
    newCopy->type = srcNode->type;
    newCopy->link = dstDir->child;
    dstDir->child = newCopy;
    cout << "cp: copied '" << src << "' to '" << dst << "'" << endl;
}

// Move a file or directory from src to dst
void mv(TreeNode* root, TreeNode* pwd, string src, string dst) {
    cp(root, pwd, src, dst);  // First copy
    remove(root, pwd, src);  // Then delete original
    cout << "mv: moved '" << src << "' to '" << dst << "'" << endl;
}

// Edit the contents of a file
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
    cout << "Enter content to append to '" << fileNode->name << "': ";
    string content;
    getline(cin, content);

    fileNode->contents.push_back(content);

    fileNode->mdate = curr_time();
    cout << "edit: appended content to file '" << path << "'" << endl;
}

void cat(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* fileNode = find_node(root, pwd, path);
    if (fileNode == nullptr) {
        cout << "cat: cannot open '" << path << "': No such file or directory" << endl;
        return;
    }
    if (fileNode->type != '-') {
        cout << "cat: '" << path << "' is not a file" << endl;
        return;
    }

    // Use the custom StringList to iterate and print each string
    fileNode->contents.displayForward();
}

void clear_screen() {
    system("CLS");
}