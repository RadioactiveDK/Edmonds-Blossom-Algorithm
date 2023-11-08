#include<bits/stdc++.h>
using namespace std;

const int M = 10;

struct struct_edge
{
  int v;
  struct_edge *n;
};

struct_edge pool[M * M * 2];
struct_edge * adj[M];
struct_edge * top = pool;



int V;
queue<int> q;

vector<int> match(M,-1);
vector<int> parent(M,-1);

int blossom[M];
vector<bool> BFS_Tree(M,false);
vector<bool> in_blossom(M,false);


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
    u = parent[match[u]];
  }
  while (1)
  {
    if (inp[v = blossom[v]])
      return v;
    else
      v = parent[match[v]];
  }
}

void mark_blossom(int lca, int u){
  while (blossom[u] != lca)
  {
    int v = match[u];
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
          q.push(u);
        BFS_Tree[u] = true;
      }
    }
}

int find_augmenting_path(int source){
    for (int i = 0; i < V; i++) blossom[i] = i;

    q.push(source);
    BFS_Tree[source] = true;

    while ( !q.empty() ) {
      int u = q.front();
      q.pop();

      for (struct_edge * e = adj[u]; e; e = e->n) {
        int v = e->v;
        if (blossom[u] != blossom[v] && match[u] != v)
        {
          if ((v == source) || (match[v] != -1 && parent[match[v]] != -1))
          {
            blossom_contraction(source, u, v);
          }
          else if (parent[v] == -1)
          {
              parent[v] = u;
              if (match[v] == -1)
              {
                  return v;
              }
              else if (!BFS_Tree[match[v]])
              {
                  q.push(match[v]);
                BFS_Tree[match[v]] = true;
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
    w = match[v];
    match[v] = u;
    match[u] = v;
    u = w;
  }
  return t != -1;
}

void edmonds(){
    int max_matching = 0;

    for (int u = 0; u < V; u++){
        if (match[u] == -1) max_matching += augment_path(u, find_augmenting_path(u));
    }
    cout<<"Maximum Matching: "<<endl;
    for (int i = 0; i < V; i++){
        if (i < match[i]) cout << i + 1 << " " << match[i] + 1 << endl;
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

    int N, M;
    cin >> V;
    cin >> M;

    for(int i=0;i<M;i++){
        int u, v;
        cin >> u >> v;
        add_edge(u - 1, v - 1);
    }
    edmonds();
}
