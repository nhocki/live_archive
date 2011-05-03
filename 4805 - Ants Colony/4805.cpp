#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <cstdlib>

using namespace std;

int c, l, h, n;
pair<int, pair<int, int> > edges[100000];
int p[100001], r[100001];
vector<int> E[100001], W[100001];
int pai[100001], inf = 1 << 30, dist[100001], pp[100001][30];
unsigned long long D[100001];
int T[100001]; 

// Get the parent and the level of node v
void dfs(int v, int p, int f){
  pai[v] = f;
  dist[v] = p;
  for(int i = 0; i < E[v].size(); i++){
    if(E[v][i] == f) continue;
    dfs(E[v][i], p + 1, v);
  }
}

// Build the parents table.
// pp[i][j] is the 2^j ancestor of the node i
void pre(){
    memset(pai, -1, sizeof pai);
    for(int i = 0; i < n; i++) for(int j = 0; (1 << j) < n; j++){
        pp[i][j] = -1;
      }
    dfs(0, 0, -1);
    pai[0] = 0;
    
    for(int i = 0; i < n; i++) pp[i][0] = pai[i];
    
    for(int j = 1; (1 << j) < n; j++){
        for(int i = 0; i < n; i++){
            if(pp[i][j - 1] == -1) continue;
            pp[i][j] = pp[pp[i][j - 1]][j - 1];
        }
    }
}


int resolve(int a, int b){
  // dist[i] is the level of node i
  if(dist[a] < dist[b]) swap(a, b);
  int log;

  // simply find the log(2) of dist[a]
  for(log = 1; (1 << log) <= dist[a]; log++);
  log--;

  // If the distance of a is != than the distance of b
  // find the ancestor of a that's in the same level of b
  for(int i = log; i >= 0; i--)
    if(dist[a] - (1 << i) >= dist[b]){
      a = pp[a][i];
    }
  
  // if a was an ancestor of b, return a!
  // because a is obviously the LCA
  if(a == b)
    return a;
  
  // While the ancestors are different, look up for the ancestors
  for(int i = log; i >= 0; i--){
    if(pp[a][i] != -1 && pp[a][i] != pp[b][i]){
      a = pp[a][i]; b = pp[b][i];
    }
  }
  return pai[a];
}


int create_edge(int from, int to, int w){
  W[from].push_back(w);
  E[from].push_back(to);

  W[to].push_back(w);
  E[to].push_back(from);
  return 0;
}

int main(){
  while(scanf("%d", &n)==1 && n){
    W[0].clear();
    E[0].clear();
    for(int i=1;i<n;++i){
      W[i].clear();
      E[i].clear();
      int to, w;
      scanf("%d%d", &to, &w);
      create_edge(i, to, w);
      T[i] = to;
    }
    T[0] = 0;
    D[0] = 0;
    for(int i=1;i<n;++i){
      D[i] = D[T[i]] + W[i][0];
    }
    
    pre();
    int q, _a, _b;
    scanf("%d", &q);
    scanf("%d%d", &_a, &_b);
    printf("%lld", D[_a] + D[_b] - 2*D[resolve(_a, _b)]);
    q--;
    while(q--){
      scanf("%d%d", &_a, &_b);
      printf(" %lld", D[_a] + D[_b] - 2*D[resolve(_a, _b)]);
    }
    puts("");
  }
  return 0;
}
