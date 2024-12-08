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
    TreeNode* parent;
    TreeNode* link;
    TreeNode* child;

    TreeNode(TreeNode* pwd, const string& name)
        : name(name), parent(pwd), permission(6),
        cdate(curr_time()), mdate(curr_time()),
        link(nullptr), child(nullptr) {}

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

int main()
{
    cout << "Hellow world" << endl;
    return 0;
}