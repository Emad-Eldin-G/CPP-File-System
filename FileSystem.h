#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
using std::string;

class Node {

    string name_;         // name of the file/directory
    bool isDir_;          // is this node a directory or not
    Node* parent_;        // pointer to parent
    Node* leftmostChild_; // pointer to leftmost child
    Node* rightSibling_;  // pointer to next (right side) sibling

    [[nodiscard]] Node* leftSibling() const;

public:
    // parameterised constructor
    Node(const string& name, bool isDir, Node* parent = nullptr, Node* leftmostChild = nullptr, Node* rightSibling = nullptr);

    // destructor
    ~Node();

    friend class FileSystem; // allow FileSystem to access private members
};

class FileSystem {

    Node* root_; // pointer to root directory
    Node* curr_; // pointer to current directory

public:
    // default constructor
    FileSystem();

    // parameterised constructor
    FileSystem(const string& testinput);

    // destructor
    ~FileSystem();

    // change directory
    string cd(const string& path);

    // list directory contents
    [[nodiscard]] string ls() const;

    // display subtree contents
    [[nodiscard]] string tree() const;

    // print working directory
    [[nodiscard]] string pwd() const;

    // create new file
    string touch(const string& name);

    // create new directory
    string mkdir(const string& name);

    // remove file
    string rm(const string& name);

    // remove directory
    string rmdir(const string& name);

    // move file/directory from src to dest
    string mv(const string& src, const string& dest);
};

#endif
