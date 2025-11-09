#include "FileSystem.h"
#include <cstddef>
#include <stack>
#include <sstream>
#include <vector>
using std::vector;
using std::stringstream;

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

void FileSystem::insert_inorder(Node* new_node, Node* parent) {
	// If directory is empty, make it the leftmost child
	if (parent->leftmostChild_ == nullptr) {
		parent->leftmostChild_ = new_node;
	}
	// If new node should be first (alphabetically before leftmost child)
	else if (new_node->name_ < parent->leftmostChild_->name_) {
		new_node->rightSibling_ = parent->leftmostChild_;
		parent->leftmostChild_ = new_node;
	}
	// Otherwise, find the correct position in the sibling chain
	else {
		Node* prev = parent->leftmostChild_;
		Node* current = prev->rightSibling_;
		
		// Find where to insert (alphabetically ordered)
		while (current != nullptr && new_node->name_ > current->name_) {
			prev = current;
			current = current->rightSibling_;
		}
		
		// Insert between prev and current
		new_node->rightSibling_ = current;
		prev->rightSibling_ = new_node;
	}
}

Node* FileSystem::navigateToPath(const string& path, Node* startDir) {
	if (path.empty()) return nullptr;
	
	// Split path by '/'
	vector<string> parts;
	stringstream ss(path);
	string part;
	while (getline(ss, part, '/')) {
		if (!part.empty()) {
			parts.push_back(part);
		}
	}
	
	if (parts.empty()) {
		// Path is just "/" or empty after splitting
		return (path[0] == '/') ? root_ : startDir;
	}
	
	// Determine starting directory
	Node* current = (path[0] == '/') ? root_ : startDir;
	
	// Navigate through each part
	for (size_t i = 0; i < parts.size(); i++) {
		if (parts[i] == "..") {
			if (current->parent_ == nullptr) return nullptr;
			current = current->parent_;
		} else if (parts[i] == ".") {
			// Stay in current directory
			continue;
		} else {
			// Find child with this name
			Node* child = current->leftmostChild_;
			while (child != nullptr && child->name_ != parts[i]) {
				child = child->rightSibling_;
			}
			if (child == nullptr) return nullptr;
			// If this is the last part, return the node
			if (i == parts.size() - 1) {
				return child;
			}
			if (!child->isDir_) return nullptr; // /a/b.txt/c is invalid
			current = child;
		}
	}
	
	return current;
}

Node* FileSystem::getParentFromPath(const string& path, string& filename) {
	if (path.empty()) return nullptr;
	
	// Find last '/'
	size_t lastSlash = path.find_last_of('/');
	
	if (lastSlash == string::npos) {
		// No slash - path is just a filename in current directory
		filename = path;
		return curr_;
	}
	
	// Extract filename and parent path
	filename = path.substr(lastSlash + 1);
	string parentPath = path.substr(0, lastSlash);
	
	if (parentPath.empty()) {
		// Path was "/filename" - parent is root
		return root_;
	}
	
	// Navigate to parent directory
	Node* parent = navigateToPath(parentPath, curr_);
	if (parent == nullptr || !parent->isDir_) return nullptr;
	return parent;
}

bool FileSystem::isAncestor(Node* node1, Node* node2) {
	if (!node1->isDir_) return false; // files can’t be ancestors
	Node* current = node2;
	while (current != nullptr) {
		if (current == node1) return true;
		current = current->parent_;
	}
	return false;
}

string FileSystem::cd(const string& path) {
	Node* target = navigateToPath(path, curr_);
	if (target == nullptr || !target->isDir_) {
		return "invalid path";
	}
	curr_ = target;
	return "";
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
    std::stack<std::pair<Node*, int>> st; // dfs stack
    st.push({curr_, level});

    while (!st.empty()) {
        auto [node, depth] = st.top();
        st.pop();

        string tabs(depth, ' ');
        tree_view += tabs + node->name_ + (node->isDir_ ? "/" : "") + "\n"; // dirs already have "/", so don't add one to prevent duplicate "//"

		// the node we're DFSing from should start checking from children, NOT siblings
        if (node != curr_ && node->rightSibling_) {
            st.push({node->rightSibling_, depth});
        }

        if (node->leftmostChild_) {
            st.push({node->leftmostChild_, depth + 1});
        }
    }

	// remove the last \n
	tree_view.pop_back();
    return tree_view;
}

string FileSystem::touch(const string& name) {
	if (name.length() < 1) {
		return "";
	}

	string filename;
	Node* parentDir = getParentFromPath(name, filename);
	if (parentDir == nullptr) {
		return "invalid path";
	}

	// Check if file with this name already exists in parent directory
	Node* i = parentDir->leftmostChild_;
	while (i != nullptr) {
		if (i->name_ == filename && !i->isDir_) {
			return "file/directory already exists";
		}
		i = i->rightSibling_;
	}

	Node* new_file = new Node(filename, false, parentDir);
	insert_inorder(new_file, parentDir);
	return "";
}

string FileSystem::mkdir(const string& name) {
	if (name.length() < 1) {
		return "";
	}

	string dirname;
	Node* parentDir = getParentFromPath(name, dirname);
	if (parentDir == nullptr) {
		return "invalid path";
	}

	// Check if directory with this name already exists in parent directory
	Node* i = parentDir->leftmostChild_;
	while (i != nullptr) {
		if (i->name_ == dirname && i->isDir_) {
			return "file/directory already exists";
		}
		i = i->rightSibling_;
	}

	Node* new_dir = new Node(dirname, true, parentDir);
	insert_inorder(new_dir, parentDir);
	return "";
}

