#include "FileSystem.h"

Node::Node(const string& name, bool isDir, Node* parent, Node* leftmostChild, Node* rightSibling) {
	name_ = name;
	isDir_ = isDir;
	parent_ = parent;
	leftmostChild_ = leftmostChild;
	rightSibling_ = rightSibling;

}

Node::~Node() {
	// empty for now
	// later add code to delete the node and all its children

}

Node* Node::leftSibling() const {
	Node* temp = parent_->leftmostChild_;
	while (temp != nullptr) {
		if (temp->rightSibling_ == this) {
			return temp;
		}
		temp = temp->rightSibling_;
	}
	return nullptr;
}

FileSystem::FileSystem() {
	root_ = new Node("", true);
	curr_ = root_;

}

// Tests
FileSystem::FileSystem(const string& testinput) {

	curr_ = root_ = new Node("", true);

	if (testinput == "1") {
		Node* e   = new Node("e"      , true , root_);
		Node* d   = new Node("d.txt"  , false, root_, nullptr, e);
		Node* c   = new Node("c.txt"  , false, root_, nullptr, d);
		Node* b   = new Node("b"      , true , root_, nullptr, c);
		Node* a   = new Node("a.txt"  , false, root_, nullptr, b);
		Node* ee  = new Node("ee.txt" , false, e);
		Node* bb2 = new Node("bb2"    , true , b);
		Node* bb1 = new Node("bb1"    , true , b, nullptr, bb2);
		Node* bbb = new Node("bbb.txt", false, bb1);
		root_->leftmostChild_ = a;
		b->leftmostChild_ = bb1;
		bb1->leftmostChild_ = bbb;
		e->leftmostChild_ = ee;
	}
	else if (testinput == "2") {
		Node* h = new Node("h"    , true , root_);
		Node* g = new Node("g.txt", false, root_, nullptr, h);
		Node* f = new Node("f"    , true , root_, nullptr, g);
		Node* e = new Node("e.txt", false, root_, nullptr, f);
		Node* d = new Node("d"    , true , root_, nullptr, e);
		Node* c = new Node("c.txt", false, root_, nullptr, d);
		Node* b = new Node("b"    , true , root_, nullptr, c);
		Node* a = new Node("a.txt", false, root_, nullptr, b);
		root_->leftmostChild_ = a;
	}
	else if (testinput == "3") {
		Node* c0 = new Node("c0", true, root_);
		Node* b0 = new Node("b0", true, root_, nullptr, c0);
		Node* a0 = new Node("a0", true, root_, nullptr, b0);
		Node* c1 = new Node("c1", true, a0);
		Node* b1 = new Node("b1", true, a0, nullptr, c1);
		Node* a1 = new Node("a1", true, a0, nullptr, b1);
		Node* c2 = new Node("c2", true, b0);
		Node* b2 = new Node("b2", true, b0, nullptr, c2);
		Node* a2 = new Node("a2", true, b0, nullptr, b2);
		Node* c3 = new Node("c3", true, c0);
		Node* b3 = new Node("b3", true, c0, nullptr, c3);
		Node* a3 = new Node("a3", true, c0, nullptr, b3);
		Node* c4 = new Node("c4", true, a1);
		Node* b4 = new Node("b4", true, a1, nullptr, c4);
		Node* a4 = new Node("a4", true, a1, nullptr, b4);
		root_->leftmostChild_ = a0;
		a0->leftmostChild_ = a1;
		b0->leftmostChild_ = a2;
		c0->leftmostChild_ = a3;
		a1->leftmostChild_ = a4;
	}
}

FileSystem::~FileSystem() {
	// release all memory used by the file system
	delete root_;
	delete curr_;
}

string FileSystem::cd(const string& path) {
	// Case 1: path is .. - go up one level
	if (path == "..") {
		if (curr->parent_ == nullptr) {
			return "invalid path";
		} else {
			curr_ = curr_->parent_;
			return "";
		}
	}

	// Case 2: path is / - go to root
	if(path == "/") {
		curr_ = root_;
		return "";
	}

	// Case 3: path is a child of the current directory
	Node* temp = curr_.leftmostChild_;

	while (temp != nullptr){
		if (temp == path) {
			curr_ = temp;
			temp = nullptr;
			return "";
		}
		temp = temp->rightSibling_;
	}

	return "invalid path";
}

string FileSystem::ls() const {

	string res;

	Node* tmp = curr_->leftmostChild_;
	while(tmp != nullptr) {
		res += tmp->name_;
		if (tmp->isDir_) res += "/\n";
		else res += "\n";
		tmp = tmp->rightSibling_;
	}
	if (res != "") res.pop_back(); // remove extra \n

	return res;
}

string FileSystem::pwd() const {
	// print absolute path of the current directory
	// Keep going up the tree until the root is reached

	Node* temp = curr_;
	string res = "";

	while (temp != root_) {
		res = "/" + temp->name_ + res;
		temp = temp->parent_;
	}

	return "/" + res;
}

string FileSystem::tree() const {

	return ""; // dummy
}

string FileSystem::touch(const string& name) {

	return ""; // dummy
}

string FileSystem::mkdir(const string& name) {

	return ""; // dummy
}

string FileSystem::rm(const string& name) {

	return ""; // dummy
}

string FileSystem::rmdir(const string& name) {

	return ""; // dummy
}

string FileSystem::mv(const string& src, const string& dest) {

	return ""; // dummy
}
