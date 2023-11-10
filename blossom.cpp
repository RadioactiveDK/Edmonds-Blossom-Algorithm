#include<bits/stdc++.h>
using namespace std;

const int M = 50;
vector<int> match(M,-1), parent(M,-1), Representative(M);

int LCA(int root, int u, int v){
  vector<bool> vis(M,false);
  vis[u]=true;
  while (1)  {
    u = Representative[u];
    vis[u] = true;
    if (u == root) break;
    u = parent[match[u]];
  }
  while (1){
    v = Representative[v];
    if (vis[v])  return v;
    else v = parent[match[v]];
  }
}

void mark_blssm(vector<bool> &in_blssm, int lca, int u){
  while (Representative[u] != lca)  {
    int v = match[u];
    in_blssm[Representative[u]] = in_blssm[Representative[v]] = true;
    u = parent[v];
  }
}

void Contract_Blssm(vector<vector<int>> &adj, queue<int> &q, vector<bool> &BFS_Tree, int s, int u, int v){  
  vector<bool> in_blssm(adj.size(), false);
  int lca = LCA(s, u, v);
  mark_blssm(in_blssm, lca, u);
  mark_blssm(in_blssm, lca, v);
  if (Representative[u] != lca)  parent[u] = v;
  if (Representative[v] != lca)  parent[v] = u;
  
  for (int i = 0; i < adj.size(); i++)
    if (in_blssm[Representative[i]]) {
      Representative[i] = lca;
      if (!BFS_Tree[u]) {
        q.push(i);
        BFS_Tree[i] = true;
      }
    }
}

int find_augmenting_path( vector<vector<int>> &adj, int source){
  for (int i = 0; i < adj.size(); i++) {
    Representative[i] = i;
    parent[i] = -1;
  }

  queue<int> q; 
  q.push(source);

  vector<bool> BFS_Tree(adj.size(), false);
  BFS_Tree[source] = true;

  while ( !q.empty() ) {
    int u = q.front();
    q.pop();

    for ( int v : adj[u] ) {
      if (Representative[u] != Representative[v] && match[u] != v){
        if ((v == source) || (match[v] != -1 && parent[match[v]] != -1))
          Contract_Blssm(adj, q, BFS_Tree, source, u, v);
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
  
  for(int i=0;i<adj.size();i++){
    match[i]=-1;
    parent[i]=-1;
  }

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
  }
}
