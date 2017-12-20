#include <algorithm>
#include <queue>
#include "tcp_index.h"
#include "my_timer.h"
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

static int FindRoot(int nodeIndex, vector<int> &tree);

int min(int a, int b, int c){
	int tmp = MIN(a, b);
	return MIN(tmp, c);
}

static bool _comp(Edge &e1, Edge &e2){
	return e1.w > e2.w;
}

TCPIndex::TCPIndex(Graph &G, Truss &T){   // Kruskal��s algorithm
	MyTimer my_timer;
	my_timer.start("TCP index construction");
	for (auto it = G.begin(); it != G.end(); it++){
		int x = it->first;
		map<int, int> &adj_map = it->second;
		vector<int> vec_adjv;
		for (map<int, int>::iterator it = adj_map.begin(); it != adj_map.end(); it++){
			vec_adjv.push_back(it->first);
		}
		vector<Edge> vec_e;
		for (int i = 0; i < vec_adjv.size(); i++){
			for (int j = i + 1; j < vec_adjv.size(); j++){
				int y = vec_adjv[i];
				int z = vec_adjv[j];
				if (G.edgeExists(y, z)){
					Edge e;
					e.v1 = i; e.v2 = j;   // to use union-find set, save index here, not real vetex
					e.w = min(T.get(x, y), T.get(x, z), T.get(y, z));
					vec_e.push_back(e);
				}
			}
		}
		sort(vec_e.begin(), vec_e.end(), _comp);
		vector<int> tree(vec_adjv.size());   // Union-Find Set
		for (int i = 0; i < tree.size(); i++) tree[i] = -1;
		int k_max = vec_e.empty() ? 0 : vec_e[0].w;
		Graph *Tx = new Graph();
		for (int i = 0; i < vec_e.size(); i++){
			int y_index = vec_e[i].v1;
			int z_index = vec_e[i].v2;
			int root_y = FindRoot(y_index, tree);
			int root_z = FindRoot(z_index, tree);
			if (root_y != root_z){   // y and z are in different connected components
				int w = vec_e[i].w;
				int y = vec_adjv[y_index];
				int z = vec_adjv[z_index];
				Tx->addEdge(y, z, w);
				tree[root_z] = root_y;
			}
		}
		tcp_index[x] = Tx;
	}
	my_timer.end();
}

static int FindRoot(int nodeIndex, vector<int> &tree){
    if(tree[nodeIndex] == -1) return nodeIndex;
    int root = FindRoot(tree[nodeIndex], tree);
    tree[nodeIndex] = root;
    return root;
}

unordered_set<int>* TCPIndex::Vk(int x, int y, int k){
	Graph *Tx = tcp_index[x];
	unordered_set<int> *z_set = new unordered_set<int>();
	//BFS, figure out connected component
	queue<int> q;
	q.push(y);
	z_set->insert(y);
	while (!q.empty()){
		int z = q.front(); 
		q.pop();
		for (map<int, int>::iterator it = Tx->N(z).begin(); it != Tx->N(z).end(); it++){
			if (it->second >= k && z_set->find(it->first) == z_set->end()){
				z_set->insert(it->first);
				q.push(it->first);
			}
		}
	}
	return z_set;
}


TCPIndex::~TCPIndex(){
	for (unordered_map<int, Graph*>::iterator it; it != tcp_index.end(); it++){
		delete it->second;
	}
}

void TCPIndex::addEdge(int x, int y, int z, int k){
	tcp_index[x]->addEdge(y, z, k);
}

void TCPIndex::updateEdge(int x, int y, int z, int k){
	Graph* g = tcp_index[x];
	vector<int> path;
	path.push_back(y);
	if(!updateEdge(g, y, z, k, path))g->addEdge(y, z, k);
	else{
		int mink = 10000;
		int last = y;
		for(int next : path){
			if(next == y)continue;
			if(g->get(last, next) < mink)mink = g->get(last, next);
			last = next;
		}

		if(mink < k){
			int u = path[path.size() - 2];
			int v = path[path.size() - 1];
			g->remove(u, v);
			g->addEdge(y, z, k);
		}
	}
}

bool TCPIndex::updateEdge(Graph *g, int u, int last, int des, vector<int> &path){
	int res = 0;
	map<int, int> &nei = g->N(u);
	for(map<int, int>::iterator it = nei.begin();it != nei.end();it++){
		if(it->first == last)continue;

		path.push_back(it->first);
		if(it->first == des)return true;
		if(updateEdge(g, it->first, u, des, path))return true;
		path.pop_back();
	}
	return false;
}

