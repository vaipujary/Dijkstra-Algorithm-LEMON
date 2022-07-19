#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <unordered_map>
#include "lemon/smart_graph.h"
#include "lemon/dijkstra.h"

using namespace lemon;
using namespace std;

std::istream& get_input_handle(const int ac, std::ifstream& fhandle, const char* fname) {

    if (ac == 4) {
        fhandle.open(fname);
       assert(fhandle.is_open());
        return fhandle;
    }

    else {
        return std::cin;
    }
}

SmartDigraph::Node add_unique_node(lemon::SmartDigraph& g, lemon::SmartDigraph::NodeMap<std::string>& node_id_map, std::unordered_map<std::string, lemon::SmartDigraph::Node>& id_node_map, std::string node) {

    SmartDigraph::Node new_node;

    if(( id_node_map.find(node)) == id_node_map.end()) {
        new_node = g.addNode();
        node_id_map[new_node] = node;
        id_node_map.emplace(node, new_node);
    }

    else {

        new_node = id_node_map.at(node);
    }

    return new_node;
}

void print_path(lemon::Dijkstra<lemon::SmartDigraph, lemon::SmartDigraph::ArcMap<unsigned int>>& dijkstra, SmartDigraph::Node& start, SmartDigraph::Node& end, lemon::SmartDigraph::NodeMap<std::string>& node_id_map){

    list<lemon::SmartDigraph::Node> path;
    SmartDigraph::Node node;

    for (node = end; node != start; node = dijkstra.predNode(node)) {

        if(node != INVALID && dijkstra.reached(node)) {
            path.push_front(node);
        }
    }

    path.push_front(start);
    path.reverse();

    cout << dijkstra.dist(end) <<": ";

   for(auto node = path.rbegin(); node != path.rend(); ++node) {
       cout << node_id_map[*node] << " ";
   }
}

int main(int argc, char** argv) {

    lemon::SmartDigraph g;

    ifstream filehandle;
    string node1;
    string node2;

    string start_node;
    string end_node;

    istream& input = get_input_handle(argc, filehandle, argv[1]);

    if (argc == 4) {
       start_node = argv[2];
       end_node = argv[3];
   }

   else {
       start_node = argv[1];
       end_node = argv[2];
   }

   std::string node_string;

   unsigned int weight;

   SmartDigraph::NodeMap<std::string> node_id_map(g); // LEMON map for node IDs

   unordered_map<std::string, lemon::SmartDigraph::Node> id_node_map;

   SmartDigraph::ArcMap<unsigned int> arc_weights(g);

   SmartDigraph::Arc Arc;

   while(std::getline(input, node_string)) {
       istringstream ss(node_string);
       ss >> node1 >> node2 >> weight;

       SmartDigraph::Node Node1 = add_unique_node(g, node_id_map, id_node_map, node1);
       SmartDigraph::Node Node2 = add_unique_node(g, node_id_map, id_node_map, node2);

       Arc = g.addArc(Node1, Node2);
       arc_weights[Arc] = weight;
   }

    SmartDigraph::Node start = id_node_map.at(start_node);
    SmartDigraph::Node end = id_node_map.at(end_node);

    using SptSolver = Dijkstra<SmartDigraph, SmartDigraph::ArcMap<unsigned int>>;

    SptSolver dijkstra(g, arc_weights);

    dijkstra.run(start, end);

   if(dijkstra.run(start, end)) {
       print_path(dijkstra, start, end, node_id_map);
       return 0;
   }

   else {
        std::cout << "No path exists from " << start_node << " to " << end_node << std::endl;
   }

 return 0;
}