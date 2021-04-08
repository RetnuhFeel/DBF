#include<cstdio>
#include<vector>
#include<map>
#include<fstream>

using namespace std;

class Node{
  public:
    int label;
    int distance;
    Node *backedge;
};

class Edge{
  public:
    Node *u;
    Node *v;
    int weight;
};

class Graph{
  public:
    int num_nodes;
    int num_edges;
    vector<Node *> all_nodes;
    vector<Edge *> all_edges;
    Graph(int n_n, int n_e);
    void BellmanFord(int source);
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
  int e_c;
  int weight;
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

  e_c = 0;
  while(fin >> n1 && fin >> n2 && fin >> weight){
    if(g->all_nodes.at(n1 - 1) == NULL){
      g->all_nodes.at(n1 - 1) = new Node;
      g->all_nodes.at(n1 - 1)->label = n1;
    }
    if(g->all_nodes.at(n2 - 1) == NULL){
      g->all_nodes.at(n2 - 1) = new Node;
      g->all_nodes.at(n2 - 1)->label = n2;
    }
    g->all_edges.at(e_c) = new Edge;
    g->all_edges.at(e_c)->u = g->all_nodes.at(n1 - 1);
    g->all_edges.at(e_c)->v = g->all_nodes.at(n2 - 1);
    g->all_edges.at(e_c)->weight = weight;
    ++e_c;
  }

  /* Find the shortest path from the source to all other nodes */
  g->BellmanFord(source - 1);

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
  all_nodes.resize(n_n, NULL);
  all_edges.resize(n_e, NULL);
}

void fileIOError(char *filename){
  fprintf(stderr, "Error reading from %s -- exiting.\n", filename);
  exit(1);
}

void Graph::BellmanFord(int source){
  unsigned int i;
  unsigned int j;
  unsigned int an_size;
  unsigned int ae_size;
  int w;
  Node *u;
  Node *v;

  an_size = all_nodes.size();
  for(i = 0; i < an_size; ++i){
    u = all_nodes.at(i);
    u->distance = INT_MAX;
    u->backedge = NULL;
  }

  all_nodes.at(source)->distance = 0;

  ae_size = all_edges.size();
  for(i = 0; i < num_nodes - 1; ++i){
    for(j = 0; j < ae_size; ++j){
      u = all_edges.at(j)->u;
      v = all_edges.at(j)->v;
      w = all_edges.at(j)->weight;
      if(u->distance != INT_MAX && u->distance + w < v->distance){
        v->distance = u->distance + w;
        v->backedge = u;
      }
    }
  }

  /* check for negative-weight cycles */
  for(i = 0; i < ae_size; ++i){
    u = all_edges.at(i)->u;
    v = all_edges.at(i)->v;
    w = all_edges.at(i)->weight;
    if(u->distance != INT_MAX && u->distance + w < v->distance){
      fprintf(stderr, "Graph contains a negative-weight cycle\n");
      exit(1);
    }
  }
}
