#pragma once
#include <iostream>
#include "list.h"
#include "hashTable.h"
#include "tree.h"
using namespace std;

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


int main() {
    // Create root node for the file system
    TreeNode* root = new TreeNode(nullptr, "");
    root->type = 'd';
    TreeNode* pwd = root;

    // Initialize file system with some default structure
    linux_tree(root);

    cout << endl;
    print_help();

    string cmd;
    cout << endl << pwd_str(root, pwd) << ">> ";
    
    while (getline(cin >> ws, cmd)) {
        list args = split(cmd, ' ');
        TreeNode* temp_pwd = nullptr;

        if (cmd == "help") {
            print_help();
        }
        else if (args.front() == "ls") {
            args.pop_front();
            if (args.empty()) {
                print_ls(pwd->child);
            }
            else {
    for (const string& arg : args) {
        cout << arg << ":" << endl;
        temp_pwd = cd(root, pwd, arg);  // Assuming this changes pwd to the correct directory
        
        if (temp_pwd != nullptr) {
            // Call the print_ls function to print the children (which is a list of Node*)
            print_ls(temp_pwd->child);  
        }
    }
}
        }
        else if (args.front() == "tree") {
            args.pop_front();
            if (args.empty()) {
                print_tree(pwd->child, "");
            }
            else {
                for (const string& arg : args) {
                    cout << arg << ":\n.\n";
                    temp_pwd = cd(root, pwd, arg);
                    if (temp_pwd != nullptr) {
                        print_tree(temp_pwd->child, "");
                    }
                }
            }
        }
        else if (cmd == "pwd") {
            cout << pwd_str(root, pwd) << endl;
        }
        else if (args.front() == "cd") {
            args.pop_front();
            if (args.empty()) {
                pwd = root;
            }
            else {
                temp_pwd = cd(root, pwd, args.front());
                if (temp_pwd != nullptr) {
                    pwd = temp_pwd;
                }
            }
        }
        else if (args.front() == "find") {
            args.pop_front();
            if (args.empty()) {
                cout << "find: missing operand" << endl;
            }
            else {
                for (const string& arg : args) {
                    list res;
                    if (arg[0] == '/') {
                        res = find_names(root, root, arg);
                    }
                    else {
                        res = find_names(pwd, pwd, arg);
                    }
                    if (res.empty()) {
                        cout << "find: '" << arg << "': no such file or directory" << endl;
                    }
                    else {
    for (Node* it = res.begin(); it != nullptr; it = it->next) {  // Iterating through Node* in custom list
        cout << it->data << endl;  // Accessing the data (string) of each Node and printing it
    }
}
                }
            }
        }
        else if (args.front() == "stat") {
            args.pop_front();
            if (args.empty()) {
                cout << "stat: missing operand" << endl;
            }
            else {
                for (const string& arg : args) {
                    print_stat(root, pwd, arg);
                }
            }
        }
        else if (args.front() == "mkdir") {
            args.pop_front();
            if (args.empty()) {
                cout << "mkdir: missing operand" << endl;
            }
            else {
                for (const string& arg : args) {
                    create(root, pwd, arg, 'd');
                }
            }
        }
        else if (args.front() == "touch") {
            args.pop_front();
            if (args.empty()) {
                cout << "touch: missing operand" << endl;
            }
            else {
                for (const string& arg : args) {
                    create(root, pwd, arg, '-');
                }
            }
        }
        else if (args.front() == "rm" || args.front() == "rmdir") {
            string command = args.front();
            args.pop_front();
            if (args.empty()) {
                cout << command << ": missing operand" << endl;
            }
            else {
                for (const string& arg : args) {
                    remove(root, pwd, arg);
                }
            }
        }
        else if (args.front() == "cp" || args.front() == "mv") {
            string command = args.front();
            args.pop_front();
            if (args.size() != 2) {
                cout << command << ": missing operand" << endl;
            }
            else {
                int keep = (command == "cp") ? 1 : 0;
                string src = args.front();
                args.pop_front();
                string dst = args.front();
                args.pop_front();
                dupl(root, pwd, src, dst, keep);
            }
        }
        else if (args.front() == "edit") {
            args.pop_front();
            if (args.empty()) {
                cout << "edit: missing operand" << endl;
            }
            else {
                edit(root, pwd, args.front());
            }
        }
        else if (args.front() == "cat") {
            args.pop_front();
            if (args.empty()) {
                cout << "cat: missing operand" << endl;
            }
            else {
                cat(root, pwd, args.front());
            }
        }
        else if (args.front() == "chmod") {
            args.pop_front();
            if (args.size() < 2) {
                cout << "chmod: missing operand" << endl;
            }
            else {
                string perm = args.front();
                args.pop_front();
                chmod(root, pwd, args.front(), perm);
            }
        }
        else if (cmd == "clear") {
            clear_screen();
        }
        else if (cmd == "exit") {
            break;
        }
        else {
            cout << "Unknown command" << endl;
        }

        cout << endl << pwd_str(root, pwd) << ">> ";
    }

    // Clean up memory
    delete root;

    cout << endl;
    return 0;
}