Loading [Contrib]/a11y/accessibility-menu.js
```
https://www.renfei.org/blog/weighted-shortest-path.html
```
# `它们的使用限制和运行时间如下：`
- `Dijkstra`和`Prim`区别：都是不断扩充，前者扩充条件是距离某个点，后者是距离当前MST
- `Dijkstra`: 不含负权。运行时间依赖于优先队列的实现，如 $O\left(\left(\mid V\mid+\mid E\mid\right)\log\mid V\mid\right)$
- `SPFA`: 无限制。运行时间$O\left(k\cdot\mid E\mid\right)\ \left(k \ll \mid V\mid\right)$
- `Bellman-Ford`：无限制。运行时间$O\left(\mid V\mid\cdot\mid E\mid\right)$
- `ASP`: 无圈。运行时间$O\left(\mid V\mid+\mid E\mid\right)$
- `Floyd-Warshall`: 无限制。运行时间$O\left(\mid V\mid^3\right)$

# `本文主要介绍的算法的代码主要来源如下：`
- `Dijkstra`: <em>Algorithms</em>（《算法概论》）Sanjoy Dasgupta, Christos Papadimitriou, Umesh Vazirani;《算法竞赛入门经典—训练指南》刘汝佳、陈峰。
- `SPFA` (Shortest Path Faster Algorithm): <em>Data Structure and Algorithms Analysis in C</em>, 2nd ed.（《数据结构与算法分析》）Mark Allen Weiss.
- `Bellman-Ford`: <em>Algorithms</em>（《算法概论》）Sanjoy Dasgupta, Christos Papadimitriou, Umesh Vazirani.
- `ASP` (Acyclic Shortest Paths): <em>Introduction to Algorithms - A Creative Approach</em>（《算法引论—一种创造性方法》）Udi Manber.
- `Floyd-Warshall`: <em>Data Structure and Algorithms Analysis in C</em>, 2nd ed.（《数据结构与算法分析》）Mark Allen Weiss.
- 其中 1~4 均为单源最短路径 (Single Source Shortest Paths) 算法； 5 为全源最短路径 (All Pairs Shortest Paths) 算法。顺便说一句，为什么没有点对点的最短路径？如果我们只需要一个起点和一个终点，不是比计算一个起点任意终点更节省时间么？答案还真不是，目前还没有发现比算从源点到所有点更快的算法。

***

# `1. Dijkstra`
```
typedef pair<int, int> HeapNode;
void Dijkstra(int s)
{
    priority_queue< HeapNode, vector<HeapNode>, greater<HeapNode> > Q;
    for (int i=0; i<num_nodes; ++i)
        d[i] = __inf;

    d[s] = 0;
    Q.push(make_pair(0, s));
    while (!Q.empty()) {
        pair<int, int> N = Q.top();
        Q.pop();
        int u = N.second;
        //if (N.first != d[u]) continue;
        for (int i=0; i<G[u].size(); ++i) {
            Edge &e = edges[G[u][i]];
            if (d[e.to] > d[u] + e.weight) {
                d[e.to] = d[u] + e.weight;
                p[e.to] = G[u][i];
                Q.push(make_pair(d[e.to], e.to));
            }
        }
    }
}

//伪代码:Dijkstra’s shortest-path algorithm.
procedure dijkstra(G,l,s)
Input: Graph G = (V,E), directed or undirected;
       positive edge lengths {le : e ∈ E};
       vertex s ∈ V
Output: For all vertices u reachable from s, dist(u) is set to the distance from s to u.

for all u ∈ V :
    dist(u) = ∞ 
    prev(u) = nil
dist(s) = 0

H = makequeue(V) (using dist-values as keys)
while H is not empty:
    u = deletemin(H)
    for all edges (u,v) ∈ E:
    if dist(v) > dist(u) + l(u,v):
        dist(v) = dist(u) + l(u,v)
        prev(v) = u
        decreasekey(H,v)

//伪代码:好理解的形式
Initialize dist(s) to 0, other dist(·) values to ∞
R = { }(the ''known region'')
while R ≠ V :
    Pick the node ∉ R with smallest dist(·)
    Add v to R
    for all edges (v,z) ∈ E:
        if dist(z) > dist(v) + l(v,z):
            dist(z) = dist(v) + l(v,z)
```

# `2. Bellman-Ford`
```
//伪代码
dist(s) = 0
repeat |V|-1 times:
    for all e ∈ E:
        update(e)

bool Bellman_Ford(int s)
{
    for (int i=0; i<num_nodes; ++i)
        d[i] = __inf;

    d[s] = 0;
    for (int i=0; i<num_nodes; ++i) {
        bool changed = false;
        for (int e=0; e<num_edges; ++e) {
            if (d[edges[e].to] > d[edges[e].from] + edges[e].weight 
               && d[edges[e].from] != __inf) {
                d[edges[e].to] = d[edges[e].from] + edges[e].weight;
                p[edges[e].to] = e;
                changed = true;
            }
        }
        if (!changed) return true;
        if (i == num_nodes && changed) return false;
    }
    return false; // 程序应该永远不会执行到这里
}
```