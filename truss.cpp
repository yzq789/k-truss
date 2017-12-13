#include "truss.h"
#include "my_timer.h"

Truss::Truss(Graph &G){
	trussDecomposition(G);
}

void Truss::add(int v1, int v2, int k){
	g[v1][v2] = k;
	g[v2][v1] = k;
}

int Truss::get(int v1, int v2){
	return g[v1][v2];
}

void Truss::trussDecomposition(Graph G){
	MyTimer my_timer;
	my_timer.start("truss decomposition");
	int k = 2;
	G.computeSup();
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
					G.decrementSup(u, w);
					G.decrementSup(v, w);
				}
			}
			add(u, v, k);
			G.remove(u, v);
			if (progress++ % 10498 == 0) printf("%.2f%% truss decomposition completed.\n", (float)progress / 1049866 * 100);
		}
		k++;
	} while (G.hasEdge());
	my_timer.end();
}