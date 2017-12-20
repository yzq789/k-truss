#include "graph.h"
#include "vector"

#ifndef __TRUSS_H
#define __TRUSS_H


class Truss : public Graph{
public:
	Truss(Graph &G);
	void updateWithEdgeInsertion(int u, int v);
	int getMaxK(vector<int> &commonN, int v);
	int getMinKInTri(int x, int y, int z);

private:
	void add(int v1, int v2, int k);
	void trussDecomposition(Graph G);
	void getKLevelTriangleList(int u, int v, int k, vector<int> &res);
	int getKLevelTriangleListLen(int u, int v, int k);
	void getK1K2(int u, int v, int &k1, int &k2);
	int getMinS(Graph &s, int &x, int &y);
};

#endif