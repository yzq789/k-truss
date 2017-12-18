#include <string>
#include "visualize.h"
#include "query_processor.h"

using namespace std;

char* HEAD = "<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <meta charset=\"utf-8\">\n"
"	<script src=\"js/jquery-3.2.1.min.js\"></script>\n"
"    <script src=\"js/echarts.min.js\"></script>\n"
"	<script src=\"js/dataTool.min.js\"></script>\n"
"</head>\n"
"\n"
"<body>\n"
"    <div id=\"main\" style=\"width: 600px;height:400px;\"></div>\n"
"\n"
"	\n"
"	   <script type=\"text/javascript\">\n"
"        var myChart = echarts.init(document.getElementById('main'));\n"
"\n"
"		xml = '<?xml version=\"1.0\" encoding=\"UTF-8\"?>' + \n"
"		 '<gexf xmlns=\"http://www.gexf.net/1.2draft\" version=\"1.2\" xmlns:viz=\"http://www.gexf.net/1.2draft/viz\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd\">' + \n"
"		 '  <meta lastmodifieddate=\"2014-01-30\">' + \n"
"		 '    <creator>Gephi 0.8.1</creator>' + \n"
"		 '    <description></description>' + \n"
"		 '  </meta>' + \n"
"		 '  <graph defaultedgetype=\"undirected\" mode=\"static\">' + \n"
"		 '    <attributes class=\"node\" mode=\"static\">' + \n"
"		 '      <attribute id=\"modularity_class\" title=\"Modularity Class\" type=\"integer\"></attribute>' + \n"
"		 '      <attribute id=\"symbol_size\" title=\"Modularity Class\" type=\"integer\"></attribute>' + \n"
"		 '    </attributes>' + \n"
"		 '    <nodes>' + ";


char *TAIL = "		 '    </edges>' + \n"
"		 '  </graph>' + \n"
"		 '</gexf>';\n"
"		\n"
"		\n"
"		\n"
"		myChart.showLoading();\n"
"		\n"
"			myChart.hideLoading();\n"
"\n"
"			var graph = echarts.dataTool.gexf.parse(xml);\n"
"			var categories = [];\n"
"			for (var i = 0; i < 90; i++) {\n"
"				categories[i] = {\n"
"					name: 'class' + i\n"
"				};\n"
"			}\n"
"			graph.nodes.forEach(function (node) {\n"
"				node.itemStyle = null;\n"
"				if(node.attributes.symbol_size == null)\n"
"					node.symbolSize = 10;\n"
"				else\n"
"					node.symbolSize = node.attributes.symbol_size\n"
"				node.value = node.symbolSize;\n"
"				node.category = node.attributes.modularity_class;\n"
"				// Use random x, y\n"
"				node.x = node.y = null;\n"
"				node.draggable = true;\n"
"			});\n"
"			option = {\n"
"				title: {\n"
"					text: 'dataset : %s\\nv = %d\\nk = %d',\n"
"					subtext: 'Default layout',\n"
"					top: 'bottom',\n"
"					left: 'right'\n"
"				},\n"
"				tooltip: {show:false},\n"
"				animation: false,\n"
"				series : [\n"
"					{\n"
"						name: 'Les Miserables',\n"
"						type: 'graph',\n"
"						layout: 'force',\n"
"						data: graph.nodes,\n"
"						links: graph.links,\n"
"						categories: categories,\n"
"						roam: true,\n"
"						label: {\n"
"							normal: {\n"
"								position: 'right'\n"
"							}\n"
"						},\n"
"						force: {\n"
"							repulsion: 100\n"
"						}\n"
"					}\n"
"				]\n"
"			};\n"
"\n"
"			myChart.setOption(option);\n"
"    </script>\n"
"</body>\n"
"\n"
"\n"
"</html>";


int edge_index = 0;

void WriteOneNode(FILE* fp, int v, int commutity_no){
	char *str = " '      <node id=\"%d\" label=\"vertex:%d\">' + \n"
		" '        <attvalues>' + \n"
		" '          <attvalue for=\"modularity_class\" value=\"%d\"></attvalue>' + \n"
		" '        </attvalues>' + \n"
		" '      </node>' + \n";
	fprintf(fp, str, v, v, commutity_no);
}

void WriteCenterNode(FILE* fp, int v, int commutity_no){
	char *str = " '      <node id=\"%d\" label=\"vertex:%d\">' + \n"
		" '        <attvalues>' + \n"
		" '          <attvalue for=\"modularity_class\" value=\"%d\"></attvalue>' + \n"
		" '          <attvalue for=\"symbol_size\" value=\"20\"></attvalue>' + \n"
		" '        </attvalues>' + \n"
		" '      </node>' + \n";
	fprintf(fp, str, v, v, commutity_no);
}


void WriteOneEdge(FILE *fp, int source, int target){
	
	char *str = "		 '      <edge id=\"%d\" source=\"%d\" target=\"%d\">' + \n"
		"		 '        <attvalues></attvalues>' + \n"
		"		 '      </edge>' + \n";
	fprintf(fp, str, edge_index++, source, target);

}

void visualize(unordered_set<Community*> &communities, int v, int k, char *dataset_name){
	edge_index = 0;
	char filename[1000];
	char *t = strrchr(dataset_name, '.');
	*t = 0;
	sprintf(filename, "./result/%s_%d_%d.html",dataset_name, v, k);
	*t = '.';
	FILE* fp = fopen(filename, "w");
	fprintf(fp, HEAD);
	int community_index = 1;
	WriteCenterNode(fp, v, community_index++);
	for (unordered_set<Community*>::iterator it = communities.begin(); it != communities.end(); it++){
		Community *community = *it;
		for (Graph::MapG::iterator ct = community->begin(); ct != community->end(); ct++){
			int vetex = ct->first;
			if (vetex != v)
				WriteOneNode(fp, vetex, community_index);
		}
		community_index++;
	}

	fprintf(fp, "		 '    </nodes>' + \n");
	fprintf(fp, "		 '    <edges>' + \n");

	for (unordered_set<Community*>::iterator it = communities.begin(); it != communities.end(); it++){
		Community *community = *it;
		for (Graph::MapG::iterator ct = community->begin(); ct != community->end(); ct++){
			int v1 = ct->first;
			map<int, int> &adj_map = ct->second;
			for (map<int, int>::iterator adg_t = adj_map.begin(); adg_t != adj_map.end(); adg_t++){
				int v2 = adg_t->first;
				WriteOneEdge(fp, v1, v2);
			}
		}
	}

	fprintf(fp, TAIL, dataset_name, v, k );

	fclose(fp);
}