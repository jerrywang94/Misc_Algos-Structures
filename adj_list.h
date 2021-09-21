#pragma once
#include <string>
#include <queue>
#include <iostream>

/*
adjacency list:
edge - struct for edges
vertices - struct for vertices
adj_list(int num_vertices) - constructor, initializes to num_vertices number of vertices
vertices& operator[](int index) - [] operator for retrieving vertices
const vertices& operator[](int index) - const [] operator for retrieving vertices
int size() - returns size of the list
*/
class adj_list {
public:
	struct edge {
		edge(std::string name, int index, int distance = 0) : next_node(name), index(index), distance(distance) {}

		edge* next = nullptr;
		int index;
		std::string next_node;
		int distance;
	};

	struct vertices {
		std::string node_name = "";
		int node_index = 0;
		edge* head = nullptr;
		std::string prev = "";
		int prev_index = 0;
		bool visited = false;
		int dist = -1;

		void add_edge(std::string name, int index, int distance);

	};

	adj_list(int num_vertices) : num_vertices(num_vertices) {
		v_array = new vertices[num_vertices];
		for (int i = 0; i < num_vertices; i++) {
			v_array[i].node_index = i;
		}
	}

	vertices& operator[](int index) {
		if (index >= num_vertices) throw "out_of_range";
		return v_array[index];
	}

	const vertices& operator[](int index) const {
		if (index >= num_vertices) throw "out_of_range";
		return v_array[index];
	}

	~adj_list() {
		delete[] v_array;
	}

	int size() const { return num_vertices; }

private:
	vertices* v_array;
	int num_vertices;

};

void adj_list::vertices::add_edge(std::string name, int index, int distance = 0) {
	if (!head)
		head = new edge(name, index, distance);
	else {
		edge* curr = head;
		while (curr->next)
			curr = curr->next;
		curr->next = new edge(name, index, distance);
	}
	return;
}
