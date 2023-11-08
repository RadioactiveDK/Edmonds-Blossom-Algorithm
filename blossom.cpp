#include<iostream>
//for memset
#include <cstring> 


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

//matrix to check if the vertex is in matched edge or not
int mate[M];

int V, E, queue_head, queue_tail, queue[M], parent[M];

//to check which "blossom" does a vertex belongs to
int blossom[M];
//the bfs traversal tree
bool BFS_Tree[M];

bool in_blossom[M], Edge[M][M];
void add_edge(int u, int v)
{
  top->v = v, top->n = adj[u], adj[u] = top++;
  top->v = u, top->n = adj[v], adj[v] = top++;
}
int LCA(int root, int u, int v)
{
  //to keep track of visited vertex within blossom
  static bool inp[M];
  memset(inp, 0, sizeof(inp));
  while (1)
  {
    //represntive vertex will be u for blossom[u]
    inp[u = blossom[u]] = true;
    //if u reaches the root then break the loop
    if (u == root)
      break;
    //now let u be the parent of mate of u
    u = parent[mate[u]];
  }
  while (1)
  {
    //when we encounter a vertex v for which inp[v] is already true
    //then v and u share a common vertex, with v as the common ancestor
    if (inp[v = blossom[v]])
      return v;
    else
      v = parent[mate[v]];
  }
}
//mark all vertices within the blossom
void mark_blossom(int lca, int u)
{
  while (blossom[u] != lca)
  {
    int v = mate[u];
    //mark the same blossom to true
    //blossom of u and blossom of v are true
    in_blossom[blossom[u]] = in_blossom[blossom[v]] = true;
    //moving up the tree
    u = parent[v];
    // maintain the tree structure
    if (blossom[u] != lca)
      parent[u] = v;
  }
}
void blossom_contraction(int s, int u, int v)
{
  
  int lca = LCA(s, u, v);
  memset(in_blossom, 0, sizeof(in_blossom));
  //makr u and v as part of the same blossom
  mark_blossom(lca, u);
  mark_blossom(lca, v);
  if (blossom[u] != lca)
    parent[u] = v;
  if (blossom[v] != lca)
    parent[v] = u;
  for (int u = 0; u < V; u++)
    if (in_blossom[blossom[u]])
    {
      //mark the representative vertex as the lca
      blossom[u] = lca;
      //mark the vertex as part of the BFS Traversal and add to queue
      if (!BFS_Tree[u])
      {
        BFS_Tree[queue[++queue_tail] = u] = true;
      }
    }
}
//find an augmenting path from source vertex
int find_augmenting_path(int source)
{
    memset(BFS_Tree, 0, sizeof(BFS_Tree));
    memset(parent, -1, sizeof(parent));
    for (int i = 0; i < V; i++)
    {
      blossom[i] = i;
    }
    queue_head = 0;
    queue_tail = 0;
    BFS_Tree[queue[queue_head] = source] = true;
    //while q is not empty
    while (queue_head <= queue_tail)
    {
      int u = queue[queue_head++];
      //traverse neighbours
      for (edge e = adj[u]; e; e = e->n)
      {
        int v = e->v;
        //check if the other vertex in queue is part of the same blossom
        //and they are not matched with each other
        if (blossom[u] != blossom[v] && mate[u] != v)
        {
          //checks if v is the source vertex
          //or if v is not matched with other vertex basically a free vertex

          if ((v == source) || (mate[v] != -1 && parent[mate[v]] != -1))
          {
            blossom_contraction(source, u, v);
          }
          //if the node hasn't been discovered  through bfs
          else if (parent[v] == -1)
          {
              //set the parent of v to u
              parent[v] = u;
              //if v is not part of matching return it
              if (mate[v] == -1)
              {
                  return v;
              }
              //v is matched with another vertex
              //checks if match[v] is processed in BFS traversal or not
              //if it is not processed 
              //then add the matched vertex at the end of the queue
              //and mark the current traversal to true
              else if (!BFS_Tree[mate[v]])
              {
                BFS_Tree[queue[++queue_tail] = mate[v]] = true;
              }
          }

        }
      }
  }
  return -1;
}
//the augment path
int augment_path(int s, int t)
{
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
int edmonds()
{
    int max_matching = 0;

    memset(mate, -1, sizeof(mate));
    for (int u = 0; u < V; u++)
    {
        //check for a free vertex
        if (mate[u] == -1)
        {
            max_matching += augment_path(u, find_augmenting_path(u));
        }
    }
  return max_matching;
}
int main()
{
    int u, v;
    cout<<"Enter the number of Vertices\n";
    cin >> V;
    cout<<"Enter the number of Edges\n";
    cin>>E;
    while (E--)
    {
        cout<<"Enter u and v(edge between nodes, not 0 based)\n";
        cin >> u >> v;
        if (!Edge[u - 1][v - 1])
        {
            add_edge(u - 1, v - 1);
            Edge[u - 1][v - 1] = Edge[v - 1][u - 1] = true;
        }
    }
  cout << edmonds() << endl;
  for (int i = 0; i < V; i++)
    if (i < mate[i])
    {
      cout<<"Max matching path"<<endl;
      cout << i + 1 << " " << mate[i] + 1 << endl;
    }
}
