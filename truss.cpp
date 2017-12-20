#include <iostream>
#include <vector>
#include "truss.h"
#include "my_timer.h"

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

void Truss::getKLevelTriangleList(int u, int v, int k, vector<int> &res){
	vector<int> commonN;
	getCommonN(u, v, commonN);
	for(int n : commonN)
	{
		int index = n;
		int minv = min(get(u, index), get(v, index));
		if(minv >= k)res.push_back(index);
	}
}

int Truss::getKLevelTriangleListLen(int u, int v, int k) {
	vector<int> triList;
	getKLevelTriangleList(u, v, k, triList);
	return triList.size();
}

void Truss::getK1K2(int u, int v, int &k1, int &k2) {
	int k = 2;
	k1 = k2 = 0;
	int lastnum = 0;
	while(true){
		int num = getKLevelTriangleListLen(u, v, k);
		if(num >= k - 2)k1 = k;
		if(lastnum >= k - 2)k2 = k;

		if(lastnum < k - 2)break;
		lastnum = num;
		k++;
	}
}

void Truss::updateWithEdgeInsertion(int u, int v) {
	MyTimer my_timer;
	int k1, k2;
	my_timer.start("compute k1 & k2");
	getK1K2(u, v, k1, k2);
	my_timer.end();

	my_timer.start("init Lk");
	add(u, v, k1);
	int kmax = k2 - 1;
	vector<int> commonN;
	getCommonN(u, v, commonN);
	vector<Graph> Lk;
	for(int k = 0; k <= kmax;k++)
		Lk.push_back(Graph());
	for(int w : commonN){
		int k = min(get(w, u), get(w, v));
		if(k <= kmax){
			if(get(w, u) == k)Lk[k].addEdge(w, u, getKLevelTriangleListLen(w, u, k));
			if(get(w, v) == k)Lk[k].addEdge(w, v, getKLevelTriangleListLen(w, v, k));
		}
	}
	my_timer.end();

	my_timer.start("deal with Lk");
	for(int k = kmax;k >= 2;k--){
		vector<pair<int, int>> Q;
		Q.clear();
		for(Graph::MapG::iterator it = Lk[k].begin();it != Lk[k].end();it++){
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
			vector<int> commonNXY;
			getCommonN(x, y, commonNXY);
			for(int z : commonNXY){
				if(get(z, x) < k || get(z, y) < k)continue;
				s.addEdge(x, y, get(x, y) + 1);
				pair<int, int> p(z, x);
				if(get(z, x) == k && !Lk[k].edgeExists(z, x)){
					Q.push_back(p);
					Lk[k].addEdge(z, x, getKLevelTriangleListLen(z, x, k));
				}
				pair<int, int> p2(z, y);
				if(get(z, y) == k && !Lk[k].edgeExists(z, y)){
					Q.push_back(p2);
					Lk[k].addEdge(z, y, getKLevelTriangleListLen(z, y, k));
				}
			}
		}
		int x, y;
		while(getMinS(Lk[k], x, y) <= k-2){
			Lk[k].remove(x, y);
			vector<int> commonNXY;
			getCommonN(x, y, commonNXY);
			for(int z : commonN){
				if(get(x, z) < k || get(y, z) < k)continue;
				if(get(x, z) == k && !Lk[k].edgeExists(x, z))continue;
				if(get(y, z) == k && !Lk[k].edgeExists(y, z))continue;
				if(Lk[k].edgeExists(x, z))Lk[k].addEdge(x, z, Lk[k].get(x, z) - 1);
				if(Lk[k].edgeExists(y, z))Lk[k].addEdge(y, z, Lk[k].get(y, z) - 1);
			}
		}
		for(Graph::MapG::iterator it = Lk[k].begin();it != Lk[k].end();it++){
			x = it->first;
			for(map<int, int>::iterator itm = it->second.begin();itm != it->second.end();itm++){
				y = itm->first;
				addEdge(x, y, k + 1);
			}
		}
	}
	my_timer.end();
}

int Truss::getMinS(Graph &s, int &x, int &y){
	if(!s.hasEdge())return 10000;

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

int Truss::getMaxK(vector<int> &commonN, int v){
	int maxk, maxindex;
	maxk = maxindex = 0;
	for(int w : commonN){
		if(get(v, w) > maxk){
			maxk = get(v, w);
			maxindex = w;
		}
	}

	return maxindex;
}