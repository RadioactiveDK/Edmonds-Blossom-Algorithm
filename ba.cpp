#include<bits/stdc++.h>
using namespace std;

const int M = 500;
struct struct_edge
{
  int v;
  struct_edge *n;
};
typedef struct_edge *edge;
struct_edge pool[M * M * 2];
edge adj[M];
edge top = pool;

vector<int> mate(M,-1);

int V, E;
int queuee_head, queuee_tail, queuee[M];



queue<int> q;

vector<int> parent(M,-1);

int blossom[M];
vector<bool> BFS_Tree(M,false);

vector<bool> in_blossom(M,false);
vector<vector<bool>> Edge(M,vector<bool> (M,false));

void add_edge(int u, int v){
  top->v = v, top->n = adj[u], adj[u] = top++;
  top->v = u, top->n = adj[v], adj[v] = top++;
}

int LCA(int root, int u, int v){
  static vector<bool> inp(M,false);
  while (1)
  {
    inp[u = blossom[u]] = true;
    if (u == root)
      break;
    u = parent[mate[u]];
  }
  while (1)
  {
    if (inp[v = blossom[v]])
      return v;
    else
      v = parent[mate[v]];
  }
}

void mark_blossom(int lca, int u){
  while (blossom[u] != lca)
  {
    int v = mate[u];
    in_blossom[blossom[u]] = in_blossom[blossom[v]] = true;
    u = parent[v];
    if (blossom[u] != lca)
      parent[u] = v;
  }
}

void blossom_contraction(int s, int u, int v){  
  int lca = LCA(s, u, v);
    for(int i=0;i<in_blossom.size();i++)in_blossom[i]=false;
  mark_blossom(lca, u);
  mark_blossom(lca, v);
  if (blossom[u] != lca)
    parent[u] = v;
  if (blossom[v] != lca)
    parent[v] = u;
  for (int u = 0; u < V; u++)
    if (in_blossom[blossom[u]])
    {
      blossom[u] = lca;
      if (!BFS_Tree[u])
      {
        BFS_Tree[queuee[++queuee_tail] = u] = true;
      }
    }
}

int find_augmenting_path(int source){
    for (int i = 0; i < V; i++) blossom[i] = i;
    
    queuee_head = 0;
    queuee_tail = 0;
    BFS_Tree[queuee[queuee_head] = source] = true;
    while (queuee_head <= queuee_tail)
    {
      int u = queuee[queuee_head++];
      for (edge e = adj[u]; e; e = e->n)
      {
        int v = e->v;
        if (blossom[u] != blossom[v] && mate[u] != v)
        {
          if ((v == source) || (mate[v] != -1 && parent[mate[v]] != -1))
          {
            blossom_contraction(source, u, v);
          }
          else if (parent[v] == -1)
          {
              parent[v] = u;
              if (mate[v] == -1)
              {
                  return v;
              }
              else if (!BFS_Tree[mate[v]])
              {
                BFS_Tree[queuee[++queuee_tail] = mate[v]] = true;
              }
          }
        }
      }
  }
  return -1;
}

int augment_path(int s, int t){
  int u = t, v, w;
  while (u != -1)
  {
    v = parent[u];
    w = mate[v];
    mate[v] = u;
    mate[u] = v;
    u = w;
  }
  return t != -1;
}

void edmonds(){
    int max_matching = 0;

    for (int u = 0; u < V; u++){
        if (mate[u] == -1) max_matching += augment_path(u, find_augmenting_path(u));
    }
    cout<<"Maximum Matching: "<<endl;
    for (int i = 0; i < V; i++){
        if (i < mate[i]) cout << i + 1 << " " << mate[i] + 1 << endl;
    }
}

int main(){
    freopen("input.txt", "r", stdin);
	// freopen("output.txt", "w", stdout);

    cout<<"\nVertices are labelled from 1 to N\n"
        <<"INPUT FORMAT for 'input.txt'\n"
        <<"\tN M \n"
        <<"\te_1 {u_1 v_1}\n"
        <<"\te_2\n\t.\n\t.\n\t.\n"
        <<"\te_M\n\n";

    cin>>V;
    cin>>E;
    for(int i=0;i<E;i++){
        int u, v;
        cin >> u >> v;
        if (!Edge[u - 1][v - 1]){
            add_edge(u - 1, v - 1);
            Edge[u - 1][v - 1] = Edge[v - 1][u - 1] = true;
        }
    }
    edmonds();
}