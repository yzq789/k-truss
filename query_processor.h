#include "tcp_index.h"

#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

typedef Graph Community;

class QueryProcessor{
public:
	QueryProcessor(char* filename);
	~QueryProcessor();
	unordered_set<Community*>* queryKTrussCommunities(int v, int k);
	void insertEdge(int u, int v);
	void displayKTruss();
	TCPIndex *tcpIndex;

private:
	Graph *G;
	Truss *T;
//	TCPIndex *tcpIndex;
	char dataset_name[256];
	void outputCommunities(unordered_set<Community*> *communities,int v, int k);
};

#endif