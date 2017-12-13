#include <stdlib.h>
#include <limits.h>
#include <iterator>
#include <algorithm>
#include "my_timer.h"
#include "graph.h"

using namespace std;



Graph::Graph(){}

Graph::Graph(char* filepath){
	MyTimer my_timer;
	my_timer.start("create gragh");
	FILE* fp = fopen(filepath, "r");
	int v1, v2;
	while (fscanf(fp, "%d%d", &v1, &v2) != EOF){
		addEdge(v1, v2);
	}
	fclose(fp);
	my_timer.end();
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

void Graph::remove(int v1, int v2){
	g[v1].erase(v2);
	g[v2].erase(v1);
}

int Graph::sup(int v1, int v2){
	return g[v1][v2];
}

void Graph::decrementSup(int v1, int v2){
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
	for (MapG::iterator it = g.begin(); it != g.end(); ++it){
		if (progress++ % (g.size() / 100) == 0) printf("%.2f%% points completed.\n", (float)progress / g.size() * 100);
		int v1 = it->first;
		map<int, int> &adj_map = it->second;
		for (map<int, int>::iterator jt = adj_map.begin(); jt != adj_map.end(); jt++){
			int v2 = jt->first;
			if (g[v1][v2] < 0){
				map<int, int> inter;
				intersetion(N(v1), N(v2), &inter);
				g[v2][v1] = g[v1][v2] = inter.size();
			}
		}
	}
	my_timer.end();
}

Edge Graph::lowestSupEdge(){
	MyTimer my_timer;
	my_timer.start("pop lowest support edge");
	int min = INT_MAX;
	int min_v1, min_v2;
	for (MapG::iterator it = g.begin(); it != g.end(); ++it){
		int v1 = it->first;
		map<int, int> adj_map = it->second;
		for (map<int, int>::iterator jt = adj_map.begin(); jt != adj_map.end(); jt++){
			int v2 = jt->first;
			if (g[v1][v2] < min){
				min = g[v1][v2];
				min_v1 = v1;
				min_v2 = v2;
			}
		}
	}
	Edge e;
	if (min == INT_MAX){
		e.w = -1;
	}
	else{
		e.w = min;
		e.v1 = min_v1;
		e.v2 = min_v2;
	}
	my_timer.end();
	return e;
}

int Graph::deg(int v){
	return g[v].size();
}

map<int, int>& Graph::N(int v){
	return g[v];
}

bool Graph::hasEdge(){
	for (MapG::iterator it = g.begin(); it != g.end(); ++it){
		if (it->second.size() > 0) return true;
	}
	return false;
}

static bool comp(pair<const int, int> p1, pair<const int, int> p2){
	return p1.first < p2.first;
}

void Graph::intersetion(map<int, int> set1, map<int, int> set2, map<int, int> *inter){
	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(*inter, inter->begin()), comp);
}

Graph::MapG::iterator Graph::begin(){
	return g.begin();
}
Graph::MapG::iterator Graph::end(){
	return g.end();
}