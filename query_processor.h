#include "tcp_index.h"

typedef Graph Community;

class QueryProcessor{
public:
	QueryProcessor(char* filename);
	~QueryProcessor();
	unordered_set<Community*>* queryKTrussCommunities(int v, int k);

private:
	Graph *G;
	Truss *T;
	TCPIndex *tcpIndex;
};