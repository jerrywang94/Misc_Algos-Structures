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

// Reimplementation of linked list, similar to std::list
template<class T>
class LList {

public:

	struct node {
		node() {}
		node(T data) : data(data) { }
		node* prev = nullptr;
		node* next = nullptr;
		T data = T();
	};

	class iterator {
	public:
		iterator(node* n) : curr(n) {}
		iterator& operator++() { curr = curr->next; return *this; }
		iterator& operator--() { curr = curr->prev; return *this; }
		T& operator*() { return curr->data; }
		bool operator==(iterator iter) { return this->curr == iter.curr ? true : false; }
		bool operator!=(iterator iter) { return this->curr != iter.curr ? true : false; }
		friend class LList;

	private:
		node* curr = nullptr;
	};

	LList() {
		head = new node;
		tail = head;
	}

	LList(T data) {
		head = new node;
		tail = head;
		head->data = data;
	}

	~LList() {
		node* next = head->next;
		node* temp = head;
		while (temp) {
			next = temp->next;
			delete temp;
			temp = next;
		}
	}

	iterator begin() { return iterator(head); }

	iterator end() { return ++iterator(tail); }

	iterator insert_node(T data, iterator iter) {
		node* new_node = new node(data);
		if (iter == iterator(tail)) {
			iter.curr->next = new_node;
			new_node->prev = iter.curr;
			tail = new_node;
		}
		else {
			node* temp = iter.curr->next;
			iter.curr->next = new_node;
			new_node->prev = iter.curr;

			new_node->next = temp;
			temp->prev = new_node;
		}
		return new_node;
	}

	iterator insert_node(T data) {
		iterator iter{ tail };
		node* new_node = new node(data);
		if (iter == iterator(tail)) {
			iter.curr->next = new_node;
			new_node->prev = iter.curr;
			tail = new_node;
		}
		else {
			node* temp = iter.curr->next;
			iter.curr->next = new_node;
			new_node->prev = iter.curr;

			new_node->next = temp;
			temp->prev = new_node;
		}
		return new_node;
	}

	void delete_node(iterator iter) {
		if (iter == begin()) {
			if (head == tail)
				return; //don't delete if there's only one node left
			node* temp = head;
			head = head->next;
			delete temp;
			head->prev = nullptr;
			iter = begin();
			return;
		}
		iter.curr->prev->next = iter.curr->next;
		if (iter.curr->next != nullptr)
			iter.curr->next->prev = iter.curr->prev;
		else
			tail = iter.curr->prev;
		delete iter.curr;
		return;
	}

private:

	node* head = nullptr;
	node* tail = nullptr;

};