string FileSystem::rm(const string& name) {
	string filename;
	Node* parentDir = getParentFromPath(name, filename);
	if (parentDir == nullptr) {
		return "file not found";
	}

	Node* target = parentDir->leftmostChild_;
	while (target != nullptr && target->name_ != filename) {
		target = target->rightSibling_;
	}

	if (target == nullptr) {
		return "file not found";
	}
	if (target->isDir_) {
		return "not a file";
	}

	// Remove from parent's children list
	if (target == parentDir->leftmostChild_) {
		parentDir->leftmostChild_ = target->rightSibling_;
	} else {
		Node* prev = parentDir->leftmostChild_;
		while (prev->rightSibling_ != target) {
			prev = prev->rightSibling_;
		}
		prev->rightSibling_ = target->rightSibling_;
	}
	delete target;
	return "";
}

string FileSystem::rmdir(const string& name) {
	// Special case: cannot remove root
	if (name == "/" || name == "..") {
		return "cannot remove root directory";
	}

	string dirname;
	Node* parentDir = getParentFromPath(name, dirname);
	if (parentDir == nullptr) {
		return "directory not found";
	}

	Node* target = parentDir->leftmostChild_;
	while (target != nullptr && target->name_ != dirname) {
		target = target->rightSibling_;
	}

	if (target == nullptr) {
		return "directory not found";
	}
	if (!target->isDir_) {
		return "not a directory";
	}
	if (target == root_) {
		return "cannot remove root directory";
	}
	if (target->leftmostChild_ != nullptr) {
		return "directory not empty";
	}

	// Remove from parent's children list
	if (target == parentDir->leftmostChild_) {
		parentDir->leftmostChild_ = target->rightSibling_;
	} else {
		Node* prev = parentDir->leftmostChild_;
		while (prev->rightSibling_ != target) {
			prev = prev->rightSibling_;
		}
		prev->rightSibling_ = target->rightSibling_;
	}
	delete target;
	return "";
}


string FileSystem::mv(const string& src, const string& dest) {
	if (dest.length() < 1 || src.length() < 1){
		return "invalid path";
	}

	if (src == dest) {
		return "source and destination are the same";
	}

	// Find source node
	Node* temp_src;
	if (src == "..") {
		if (curr_ == root_) {
			return "source does not exist";
		}
		temp_src = curr_->parent_;
	} else {
		temp_src = navigateToPath(src, curr_);
		if (temp_src == nullptr) {
			return "source does not exist";
		}
	}

	// Cannot move root
	if (temp_src == root_) {
		return "cannot move or rename root directory";
	}

	// Check if current directory is inside source
	if (isAncestor(temp_src, curr_)) {
		return "cannot move or rename while current directory is inside source";
	}

	// Handle destination
	Node* dest_parent;
	string dest_filename;
	
	if (dest == "..") {
		if (curr_ == root_) {
			return "invalid path";
		}
		dest_parent = curr_->parent_;
		dest_filename = temp_src->name_; // Keep original name when moving to parent
		
		// Check if parent already has a file/directory with the same name
		Node* check = dest_parent->leftmostChild_;
		while (check != nullptr) {
			if (check->name_ == dest_filename) {
				return "destination already has file/directory of same name";
			}
			check = check->rightSibling_;
		}
	} else if (dest == "/") {
		// Moving to root
		dest_parent = root_;
		dest_filename = temp_src->name_; // Keep original name
		
		// Check if root already has a file/directory with the same name
		Node* check = root_->leftmostChild_;
		while (check != nullptr) {
			if (check->name_ == dest_filename) {
				return "destination already has file/directory of same name";
			}
			check = check->rightSibling_;
		}
	} else {
		dest_parent = getParentFromPath(dest, dest_filename);
		if (dest_parent == nullptr) {
			return "invalid path";
		}

		// Check if destination already exists
		Node* temp_dest = dest_parent->leftmostChild_;
		while (temp_dest != nullptr && temp_dest->name_ != dest_filename) {
			temp_dest = temp_dest->rightSibling_;
		}

		if (temp_dest != nullptr) {
			// Destination exists
			if (!temp_dest->isDir_) {
				if (temp_src->isDir_) {
					return "source is a directory but destination is an existing file";
				} else {
					return "destination already has file of same name";
				}
			}
			// Destination is a directory - check if moving into itself
			if (temp_dest == temp_src || isAncestor(temp_src, temp_dest)) {
				return "cannot move source into a subdirectory of itself";
			}
			// Check if destination already has a child with same name as source
			Node* check = temp_dest->leftmostChild_;
			while (check != nullptr) {
				if (check->name_ == temp_src->name_) {
					return "destination already has file/directory of same name";
				}
				check = check->rightSibling_;
			}
			dest_parent = temp_dest;
			dest_filename = temp_src->name_; // Keep original name when moving into directory
		}
	}

	// Get source's parent and remove from it
	Node* src_parent = temp_src->parent_;
	if (temp_src == src_parent->leftmostChild_) {
		src_parent->leftmostChild_ = temp_src->rightSibling_;
	} else {
		Node* prev = src_parent->leftmostChild_;
		while (prev->rightSibling_ != temp_src) {
			prev = prev->rightSibling_;
		}
		prev->rightSibling_ = temp_src->rightSibling_;
	}

	// Update source node
	temp_src->parent_ = dest_parent;
	temp_src->name_ = dest_filename;
	temp_src->rightSibling_ = nullptr;
	insert_inorder(temp_src, dest_parent);
	return "";
}
