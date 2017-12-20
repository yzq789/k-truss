#include <unordered_set>
#include "truss.h"

#ifndef __TCP_INDEX_H
#define __TCP_INDEX_H


class TCPIndex{
public:
	TCPIndex(Graph &G, Truss &T);
	~TCPIndex();
	unordered_set<int>* Vk(int x, int y, int k);
	void addEdge(int x, int y, int z, int k);
	void updateEdge(int x, int y, int z, int k);
	bool updateEdge(Graph *g, int u, int last, int des, vector<int> &path);
	unordered_map<int, Graph*> tcp_index;
};

#endif