#include <stdio.h>
#include "query_processor.h"

using namespace std;

int main(){

	//char *filename = "D:/archive/Study/Modern_Database_System/project3/dataset/Test/test.txt";
	char *filename = "D:/archive/Study/Modern_Database_System/project3/dataset/DBLP/dblp.ungraph.txt";
	QueryProcessor *query_processor = new QueryProcessor(filename);
	unordered_set<Community*> *communities = query_processor->queryKTrussCommunities(9, 4);

	for (unordered_set<Community*>::iterator it = communities->begin(); it != communities->end(); it++){
		delete *it;
	}
	delete communities, query_processor;
}