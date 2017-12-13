#include <unordered_set>
#include "truss.h"

class TCPIndex{
public:
	TCPIndex(Graph &G, Truss &T);
	~TCPIndex();
	unordered_set<int>* Vk(int x, int y, int k);

private:
	unordered_map<int, Graph*> tcp_index;
};