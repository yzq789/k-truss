#include <set>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

#ifndef __GRAPH_H
#define __GRAPH_H


typedef struct{
	int w;
	int v1;
	int v2;
}Edge;

class Graph{
public :
	Graph();
	Graph(char* filepath);
	int get(int v1, int v2);
	void addEdge(int v1, int v2);
	void addEdge(int v1, int v2, int w);
	void addEdgeOneWay(int v1, int v2);
	void remove(int v1, int v2, bool sup_index_remove = false);
	int sup(int v1, int v2);
	void decrementSup(int v1, int v2, bool has_sup_index = false);
	bool edgeExists(int v1, int v2);
	void computeSup();
	Edge lowestSupEdge();
	int deg(int v);
	map<int, int>& N(int v);
	bool hasEdge();
	typedef unordered_map<int, map<int, int>> MapG;
	MapG::iterator begin();
	MapG::iterator end();
	void getCommonN(int u, int v, vector<int> &res);
	void display();

protected:
	MapG g;
	map<int,set<pair<int,int>>> sup_index;

private:
	void intersetion(map<int,int> &set1, map<int,int> &set2, map<int,int> *inter);
};

#endif