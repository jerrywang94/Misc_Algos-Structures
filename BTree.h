#pragma once

// Struct to represent nodes in linked list / Binary Tree
template<class T>
struct node {
	node* left = nullptr;
	node* right = nullptr;
	node* parent = nullptr;
	T data = T();
	node(T data) : data(data) {}
};

/*
* Implementation of binary trees:
* void insert_node(T data) - insert node containing data
* void delete_node(node* d_node) - delete node represented in the pointer
* void delete_node(T data) - delete node containing data
* node* find_node(T data) - return pointer to node containing data
* void print_all() - print all nodes inside tree (BFS)
*/
template<class T>
class BTree {
public:

	using node = node<T>;

	void insert_node(T data) {
		if (root == nullptr) {
			root = new node(data);
			return;
		}
		node* curr = root;

		while (true) {
			if (curr->data == data)
				return;

			if (curr->data > data) {
				if (curr->left == nullptr) {
					curr->left = new node(data);
					curr->left->parent = curr;
					return;
				}
				else
					curr = curr->left;
			}
			else {
				if (curr->right == nullptr) {
					curr->right = new node(data);
					curr->right->parent = curr;
					return;
				}
				else
					curr = curr->right;
			}
		}
	}

	void delete_node(node* d_node) {
		if (d_node == nullptr)
			return;

		//leaf node
		if (d_node->left == nullptr && d_node->right == nullptr) {
			if (d_node->parent->right == d_node)
				d_node->parent->right = nullptr;
			else
				d_node->parent->left = nullptr;
			delete d_node;
			return;
		}

		if (d_node == root) {
			if (d_node->right != nullptr) {
				node* r_branch = d_node->right;

				if (d_node->left == nullptr) {
					root = r_branch;
					r_branch->parent = d_node->parent;
					delete d_node;
					return;
				}

				node* l_btm_node = d_node->left;

				while (l_btm_node->right != nullptr)
					l_btm_node = l_btm_node->right;
				l_btm_node->right = r_branch;
				r_branch->parent = l_btm_node;

				d_node->left->parent = d_node->parent;
				root = d_node->left;
				delete d_node;
				return;
			}
			else {
				node* l_branch = d_node->left;

				if (d_node->right == nullptr) {
					root = l_branch;
					l_branch->parent = d_node->parent;
					delete d_node;
					return;
				}

				node* r_btm_node = d_node->right;

				while (r_btm_node->left != nullptr)
					r_btm_node = r_btm_node->left;
				r_btm_node->left = l_branch;
				l_branch->parent = r_btm_node;

				d_node->right->parent = d_node->parent;
				root = d_node->right;
				delete d_node;
			}
		}

		//not leaf node
		if (d_node->parent->right == d_node) {	//right side node being deleted
			node* l_branch = d_node->left;

			if (d_node->right == nullptr) {
				d_node->parent->right = l_branch;
				l_branch->parent = d_node->parent;
				delete d_node;
				return;
			}

			node* r_btm_node = d_node->right;

			while (r_btm_node->left != nullptr)
				r_btm_node = r_btm_node->left;
			r_btm_node->left = l_branch;
			l_branch->parent = r_btm_node;

			d_node->right->parent = d_node->parent;
			d_node->parent->right = d_node->right;
			delete d_node;
		}
		else if (d_node->parent->left == d_node) {	// left side node being deleted
			node* r_branch = d_node->right;

			if (d_node->left == nullptr) {
				d_node->parent->left = r_branch;
				r_branch->parent = d_node->parent;
				delete d_node;
				return;
			}

			node* l_btm_node = d_node->left;

			while (l_btm_node->right != nullptr)
				l_btm_node = l_btm_node->right;
			l_btm_node->right = r_branch;
			r_branch->parent = l_btm_node;

			d_node->left->parent = d_node->parent;
			d_node->parent->left = d_node->left;
			delete d_node;
		}

	}

	void delete_node(T data) {
		delete_node(find_node(data));
	}

	node* find_node(T data) {
		node* curr = root;
		while (curr) {
			if (curr->data == data)
				return curr;
			else if (curr->data > data)
				curr = curr->left;
			else
				curr = curr->right;
		}
		return nullptr;
	}

	void print_all() {
		std::queue<node*> node_q;
		node_q.push(root);
		node_q.push(nullptr);
		node* curr;

		while (!node_q.empty()) {
			curr = node_q.front();
			if (!curr) {
				std::cout << "\n";
				node_q.pop();
				if (node_q.empty())
					return;
				node_q.push(nullptr);
				continue;
			}
			std::cout << node_q.front()->data << "_";
			if (node_q.front()->left)
				node_q.push(node_q.front()->left);
			if (node_q.front()->right)
				node_q.push(node_q.front()->right);
			node_q.pop();
		}

		return;
	}

private:
	node* root = nullptr;
};