/* Dijkstra's Algorithm https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm *
 * implemented in cpp by Hunter N. Leef.                                     */
#include<cstdio>
#include<vector>
#include<map>
#include<fstream>

using namespace std;

class Node{
  public:
    int label;
    int distance;
    bool visited;
    vector<int> adj_list;
    Node *backedge;
};

class Graph{
  public:
    int num_nodes;
    int num_edges;
    vector< vector<int> > adj_mat;
    vector<Node *> all_nodes;
    Graph(int n_n, int n_e);
    void Dijkstra(int source, int target);
};

void printUsage();

void fileIOError(char *filename);

int main(int argc, char **argv){
  ifstream fin;
  int source;
  int target;
  int n_n;
  int n_e;
  int n1;
  int n2;
  unsigned int weight;
  Graph *g;

  if(argc < 4){ printUsage(); }

  sscanf(argv[2], "%d", &source);
  sscanf(argv[3], "%d", &target);

  fin.open(argv[1]);
  if(fin.fail() || fin.bad()){
    fprintf(stderr, "%s failed to open or is bad.\n", argv[1]);
    exit(1);
  }

  if(!(fin >> n_n)){ fileIOError(argv[1]); }
  if(!(fin >> n_e)){ fileIOError(argv[1]); }

  g = new Graph(n_n, n_e);

  while(fin >> n1 && fin >> n2 && fin >> weight){
    if(g->all_nodes.at(n1 - 1) == NULL){
      g->all_nodes.at(n1 - 1) = new Node;
      g->all_nodes.at(n1 - 1)->label = n1;
    }
    g->all_nodes.at(n1 - 1)->adj_list.push_back(n2 - 1);
    if(g->all_nodes.at(n2 - 1) == NULL){
      g->all_nodes.at(n2 - 1) = new Node;
      g->all_nodes.at(n2 - 1)->label = n2;
    }
    g->all_nodes.at(n2 - 1)->adj_list.push_back(n1 - 1);
    g->adj_mat.at(n1 - 1).at(n2 - 1) = weight;
    g->adj_mat.at(n2 - 1).at(n1 - 1) = weight;
  }

  /* Find the shortest path from the source to all other nodes */
  g->Dijkstra(source - 1, target - 1);

  printf("Node\n");
  printf("%5d ", g->all_nodes.at(0)->label);
  for(unsigned int i = 1; i < g->all_nodes.size() - 1; ++i){
    printf("|%5d ", g->all_nodes.at(i)->label);
  }
  printf("|%5d\n", g->all_nodes.at(g->all_nodes.size() - 1)->label);

  printf("Distance\n");
  printf("%5d ", g->all_nodes.at(0)->distance);
  for(unsigned int i = 1; i < g->all_nodes.size() - 1; ++i){
    printf("|%5d ", g->all_nodes.at(i)->distance);
  }
  printf("|%5d\n", g->all_nodes.at(g->all_nodes.size() - 1)->distance);

  printf("Backedge (prev node)\n");
  for(unsigned int i = 0; i < g->all_nodes.size(); ++i){
    Node *n = g->all_nodes.at(i);
    if(n->backedge == NULL){
      if(i == 0){ printf(" NULL "); }
      else if(i < g->all_nodes.size() - 1){ printf("| NULL "); }
      else{ printf("| NULL\n"); }
    }
    else if(i == 0){ printf("%5d ", g->all_nodes.at(i)->backedge->label); }
    else if(i > 0 && i < g->all_nodes.size() - 1){ printf("|%5d ", g->all_nodes.at(i)->backedge->label); }
    else{ printf("|%5d\n", g->all_nodes.at(i)->backedge->label); }
  }

  if(target > 0){
    vector<int> path;
    Node *n;
    n = g->all_nodes.at(target - 1);
    while(n != NULL){
      path.push_back(n->label);
      n = n->backedge;
    }
    printf("\nPath from %d to %d\n", source, target);
    for(unsigned int i = path.size() - 1; i > 0; --i){
      printf("%d -> ", path.at(i));
    }
    printf("%d\n", path.at(0));
  }

  delete g;

  return 0;
}

void printUsage(){
  printf("usage:  ./dijkstra graph-file source target\n");
  exit(1);
}

Graph::Graph(int n_n, int n_e){
  num_nodes = n_n;
  num_edges = n_e;
  adj_mat.resize(n_n);
  for(unsigned int i = 0; i < n_n; ++i){ adj_mat.at(i).resize(n_n, 0); }
  all_nodes.resize(n_n, NULL);
}

void fileIOError(char *filename){
  fprintf(stderr, "Error reading from %s -- exiting.\n", filename);
  exit(1);
}

void Graph::Dijkstra(int source, int target){
  /* nsd -- nodes sorted/keyed on distance */
  multimap<int, Node *> nsd;
  multimap<int, Node *>::iterator mit;
  unsigned int i;
  Node *src;
  Node *n;
  Node *n_n;

  for(i = 0; i < all_nodes.size(); ++i){
    all_nodes.at(i)->distance = -1;
    all_nodes.at(i)->visited = false;
  }
  src = all_nodes.at(source);
  src->distance = 0;
  src->backedge = NULL;
  nsd.insert(make_pair(src->distance, src));

  while(!nsd.empty()){
    n = nsd.begin()->second;
    nsd.erase(nsd.begin());
    n->visited = true;

    for(i = 0; i < n->adj_list.size(); ++i){
      int d;
      n_n = all_nodes.at(n->adj_list.at(i));
      d = n->distance + adj_mat.at(n->label - 1).at(n_n->label - 1);

      if(!n_n->visited && (n_n->distance == -1 || d < n_n->distance)){
        mit = nsd.find(n_n->distance);
        if(mit != nsd.end() && mit->second != n_n){
          pair <multimap<int, Node *>::iterator, multimap<int, Node *>::iterator> ret;
          ret = nsd.equal_range(n_n->distance);
          for(mit = ret.first; mit != ret.second; ++mit){
            if(mit->second == n_n){
              nsd.erase(mit);
              break;
            }
          }
        }
        n_n->distance = d;
        n_n->backedge = n;
        nsd.insert(make_pair(d, n_n));
      }
    }
  }
}
