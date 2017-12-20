#include <queue>
#include "query_processor.h"
#include "my_timer.h"
#include "visualize.h"

using namespace std;


QueryProcessor::QueryProcessor(char* filename){
	G = new Graph(filename);
	T = new Truss(*G);
	tcpIndex = new TCPIndex(*G, *T);
	char *p = strrchr(filename, '/');
	strcpy(dataset_name, p + 1);
}

QueryProcessor::~QueryProcessor(){
	delete G, T, tcpIndex;
}

void QueryProcessor::outputCommunities(unordered_set<Community*> *communities, int v, int k){
	char filename[1000];
	sprintf(filename, "./result/%s_%d_%d.txt", dataset_name, v, k);
	FILE *fp = fopen(filename, "w");
	for (unordered_set<Community*>::iterator it = communities->begin(); it != communities->end(); it++){
		Community *community = *it;
		for (Graph::MapG::iterator ct = community->begin(); ct != community->end(); ct++){
			int vetex = ct->first;
			if (ct != community->begin()) fprintf(fp, ",");
			fprintf(fp, "%d", vetex);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void QueryProcessor::queryKTrussCommunities(int v, int k){
	MyTimer my_timer;
	char log[512];
	sprintf(log, "query k-truss communities v=%d k=%d", v, k);
	my_timer.start(log);
	unordered_set<Community*> *communities = new unordered_set<Community*>();
	Graph *visited = new Graph();
	for (map<int, int>::iterator it = G->N(v).begin(); it != G->N(v).end(); it++){
		int u = it->first;
		if (T->get(v, u) >= k && !visited->edgeExists(v,u)){
			Community *C = new Community();
			queue<pair<int,int>> q;
			q.push(make_pair(v, u));
			while (!q.empty()){
				pair<int, int> x_y;
				x_y = q.front();
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
//	my_timer.start("result visualizing");
//	visualize(*communities, v, k, dataset_name);
//	my_timer.end();
//	outputCommunities(communities, v, k);
	for (unordered_set<Community*>::iterator it = communities->begin(); it != communities->end(); it++){
		delete *it;
	}
}

void QueryProcessor::insertEdge(int u, int v) {
	G->addEdge(u, v);
	T->updateWithEdgeInsertion(u, v);
}

void QueryProcessor::displayKTruss(){
	T->display();
}