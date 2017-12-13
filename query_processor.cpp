#include <queue>
#include "query_processor.h"
#include "my_timer.h"

using namespace std;

QueryProcessor::QueryProcessor(char* filename){
	G = new Graph(filename);
	T = new Truss(*G);
	tcpIndex = new TCPIndex(*G, *T);
}

QueryProcessor::~QueryProcessor(){
	delete G, T, tcpIndex;
}

unordered_set<Community*>* QueryProcessor::queryKTrussCommunities(int v, int k){
	MyTimer my_timer;
	my_timer.start("query k-truss communities");
	unordered_set<Community*> *communities = new unordered_set<Community*>();
	Graph *visited = new Graph();
	for (map<int, int>::iterator it = G->N(v).begin(); it != G->N(v).end(); it++){
		int u = it->first;
		if (T->get(v, u) >= k && !visited->edgeExists(v,u)){
			Community *C = new Community();
			queue<pair<int,int>> q;
			q.push(make_pair(v, u));
			while (!q.empty()){
				pair<int, int> x_y = q.front();
				q.pop();
				int x = x_y.first, y = x_y.second;
				if (!visited->edgeExists(x,y)){
					unordered_set<int>* Vk = tcpIndex->Vk(x, y, k);
					for (unordered_set<int>::iterator zt = Vk->begin(); zt != Vk->end(); zt++){
						int z = *zt;
						visited->addEdgeOneWay(x, z);
						C->addEdge(x, z);
						if (!visited->edgeExists(z, x)){
							q.push(make_pair(z, x));
						}
					}
					delete Vk;
				}
			}
			communities->insert(C);
		}
	}
	delete visited;
	my_timer.end();
	return communities;
}