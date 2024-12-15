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
    TreeNode* root = new TreeNode(nullptr, "");
    TreeNode* pwd = root;                    
    
    print_help();
    linux_tree(root);      
    string command;
    while (true) {
        cout << ">> ";
        cin >> command;

        if (command == "help") {
            print_help();
        }
        else if (command == "ls") {
            print_ls(pwd);
        }
        else if (command == "stat") {
            string path;
            cin >> path;
            print_stat(root, pwd, path);
        }
        else if (command == "pwd") {
            cout << pwd_str(pwd) << endl;
        }
        else if (command == "cd") {
            string path;
            cin >> path;
            TreeNode* newPwd = cd(root, pwd, path);
            if (newPwd != nullptr) {
                pwd = newPwd;
            }
        }
        else if (command == "mkdir") {
            string path;
            cin >> path;
            create(root, pwd, path, 'd');
        }
        else if (command == "touch") {
            string path;
            cin >> path;
            create(root, pwd, path, '-');
        }
        else if (command == "rm") {
            string path;
            cin >> path;
            remove(root, pwd, path);
        }
        else if (command == "chmod") {
            string path, mode;
            cin >> path >> mode;
            chmod(root, pwd, path, mode);
        }
        else if (command == "clear") {
            clear_screen();
        }
        else if (command == "exit") {
            cout << "Exiting file system simulator. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Unknown command. Type 'help' for the list of commands." << endl;
        }
    }

    delete root; 
    return 0;
}
