#include <iostream>
#include "list.h"
#include "hashTable.h"
#include "tree.h"
#include <fstream>
using namespace std;

const int MAX_SUGGESTIONS = 100;
const int MAX_COMMANDS = 18; // Number of valid commands

// Array of valid commands
const string VALID_COMMANDS[MAX_COMMANDS] = {
    "help", "ls", "tree", "pwd", "cd", "find", "stat", 
    "mkdir", "touch", "rm", "rmdir", "cp", "mv", 
    "edit", "cat", "chmod", "clear", "exit"
};

// Function to check if a command exists in an array
bool command_exists(const string suggestions[], int count, const string& command) {
    for (int i = 0; i < count; i++) {
        if (suggestions[i] == command) {
            return true;
        }
    }
    return false;
}

// Function to check if a command is valid
bool is_valid_command(const string &command) {
    
    string base_command = command.substr(0, command.find(' ')); // Extract the base command before any arguments
    
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (VALID_COMMANDS[i] == base_command) {
            return true;
        }
    }
    return false;
}

void add_to_history(const string &command) {
    // Only add valid commands to history
    if (is_valid_command(command)) {
        ofstream historyFile("history.txt", ios::app);
        if (historyFile.is_open()) {
            historyFile << command << endl;
            historyFile.close();
        }
        else {
            cerr << "Error: Unable to open history file." << endl;
        }
    }
}

int get_suggestions(const string &partialCommand, string suggestions[]) {
    // Dont suggest if the command is already valid
    if (is_valid_command(partialCommand)) {
        return 0;
    }

    int count = 0;

    // First check against valid commands
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (VALID_COMMANDS[i].find(partialCommand) == 0) { // Command starts with partial input
            if (!command_exists(suggestions, count, VALID_COMMANDS[i])) {
                suggestions[count++] = VALID_COMMANDS[i];
                if (count >= MAX_SUGGESTIONS) return count;
            }
        }
    }

    // Then check history for more complex commands with arguments
    ifstream historyFile("history.txt");
    if (historyFile.is_open()) {
        string line;
        while (getline(historyFile, line)) {
            if (line.find(partialCommand) == 0) { // Line starts with partial input
                if (!command_exists(suggestions, count, line)) {
                    suggestions[count++] = line;
                    if (count >= MAX_SUGGESTIONS) break;
                }
            }
        }
        historyFile.close();
    }

    return count;
}

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
    TreeNode *root = new TreeNode(nullptr, "");
    TreeNode *pwd = root;

    print_help();
    linux_tree(root);

    string command;
    string suggestions[MAX_SUGGESTIONS];

    while (true)
    {
        cout << ">> ";
        cin >> command;

        if (!is_valid_command(command)) {
            int suggestionCount = get_suggestions(command, suggestions);
            if (suggestionCount > 0) {
                cout << "Did you mean:" << endl;
                for (int i = 0; i < suggestionCount; i++) {
                    cout << "  " << suggestions[i] << endl;
                }
                continue;
            } else {
                cout << "Unknown command. Type 'help' for the list of commands." << endl;
                continue;
            }
        }

        // Process valid command
        add_to_history(command);

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
            print_tree(root, ""); // List contents of the current directory in a tree-like format
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
            TreeNode *node = find_node(root, pwd, name); // Find file or directory named N
            if (node != nullptr)
            {
                cout << "Found: " << node->name << endl;
            }
            else
            {
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
            if (src.empty() || dst.empty())
            {
                cout << "cp: missing file operand" << endl;
                cout << "Usage: cp source_file destination" << endl;
            }
            else
            {
                cp(root, pwd, src, dst); // Using the new cp function instead of dupl
            }
        }
        else if (command == "mv")
        {
            string src, dst;
            cin >> src >> dst;
            if (src.empty() || dst.empty())
            {
                cout << "mv: missing file operand" << endl;
                cout << "Usage: mv source_file destination" << endl;
            }
            else
            {
                mv(root, pwd, src, dst);
            }
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
        else if (command == "clear")
        {
            clear_screen(); // Clear the console screen
        }
        else if (command == "exit")
        {
            cout << "Exiting file system simulator. Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "Unknown command. Type 'help' for the list of commands." << endl;
        }
    }

    delete root;
    return 0;
}