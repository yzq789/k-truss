#include <iostream>
#include <vector>
#include "truss.h"
#include "my_timer.h"

#define min(a,b) (((a)<(b))?(a):(b))

using namespace std;

Truss::Truss(Graph &G){
	trussDecomposition(G);
}

void Truss::add(int v1, int v2, int k){
	g[v1][v2] = k;
	g[v2][v1] = k;
}

void Truss::trussDecomposition(Graph G){
	MyTimer my_timer;
	int k = 2;
	G.computeSup();
	my_timer.start("truss decomposition");
	int progress = 0;
	do{
		while (true){
			Edge e = G.lowestSupEdge();
			if (e.w < 0 || e.w > k - 2) break;
			int u, v;
			if (G.deg(e.v1) < G.deg(e.v2)){
				u = e.v1;
				v = e.v2;
			}
			else{
				u = e.v2;
				v = e.v1;
			}

			for (map<int, int>::iterator it = G.N(u).begin(); it != G.N(u).end(); it++){
				int w = it->first;
				if (G.edgeExists(v, w)){
					G.decrementSup(u, w, true);
					G.decrementSup(v, w, true);
				}
			}
			add(u, v, k);
			G.remove(u, v, true);
		}
		k++;
	} while (G.hasEdge());
	my_timer.end();
}

void Truss::getKLevelTriangleList(int u, int v, int k, list<int> &res){
	list<int> commonN;
	getCommonN(u, v, commonN);
	for(int n : commonN)
	{
		int index = n;
		int minv = min(get(u, index), get(v, index));
		if(minv >= k)res.push_back(index);
	}
}

int Truss::getKLevelTriangleListLen(int u, int v, int k) {
	list<int> triList;
	getKLevelTriangleList(u, v, k, triList);
	return triList.size();
}

void Truss::getK1K2(int u, int v, int &k1, int &k2) {
	int k = 2;
	k1 = k2 = 0;
	while(true){
		int num = getKLevelTriangleListLen(u, v, k);
		if(num >= k - 2)k1 = k;
		if(num >= k - 3)k2 = k;

		if(num < k - 3)break;
		k++;
	}
}

void Truss::updateWithEdgeInsertion(int u, int v) {
	MyTimer my_timer;
	int k1, k2;
	getK1K2(u, v, k1, k2);

	add(u, v, k1);
	int kmax = k2 - 1;
	list<int> commonN;
	getCommonN(u, v, commonN);
	Graph Lk;
	for(int w : commonN){
		int k = min(get(w, u), get(w, v));
		if(k <= kmax){
			if(get(w, u) == k)Lk.addEdge(w, u, getKLevelTriangleListLen(w, u, k));
			if(get(w, v) == k)Lk.addEdge(w, v, getKLevelTriangleListLen(w, v, k));
		}
	}

	for(int k = kmax;k >= 2;k--){
		vector<pair<int, int>> Q;
		Q.clear();
		for(MapG::iterator it = Lk.begin();it != Lk.end();it++){
			for(map<int, int>::iterator itm = it->second.begin();itm != it->second.end();itm++){
				Q.push_back(make_pair(it->first, itm->first));
			}
		}
		Graph s;
		while(!Q.empty()){
			pair<int, int> e = Q[Q.size()-1];
			Q.pop_back();
			int x = e.first;
			int y = e.second;
			s.addEdge(x, y, 0);
			list<int> commonNXY;
			getCommonN(x, y, commonNXY);
			for(int z : commonNXY){
				if(get(z, x) < k || get(z, y) < k)continue;
				s.addEdge(x, y, get(x, y) + 1);
				pair<int, int> p(z, x);
				if(get(z, x) == k && !Lk.edgeExists(z, x)){
					Q.push_back(p);
					Lk.addEdge(z, x, getKLevelTriangleListLen(z, x, k));
				}
				pair<int, int> p2(z, y);
				if(get(z, y) == k && !Lk.edgeExists(z, y)){
					Q.push_back(p2);
					Lk.addEdge(z, y, getKLevelTriangleListLen(z, y, k));
				}
			}
		}
		int x, y;
		while(getMinS(Lk, x, y) <= k-2){
			Lk.remove(x, y);
			list<int> commonNXY;
			getCommonN(x, y, commonNXY);
			for(int z : commonN){
				if(get(x, z) < k || get(y, z) < k)continue;
				if(get(x, z) == k && !Lk.edgeExists(x, z))continue;
				if(get(y, z) == k && !Lk.edgeExists(y, z))continue;
				if(Lk.edgeExists(x, z))Lk.addEdge(x, z, Lk.get(x, z) - 1);
				if(Lk.edgeExists(y, z))Lk.addEdge(y, z, Lk.get(y, z) - 1);
			}
		}
		for(MapG::iterator it = Lk.begin();it != Lk.end();it++){
			x = it->first;
			for(map<int, int>::iterator itm = it->second.begin();itm != it->second.end();itm++){
				y = itm->first;
				addEdge(x, y, k + 1);
			}
		}
	}
}

int Truss::getMinS(Graph &s, int &x, int &y){
	x = s.begin()->first;
	y = s.begin()->second.begin()->first;
	int min = s.begin()->second.begin()->second;

	for(MapG::iterator it = begin();it != end();it++){
		for(map<int, int>::iterator itm = it->second.begin();itm != it->second.end();itm++){
			if(itm->second < min){
				min = itm->second;
				x = it->first;
				y = itm->first;
			}
		}
	}
	return min;
}