#include <ios>
#include <iostream>
#include "my_timer.h"
#include "query_processor.h"
#include "visualize.h"

using namespace std;

void display(unordered_set<Community*>* communities){
	int counter = 1;
	for(Community* com : *communities){
		cout <<counter <<endl;
		counter++;

		for(Graph::MapG::iterator it = com->begin();it != com->end();it++){
			cout <<it->first <<"\t";
		}
		cout <<endl;
	}
}

int main(){
	MyTimer my_timer;
	my_timer.start("application");
	ios_base::sync_with_stdio(false);
	QueryProcessor *query_processor;

	char *test_path = "/Users/East/CLionProjects/k-truss/data/test.txt";
	query_processor = new QueryProcessor(test_path);
    unordered_set<Community*>* communities = query_processor->queryKTrussCommunities(9, 5);
	display(communities);
//    my_timer.start("result visualizing");
//	visualize(*communities, 9, 5, "test.txt");
//	my_timer.end();

	char *test_path1 = "/Users/East/CLionProjects/k-truss/data/test1.txt";
	query_processor = new QueryProcessor(test_path1);
	query_processor->insertEdge(9, 31);
	unordered_set<Community*>* communities1 = query_processor->queryKTrussCommunities(9, 5);
	display(communities1);
	delete query_processor;

//    cout <<"Begin facebook:" <<endl;
//	char *facebook_path = "/Users/East/CLionProjects/k-truss/data/facebook_combined.txt";
//	query_processor = new QueryProcessor(facebook_path);
//	query_processor->queryKTrussCommunities(414, 20);
//    query_processor->insertEdge(414, 412);
//    query_processor->queryKTrussCommunities(414, 20);
	//query_processor->queryKTrussCommunities(414, 21);
	//query_processor->queryKTrussCommunities(414, 22);
	//query_processor->queryKTrussCommunities(414, 23);
	//query_processor->queryKTrussCommunities(1017, 5);
	//query_processor->queryKTrussCommunities(1017, 10);
	//query_processor->queryKTrussCommunities(1017, 15);
	//query_processor->queryKTrussCommunities(1017, 20);
	//delete query_processor;


	//char *dblp_path = "D:/archive/Study/Modern_Database_System/project3/dataset/DBLP/dblp.ungraph.txt";
	//query_processor = new QueryProcessor(dblp_path);
	//query_processor->queryKTrussCommunities(105653, 3);
	//query_processor->queryKTrussCommunities(105653, 4);
	//query_processor->queryKTrussCommunities(105653, 5);
	//query_processor->queryKTrussCommunities(143836, 3);
	//query_processor->queryKTrussCommunities(143836, 4);
	//query_processor->queryKTrussCommunities(143836, 5);
	//query_processor->queryKTrussCommunities(143836, 6);
	//query_processor->queryKTrussCommunities(143836, 8);
	//query_processor->queryKTrussCommunities(143836, 10);
	//query_processor->queryKTrussCommunities(143836, 15);
	//delete query_processor;

	my_timer.end();
	system("pause");
}


