#include <iostream>
#include "list.h"
#include "hashTable.h"
#include "tree.h"
#include <conio.h>
//#include "stack.h"

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
    cout << "\thistory   -   show command history" << endl;
    cout << "\tclear     -   clear the console screen" << endl;
    cout << "\texit      -   exit the shell" << endl;
}

int main()
{
    TreeNode *root = new TreeNode(nullptr, "");
    TreeNode *pwd = root;
    //CommandHistory shell;

    print_help();
    linux_tree(root);

    string command;
    while (true)
    {
        cout << ">> ";
        cin >> command;
        //shell.add_command(command);

        if (command == "help")
        {
            print_help();
        }
        else if (command == "ls")
        {
            print_ls(pwd); // List contents of the current directory
        }
        else if (command == "tree")
        {
            linux_tree(root); // List contents of the current directory in a tree-like format
        }
        else if (command == "pwd")
        {
            cout << pwd_str(pwd) << endl; // Print the current working directory
        }
        else if (command == "cd")
        {
            string path;
            cin >> path;
            command += " " + path;
            TreeNode *newPwd = cd(root, pwd, path); // Change directory to DIR
            if (newPwd != nullptr)
            {
                pwd = newPwd;
            }
        }
        else if (command == "find")
        {
            string name;
            cin >> name;
            TreeNode* node = find_node(root, pwd, name); // Find file or directory named N
            if (node != nullptr) {
                cout << "Found: " << node->name << endl;
            } else {
                cout << "Not found" << endl;
            }
        }
        else if (command == "stat")
        {
            string path;
            cin >> path;
            command += " " + path;
            print_stat(root, pwd, path); // Print metadata of file or directory at path P
        }
        else if (command == "mkdir")
        {
            string path;
            cin >> path;
            command += " " + path;
            create(root, pwd, path, 'd'); // Create a directory named D
        }
        else if (command == "touch")
        {
            string path;
            cin >> path;
            command += " " + path;
            create(root, pwd, path, '-'); // Create a file named F
        }
        else if (command == "rm")
        {
            string path;
            cin >> path;
            command += " " + path;
            remove(root, pwd, path); // Remove the file or directory at path P
        }
        else if (command == "rmdir")
        {
            string path;
            cin >> path;
            command += " " + path;
            remove(root, pwd, path); // Remove the directory at path P
        }
        else if (command == "cp")
        {
            string src, dst;
            cin >> src >> dst;
            dupl(root, pwd, src, dst, 0); // Copy file or directory from S to D
        }
        else if (command == "mv")
        {
            string src, dst;
            cin >> src >> dst;
            dupl(root, pwd, src, dst, 1); // Move file or directory from S to D
        }
        else if (command == "edit")
        {
            string path;
            cin >> path;
            command += " " + path;
            edit(root, pwd, path); // Edit the file at path P
        }
        else if (command == "cat")
        {
            string path;
            cin >> path;
            command += " " + path;
            cat(root, pwd, path); // Print the contents of the file at path P
        }
        else if (command == "chmod")
        {
            string path, mode;
            cin >> path >> mode;
            command += " " + path + " " + mode;
            chmod(root, pwd, path, mode); // Change permissions of the file at path P to mode M
        }
        // else if (command == "history")
        // {
        //     cout << "Command History:" << endl;
        //     stack tempStack = commandHistory; // Make a copy of the command history
        //     int index = 1;

        //     while (!tempStack.isEmpty())
        //     {
        //         cout << index++ << ": " << tempStack.getTop() << endl;
        //         tempStack.pop();
        //     }
        // }
        else if (command == "clear")
        {
            clear_screen(); // Clear the console screen
        }
        else if (command == "exit"){
            cout << "Exiting file system simulator. Goodbye!" << endl;
            break;
        }
        // else if (command == "history"){
        //     shell.show_history();
        // }

        else
        {
            cout << "Unknown command. Type 'help' for the list of commands." << endl;
        }
    }

    delete root;
    return 0;
}