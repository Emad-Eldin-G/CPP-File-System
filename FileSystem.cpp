#include "FileSystem.h"
#include <cstddef>
#include <stack>

Node::Node(const string& name, bool isDir, Node* parent, Node* leftmostChild, Node* rightSibling) : name_(name), isDir_(isDir), parent_(parent), leftmostChild_(leftmostChild), rightSibling_(rightSibling) {
}

Node::~Node() {
	// Recursively delete all children
	Node* child = leftmostChild_;
	while (child != nullptr) {
		Node* next = child->rightSibling_;
		delete child;
		child = next;
	}
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

	return;
}

FileSystem::~FileSystem() {
	// release all memory used by the file system
	// delete root_ will recursively delete all children
	delete root_;
}

string FileSystem::cd(const string& path) {
	// Case 1: path is .. - go up one level
	if (path == "..") {
		if (curr_->parent_ == nullptr) return "invalid path";
		curr_ = curr_->parent_;
		return "";
	}
	// Case 2: path is / - go to root
	else if (path == "/") {
		curr_ = root_;
		return "";
	}
	// Case 3: path is a child of the current directory
	else {
		Node* temp = curr_->leftmostChild_;
		while (temp != nullptr) {
			if (temp->name_ == path) {
				curr_ = temp;
				return "";
			}
			temp = temp->rightSibling_;
		}
		return "invalid path";
	}
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
	Node* temp = curr_;
	string res = "";

	while (temp != root_) {
		res = "/" + temp->name_ + res;
		temp = temp->parent_;
	}

	// If res is empty, we're at root, return "/"
	// Otherwise, res already starts with "/", so just return it
	// This avoids the double slash in terminal view
	return res.empty() ? "/" : res;
}

string FileSystem::tree() const {
	if (!curr_) return "";

    string tree_view;
	int level = 0;
    std::stack<std::pair<Node*, int>> st;
    st.push({curr_, level});

    while (!st.empty()) {
        auto [node, depth] = st.top();
        st.pop();

        string tabs(depth, ' ');
        tree_view += tabs + node->name_ + (node->isDir_ ? "/" : "") + "\n";

        if (node != curr_ && node->rightSibling_) {
            st.push({node->rightSibling_, depth});
        }

        if (node->leftmostChild_) {
            st.push({node->leftmostChild_, depth + 1});
        }
    }

    return tree_view;
}

string FileSystem::touch(const string& name) {
	if (name.length() < 1) {
		return "";
	}

	// Check if file with this name already exists in current directory
	Node* i = curr_->leftmostChild_;
	while (i != nullptr) {
		if (i->name_ == name && !i->isDir_) {
			return name + "/" + i->parent_->name_+ " already exists";
		}
		i = i->rightSibling_;
	}

	Node* new_file = new Node(name, false, curr_);

	// If directory is empty, make it the leftmost child
	if (curr_->leftmostChild_ == nullptr) {
		curr_->leftmostChild_ = new_file;
	}
	// If new file should be first (alphabetically before leftmost child)
	else if (name < curr_->leftmostChild_->name_) {
		new_file->rightSibling_ = curr_->leftmostChild_;
		curr_->leftmostChild_ = new_file;
	}
	// Otherwise, find the correct position in the sibling level
	else {
		Node* prev = curr_->leftmostChild_;
		Node* current = prev->rightSibling_;
		
		// Find where to insert (alphabetically ordered)
		while (current != nullptr && name > current->name_) {
			prev = current;
			current = current->rightSibling_;
		}
		
		// Insert between prev and current
		new_file->rightSibling_ = current;
		prev->rightSibling_ = new_file;
	}

	return "";
}

string FileSystem::mkdir(const string& name) {
	if (name.length() < 1) {
		return "";
	}

	// Check if directory with this name already exists in current directory
	Node* i = curr_->leftmostChild_;
	while (i != nullptr) {
		if (i->name_ == name && i->isDir_) {
			return name + "/" + i->parent_->name_+ " already exists";
		}
		i = i->rightSibling_;
	}

	Node* new_dir = new Node(name, true, curr_);

	// If directory is empty, make it the leftmost child
	if (curr_->leftmostChild_ == nullptr) {
		curr_->leftmostChild_ = new_dir;
	}
	// If new directory should be first (alphabetically before leftmost child)
	else if (name < curr_->leftmostChild_->name_) {
		new_dir->rightSibling_ = curr_->leftmostChild_;
		curr_->leftmostChild_ = new_dir;
	}
	// Otherwise, find the correct position in the sibling level
	else {
		Node* prev = curr_->leftmostChild_;
		Node* current = prev->rightSibling_;
		
		// Find where to insert (alphabetically ordered)
		while (current != nullptr && name > current->name_) {
			prev = current;
			current = current->rightSibling_;
		}
		
		new_dir->rightSibling_ = current;
		prev->rightSibling_ = new_dir;
	}
	return "";
}

string FileSystem::rm(const string& name) {
	Node* i = curr_->leftmostChild_;

	if (i->name_ == name) {
		if (i->isDir_) {
			return "not a file";
		}
		curr_->leftmostChild_ = i->rightSibling_;
		delete i;
		return "";
	}
	else {
		while (i != nullptr) {
			if (i->rightSibling_ != nullptr && i->rightSibling_->name_ == name) {
				if (i->rightSibling_->isDir_) {
					return "not a file";
				}
				Node* to_del = i->rightSibling_;
				i->rightSibling_ = i->rightSibling_->rightSibling_;
				delete to_del;
				return "";
			}
			i = i->rightSibling_;
		}
		return "file not found";
	}
}

string FileSystem::rmdir(const string& name) {

	return ""; // dummy
}

string FileSystem::mv(const string& src, const string& dest) {

	return ""; // dummy
}
