#include<bits/stdc++.h>
using namespace std;

const int M = 50;

queue<int> q;
vector<int> match(M,-1), parent(M,-1), blossom(M);
vector<bool> BFS_Tree(M,false);

int LCA(int root, int u, int v){
  vector<bool> vis(M,false);
  vis[u]=true;
  while (1)  {
    u = blossom[u];
    vis[u] = true;
    if (u == root) break;
    u = parent[match[u]];
  }
  while (1){
    v = blossom[v];
    if (vis[v])  return v;
    else v = parent[match[v]];
  }
}

void mark_blossom(vector<bool> &in_blssm, int lca, int u){
  while (blossom[u] != lca)  {
    int v = match[u];
    in_blssm[blossom[u]] = in_blssm[blossom[v]] = true;
    u = parent[v];
    // if (blossom[u] != lca)  parent[u] = v;
  }
}

void blossom_contraction(vector<vector<int>> &adj, int s, int u, int v){  
  vector<bool> in_blssm(adj.size(), false);
  int lca = LCA(s, u, v);
  mark_blossom(in_blssm, lca, u);
  mark_blossom(in_blssm, lca, v);
  if (blossom[u] != lca)  parent[u] = v;
  if (blossom[v] != lca)  parent[v] = u;
  
  for (int i = 0; i < adj.size(); i++)
    if (in_blssm[blossom[i]]) {
      blossom[i] = lca;
      if (!BFS_Tree[u]) {
        q.push(i);
        BFS_Tree[i] = true;
      }
    }
}

int find_augmenting_path( vector<vector<int>> &adj, int source){
  for (int i = 0; i < adj.size(); i++) {
    BFS_Tree[i] = false;
    blossom[i] = i;
    parent[i] = -1;
  }
  q = queue<int>(); 
  q.push(source);
  BFS_Tree[source] = true;

  while ( !q.empty() ) {
    int u = q.front();
    q.pop();

    for ( int v : adj[u] ) {
      if (blossom[u] != blossom[v] && match[u] != v){
        if ((v == source) || (match[v] != -1 && parent[match[v]] != -1))
          blossom_contraction(adj, source, u, v);
        else if (parent[v] == -1) {
          parent[v] = u;
          if (match[v] == -1) return v;
          else if (!BFS_Tree[match[v]]){
            q.push(match[v]);
            BFS_Tree[match[v]] = true;
          }
        }
      }
    }
  }
  return -1;
}

void invert_path( int u){
  while (u != -1) {
    int w = match[parent[u]];
    match[u] = parent[u];
    match[parent[u]] = u;
    u = w;
  }
}

void edmonds(vector<vector<int>> &adj){
  for (int r = 0; r < adj.size(); r++)
    if (match[r] == -1) invert_path(find_augmenting_path(adj, r));

  cout<<"Maximum Matching: "<<endl;

  for (int i = 0; i < adj.size(); i++)
    if (i < match[i]) cout << i + 1 << " " << match[i] + 1 << endl;
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
  
  int X, N, M;
  while( cin >> X >> N >> M ){
    vector<vector<int>> adj(N);

    for(int i=0;i<M;i++){
      int u, v;
      cin >> u >> v;
      adj[u - 1].push_back(v - 1);
      adj[v - 1].push_back(u - 1);
    }
    cout << "\t"<<X<<endl;
    edmonds(adj);
    for(int i=0;i<N;i++){
      q.pop();
      match[i]=-1, parent[i]-1, BFS_Tree[i]=false;
    }
  }
}
