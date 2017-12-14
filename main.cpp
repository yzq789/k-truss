#include <ios>
#include "my_timer.h"
#include "query_processor.h"

using namespace std;

int main(){
	MyTimer my_timer;
	my_timer.start("application");
	ios_base::sync_with_stdio(false);
	//char *filename = "D:/archive/Study/Modern_Database_System/project3/dataset/Test/test.txt";
	//char *filename = "D:/archive/Study/Modern_Database_System/project3/dataset/DBLP/dblp.ungraph.txt";
	//char *filename = "D:/archive/Study/Modern_Database_System/project3/dataset/Amazon/amazon.ungraph.txt";
	char *filename = "./data/facebook_combined.txt";
	
	QueryProcessor *query_processor = new QueryProcessor(filename);
	unordered_set<Community*> *communities = query_processor->queryKTrussCommunities(0, 4);
	for (unordered_set<Community*>::iterator it = communities->begin(); it != communities->end(); it++){
		delete *it;
	}
	delete communities, query_processor;
	my_timer.end();
}