#include "graph.h"

class Truss : public Graph{
public:
	Truss(Graph &G);
	int get(int v1, int v2);

private:
	void add(int v1, int v2, int k);
	void trussDecomposition(Graph G);
};