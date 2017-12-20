#include <stdlib.h>
#include <limits.h>
#include <iterator>
#include <algorithm>
#include <iostream>
#include "my_timer.h"
#include "graph.h"

using namespace std;

Graph::Graph(){}

Graph::Graph(char* filepath){
	MyTimer my_timer;
	my_timer.start("create gragh");
	g.reserve(500000);
	FILE* fp = fopen(filepath, "r");
	int v1, v2;
	while (fscanf(fp, "%d%d", &v1, &v2) != EOF){
		addEdge(v1, v2);
	}
	fclose(fp);
	my_timer.end();
}

int Graph::get(int v1, int v2){
	return g[v1][v2];
}

void Graph::addEdge(int v1, int v2){
	addEdge(v1, v2, -1);
}

void Graph::addEdge(int v1, int v2, int w){
	g[v1][v2] = w;
	g[v2][v1] = w;
}

void Graph::addEdgeOneWay(int v1, int v2){
	g[v1][v2] = 0;
}

void Graph::remove(int v1, int v2, bool sup_index_remove){
	if (sup_index_remove){
		int sup = g[v1][v2];
		pair<int, int> e = v1 < v2 ? make_pair(v1, v2) : make_pair(v2, v1);
		sup_index[sup].erase(e);
		if (sup_index[sup].empty()) sup_index.erase(sup);
	}
	g[v1].erase(v2);
	g[v2].erase(v1);
}

int Graph::sup(int v1, int v2){
	return g[v1][v2];
}

void Graph::decrementSup(int v1, int v2, bool has_sup_index){
	if (has_sup_index){
		int sup = g[v1][v2];
		pair<int, int> e = v1 < v2 ? make_pair(v1, v2) : make_pair(v2, v1);
		sup_index[sup].erase(e);
		if (sup_index[sup].empty()) sup_index.erase(sup);
		sup_index[sup - 1].insert(e);
	}
	g[v1][v2]--;
	g[v2][v1]--;
}

bool Graph::edgeExists(int v1, int v2){
	return (g.find(v1) != g.end() && g[v1].find(v2) != g[v1].end());
}

void Graph::computeSup(){
	MyTimer my_timer;
	my_timer.start("compute supports");
	int progress = 0;
	for (auto it = g.begin(); it != g.end(); ++it){
		int v1 = it->first;
		map<int, int> &adj_map = it->second;
		for (map<int, int>::iterator jt = adj_map.begin(); jt != adj_map.end(); jt++){
			int v2 = jt->first;
			if (g[v1][v2] < 0){
				map<int, int> inter;
				intersetion(N(v1), N(v2), &inter);
				int sup = inter.size();
				g[v2][v1] = g[v1][v2] = sup;
				pair<int, int> e = v1 < v2 ? make_pair(v1, v2) : make_pair(v2, v1);
				sup_index[sup].insert(e);
			}
		}
	}
	my_timer.end();
}

Edge Graph::lowestSupEdge(){
	MyTimer my_timer;
	Edge e;
	if (sup_index.empty()){
		e.w = -1;
		return e;
	}
	int min_sup = sup_index.begin()->first;
	pair<int, int> edge = *(sup_index.begin()->second.begin());
	e.w = min_sup;
	e.v1 = edge.first;
	e.v2 = edge.second;
	return e;
}

int Graph::deg(int v){
	return g[v].size();
}

map<int, int>& Graph::N(int v){
	return g[v];
}

bool Graph::hasEdge(){
	for (auto it = g.begin(); it != g.end(); ++it){
		if (it->second.size() > 0) return true;
	}
	return false;
}

static bool comp(pair<const int, int> p1, pair<const int, int> p2){
	return p1.first < p2.first;
}

void Graph::intersetion(map<int, int> &set1, map<int, int> &set2, map<int, int> *inter){
	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(*inter, inter->begin()), comp);
}

Graph::MapG::iterator Graph::begin(){
	return g.begin();
}
Graph::MapG::iterator Graph::end(){
	return g.end();
}

void Graph::getCommonN(int u, int v, vector<int> &commonN){
	map<int, int> res;
	intersetion(N(u), N(v), &res);
	for(map<int,int>::iterator it = res.begin();it != res.end();it++)
		commonN.push_back(it->first);
}

void Graph::display() {
//	sort(begin(), end());
	for(auto it = begin();it != end();it++){
		for(map<int, int>::iterator itm = it->second.begin();itm != it->second.end();itm++){
			cout <<it->first <<"\t" <<itm->first <<"\t" <<itm->second <<endl;
		}
	}
}
41 32
44 32