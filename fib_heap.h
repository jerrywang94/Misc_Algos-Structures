#pragma once
#include <cmath>
#include <limits>
#include <memory>

template <class T>
class fib_heap {
public:

	struct node {
		int key = INT_MIN;
		T* obj_ptr = nullptr;
		enum class status { unmarked, marked, twice_marked };

		node* next = nullptr;
		node* prev = nullptr;
		node* child = nullptr;
		node* parent = nullptr;
		status marked = status::unmarked;

		void inc_marked() {
			if (parent == nullptr)	// root nodes never get marked.
				return;

			switch (marked) {
			case status::unmarked: marked = status::marked; break;
			case status::marked: marked = status::twice_marked; break;
			case status::twice_marked: break;
			}
		}

		int rank() {
			if (!child)
				return 0;
			int children = 1;
			node* curr = child->next;
			while (curr != child) {
				++children;
				curr = curr->next;
			}
			return children;
		}

		node(int key, T* obj_ptr) : key(key), obj_ptr(obj_ptr) {}

		~node() {
			node* next_node = this->next;
			node* double_next;
			delete child;
			while (next_node) {
				if (next_node == this)
					break;
				double_next = next_node->next == this ? nullptr : next_node->next;	// if looped around, set null and end loop
				next_node->next = nullptr;	// Need to set null, or extra delete being recursed
				delete next_node;
				next_node = double_next;
			}
		}
	};

	fib_heap() {}

	~fib_heap() {
		delete min;
	}

	T* find_min() {
		return min->obj_ptr;
	}

	int min_key() {
		if (min == nullptr)
			return INT_MIN;
		return min->key;
	}

	T* extract_min() {
		if (!min) // Empty
			return nullptr;

		T* obj = min->obj_ptr;
		if (min->child == nullptr) {
			min->prev->next = min->next;
			min->next->prev = min->prev;
			node* temp = min->next == min ? nullptr : min->next;
			min->next = nullptr;
			min->prev = nullptr;
			delete min;
			--num_nodes;
			min = temp;
		}
		else {
			min->prev->next = min->next;
			min->next->prev = min->prev;
			node* temp = min->next == min ? nullptr : min->next;
			min->next = nullptr;
			min->prev = nullptr;

			node* curr = min->child;
			do {
				curr->parent = nullptr;
				curr = curr->next;
			} while (curr != min->child);
			if (temp != nullptr)
				merge_node_lists(min->child, temp);
			else
				temp = min->child;
			min->child = nullptr;
			delete min;
			--num_nodes;
			min = temp;
		}

		if (min != nullptr) {
			node* curr = min->next;
			node* new_min = min;
			while (curr != min) {
				if (curr->key < new_min->key)
					new_min = curr;
				curr = curr->next;
			}
			min = new_min;
		}

		consolidate_tree();
		return obj;
	}

	bool decrease_key(int new_key, node* changed_node) {
		if (new_key > changed_node->key)
			return false;

		node* curr_node = changed_node;
		int i = changed_node->key;
		changed_node->key = new_key;

		while (curr_node->parent != nullptr) { // Continuously check upwards until hitting first rank.
			if (curr_node->key <= curr_node->parent->key && curr_node->marked != node::status::twice_marked)
				break;
			else {
				node* temp = curr_node->parent;
				if (temp->child == curr_node) { // if a different child is linked to parent, then nothing further required
					if (curr_node->next == curr_node)
						temp->child = nullptr;
					else
						temp->child = curr_node->next;
				}

				temp->inc_marked();
				curr_node->parent = nullptr;

				curr_node->next->prev = curr_node->prev;
				curr_node->prev->next = curr_node->next;
				curr_node->next = curr_node->prev = curr_node;
				curr_node->marked = node::status::unmarked;

				merge_node_lists(curr_node, min);
				curr_node = temp;
			}
		}

		node* curr = min->next;
		node* new_min = min;
		while (curr != min) {
			if (curr->key < new_min->key)
				new_min = curr;
			curr = curr->next;
		}
		min = new_min;

		consolidate_tree();
		return true;
	}

	void delete_key(node* del_node) {
		if (!decrease_key(INT_MIN, del_node))
			return;
		extract_min();
	}

	void merge_heaps(fib_heap& mheap) {
		if (mheap.size() == 0) // nothing to merge from other heap
			return;

		num_nodes += mheap.num_nodes;
		mheap.num_nodes = 0;

		if (min == nullptr) {
			min = mheap.min;
			mheap.min = nullptr;
			return;
		}

		merge_node_lists(min, mheap.min);

		min = min->key < mheap.min->key ? min : mheap.min;
		mheap.min = nullptr;
	}

	void merge_heaps(fib_heap* mheap) {
		if (mheap->size() == 0) // nothing to merge from other heap
			return;

		num_nodes += mheap->num_nodes;
		mheap->num_nodes = 0;

		if (min == nullptr) {
			min = mheap->min;
			mheap->min = nullptr;
			return;
		}

		merge_node_lists(min, mheap->min);

		min = min->key < mheap->min->key ? min : mheap->min;
		mheap->min = nullptr;
	}

	node* insert(int key, T* obj_ptr = nullptr) {
		if (min == nullptr) {
			min = new node(key, obj_ptr);
			min->prev = min->next = min;
			++num_nodes;
			return min;
		}
		else {
			node* temp = new node(key, obj_ptr);
			temp->prev = temp->next = temp;
			merge_node_lists(min, temp);
			++num_nodes;

			if (temp->key < min->key)
				min = temp;

			return temp;
		}
	}

	int size() {
		return num_nodes;
	}

private:
	node* min = nullptr;
	int num_nodes = 0;

	void consolidate_tree() {
		if (size() == 0)
			return;

		auto rank_arr = std::make_unique<node* []>(static_cast<int>(std::log2(size()) + 2));

		rank_arr[min->rank()] = min;
		node* curr = min->next;

		while (curr != min) {
			if (rank_arr[curr->rank()] == nullptr) {
				rank_arr[curr->rank()] = curr;
				curr = curr->next;
			}
			else {
				node* temp = curr;
				curr = curr->next;
				node* merged_nodes;

				for (;;) {	// Merge nodes until all unique then break in the loop
					int old_rank = temp->rank();
					merged_nodes = link(temp, rank_arr[old_rank]);
					rank_arr[old_rank] = nullptr;
					if (rank_arr[merged_nodes->rank()] == nullptr) {
						rank_arr[merged_nodes->rank()] = merged_nodes;
						break;
					}
					temp = merged_nodes;
				}
			}
		}
	}

	node* link(node* first, node* second) {
		node* new_child = (first->key > second->key || second == min) ? first : second; // second conditional required for corner case: if min->key is second and is equal to first->key
		node* new_parent = (first->key > second->key || second == min) ? second : first;

		new_child->parent = new_parent;
		new_child->prev->next = new_child->next;
		new_child->next->prev = new_child->prev;

		new_child->next = new_child->prev = new_child;

		if (new_parent->child == nullptr)
			new_parent->child = new_child;
		else {
			merge_node_lists(new_child, new_parent->child);

			new_parent->child = new_child; // probably unnecessary, but makes the largest rank subtree the head child node of the parent
		}


		node* c = new_child;
		node* p = new_parent;

		return new_parent;
	}

	void merge_node_lists(node* first, node* second) {
		if (!first || !second)
			return;

		node* temp;
		first->next->prev = second;
		second->next->prev = first;
		temp = first->next;
		first->next = second->next;
		second->next = temp;
		return;
	}

};