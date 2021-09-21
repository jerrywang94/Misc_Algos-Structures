#pragma once
#include "adj_list.h"
#include <vector>
#include "fib_heap.h"
/*
* Implementation of Dijkstra's Shortest Path Algorithm without the use of a priority queue.
*/
void DJK_noPQ(adj_list& alist, int init, int dest) {

	adj_list::vertices* curr = &alist[init];
	curr->dist = 0;

	while (curr && curr != &alist[dest]) {
		adj_list::edge* iter = curr->head;
		while (iter) {
			if (alist[iter->index].dist == -1 || (iter->distance + curr->dist) < alist[iter->index].dist) {
				alist[iter->index].dist = iter->distance + curr->dist;
				alist[iter->index].prev = curr->node_name;
				alist[iter->index].prev_index = curr->node_index;
			}
			iter = iter->next;
		}
		curr->visited = true;

		curr = nullptr;
		for (int i = 0; i < alist.size(); i++) {
			if (curr == nullptr) {
				if (!alist[i].visited)
					curr = &alist[i];
			}
			else {
				if (!alist[i].visited && alist[i].dist < curr->dist && alist[i].dist != -1)
					curr = &alist[i];
			}
		}
	}
}

// Node struct for use in DJK_std_pq
struct djk_std_pq_node {
	int index = -1;
	int dist = -1;
	djk_std_pq_node(int index, int dist) : index(index), dist(dist) {}
	friend bool operator>(const djk_std_pq_node& lhs, const djk_std_pq_node& rhs) {
		return lhs.dist > rhs.dist;
	}
};

/*
* Implementation of Dijkstra's Shortest Path Algorithm using std::priority_queue.
* Since std::priority_queue does not allow for priority updates of specific elements,
* this simply re-adds the same element back into the queue with an updated priority value
* whenever it needs to be updated. Stale values are ignored when extracted from the pq.
*/
void DJK_std_pq(adj_list& alist, int init, int dest) {

	adj_list::vertices* curr = &alist[init];
	curr->dist = 0;

	std::priority_queue <djk_std_pq_node, std::vector<djk_std_pq_node>, std::greater<djk_std_pq_node>> aq;

	while (curr && curr != &alist[dest]) {
		adj_list::edge* iter = curr->head;
		while (iter) {
			if (alist[iter->index].dist == -1 || (iter->distance + curr->dist) < alist[iter->index].dist) {
				alist[iter->index].dist = iter->distance + curr->dist;
				alist[iter->index].prev = curr->node_name;
				alist[iter->index].prev_index = curr->node_index;
				aq.push(djk_std_pq_node(iter->index, alist[iter->index].dist));
			}
			iter = iter->next;
		}
		curr->visited = true;

		curr = nullptr;

		while (!aq.empty()) {
			if (!alist[aq.top().index].visited)
				break;
			aq.pop();
		}

		if (aq.empty())
			return;
		curr = &alist[aq.top().index];
		aq.pop();
	}

}

/*
* Implementation of Dijkstra's Shortest Path Algorithm using custom fib_heap
* implementation. Node priorities are updated dynamically. Slightly faster than
* DJK_std_pq through my small amount of testing.
*/
void DJK_new_pq(adj_list& alist, int init, int dest) {

	adj_list::vertices* curr = &alist[init];
	curr->dist = 0;

	fib_heap<adj_list::vertices> fh;
	using nodetype = fib_heap<adj_list::vertices>::node;
	auto node_ptr_arr = std::make_unique<nodetype* []>(alist.size());

	while (curr && curr != &alist[dest]) {
		adj_list::edge* iter = curr->head;
		while (iter) {
			if (alist[iter->index].dist == -1 || (iter->distance + curr->dist) < alist[iter->index].dist) {
				alist[iter->index].dist = iter->distance + curr->dist;
				alist[iter->index].prev = curr->node_name;
				alist[iter->index].prev_index = curr->node_index;

				if (node_ptr_arr[iter->index] != nullptr) {
					nodetype* ab = node_ptr_arr[iter->index];
					fh.decrease_key(alist[iter->index].dist, node_ptr_arr[iter->index]);
				}
				else
				{
					node_ptr_arr[iter->index] = fh.insert(alist[iter->index].dist, &alist[iter->index]);
				}

			}
			iter = iter->next;
		}
		curr->visited = true;
		curr = nullptr;

		if (fh.size() == 0)
			return;
		else {
			curr = fh.extract_min();
			node_ptr_arr[curr->node_index] = nullptr;
		}
	}

}