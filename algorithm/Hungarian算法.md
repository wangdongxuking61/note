`不带权匹配`
```
二分图的最大匹配、完美匹配
https://www.renfei.org/blog/bipartite-matching.html
```

`不带权匹配BFS模板`
```
//poj 1274
#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

#define MAXN 201            //MAXN 为表示 X 集合和 Y 集合顶点个数最大值的符号常量
int nx, ny;                 //X 和 Y 集合中顶点的个数
int g[MAXN][MAXN];          //邻接矩阵，g[i][j]为 1 表示 Xi 和 Yj 有边相连
int cx[MAXN], cy[MAXN];     //cx[i]表示最终求得的最大匹配中与 Xi 匹配的 Y 顶点, cy[i]同理
int mk[MAXN];               //DFS 算法中记录顶点访问状态的数组，mk[i] = 0表示未访问过，为1表示访问过
int pred[MAXN];

int BFS(int u)            //从X集合中的顶点u出发，用广度优先的策略寻找增广路，(这种增广路只能使当前的匹配数增加 1)
{
    queue<int> Q;
    Q.push(u);
    memset(pred, 0xff, sizeof(pred));
    while(!Q.empty())
    {
        int w = Q.front();
        Q.pop();
        for (int v = 1; v <= ny; ++v)
        {
            if (g[w][v] && !mk[v]) // v 与 w 邻接，且没有访问过
            {
                mk[v] = 1;      //访问v
                pred[v] = w;
                if (cy[v] == -1) //这里要处理好
                {
                    int tmpw = w, tmpv = v;
                    while(1)
                    {
                        cy[tmpv] = pred[tmpv];
                        int lastv = cx[tmpw];
                        cx[pred[tmpv]] = tmpv;
                        if(lastv == -1) //此时tmpw无匹配，是增广路起点u
                            break;
                        tmpv = lastv;
                        tmpw = pred[tmpv];
                    }
                    return 1;   //找到可增广路
                }
                else
                    Q.push(cy[v]);
            }
        }
    }
    return 0 ;  //如果不存在从 u 出发的增广路
}

int MaxMatch()  //求二部图最大匹配的匈牙利算法
{
    int res = 0; //所求得的最大匹配
    memset(cx, 0xff, sizeof(cx));   //从1匹配开始增广, 将cx和cy各元素初始化为-1
    memset(cy, 0xff, sizeof(cy));
    for(int i = 1; i <= nx; i++)
    {
        if(cx[i] == -1) //从每个未盖点出发进行寻找增广路
        {
            memset(mk, 0, sizeof(mk));
            res += BFS(i); //每找到一条增广路，可使得匹配数加 1
        }
    }
    return res;
}

int main()
{
    while(cin >> nx >> ny)
    {
        memset(g, 0x00, sizeof(g));
        for (int i = 1; i <= nx; ++i)
        {
            int num, stall;
            cin >> num;
            for (int j = 0; j < num; ++j)
            {
                cin >> stall;
                g[i][stall] = 1;
            }
        }
        cout << MaxMatch() << endl;
    }
    return 0;
}
```

`不带权匹配DFS模板`
```
//poj 1274
#include <iostream>
#include <cstring>
#include <stdio.h>
using namespace std;

#define MAXN 201            //MAXN 为表示 X 集合和 Y 集合顶点个数最大值的符号常量
int nx, ny;                 //X 和 Y 集合中顶点的个数
int g[MAXN][MAXN];          //邻接矩阵，g[i][j]为 1 表示 Xi 和 Yj 有边相连
int cx[MAXN], cy[MAXN];     //cx[i]表示最终求得的最大匹配中与 Xi 匹配的 Y 顶点, cy[i]同理
int mk[MAXN];               //DFS 算法中记录顶点访问状态的数组，mk[i] = 0表示未访问过，为1表示访问过

int path(int u)            //从X集合中的顶点u出发，用深度优先的策略寻找增广路，(这种增广路只能使当前的匹配数增加 1)
{
    for(int v = 1; v <= ny ; v++)//考虑所有 Yi 顶点 v
    {
        if(g[u][v] && !mk[v]) // v 与 u 邻接，且没有访问过
        {
            mk[v] = 1;      //访问v
            //如果 v 没有匹配，或者 v 已经匹配了，但从 cy[v] 出发可以找到一条增广路
            if(cy[v] == -1 || path(cy[v])) //注意如果前一个条件成立，则不会递归调用
            {
                cx[u] = v; //把 v 匹配给 u
                cy[v] = u; //把 u 匹配给 v
                return 1;  //找到可增广路
            }
        }
    }
    return 0 ;  //如果不存在从 u 出发的增广路
}

int MaxMatch()  //求二部图最大匹配的匈牙利算法
{
    int res = 0; //所求得的最大匹配
    memset(cx, 0xff, sizeof(cx)) ;   //从1匹配开始增广, 将cx和cy各元素初始化为-1
    memset(cy, 0xff, sizeof(cy)) ;
    for(int i = 1; i <= nx; i++)
    {
        if(cx[i] == -1) //从每个未盖点出发进行寻找增广路
        {
            memset(mk, 0 ,sizeof(mk) ) ;
            res += path(i); //每找到一条增广路，可使得匹配数加 1
        }
    }
    return res;
}

int main()
{
    while(cin >> nx >> ny)
    {
        memset(g, 0x00, sizeof(g));
        for (int i = 1; i <= nx; ++i)
        {
            int num, stall;
            cin >> num;
            for (int j = 0; j < num; ++j)
            {
                cin >> stall;
                g[i][stall] = 1;
            }
        }
        cout << MaxMatch() << endl;
    }
    return 0;
}
```
`带权匹配`
```
`第一步:变换指派问题的系数矩阵(cij)为(bij)，使 在(bij)的各行各列中都出现0元素，即`
(1) 从(cij)的每行元素都减去该行的最小元素;
(2) 再从所得新系数矩阵的每列元素中减去该列的最 小元素。

`第二步:进行试指派，以寻求最优解。在(bij)中找尽可能多的独立0元素，若能找出n个独立0元素，就以这n个独立0元素对应解矩阵(xij)中的元 素为1，其余为0，这就得到最优解`。
找独立0元素，常用的步骤为:
(1)从只有一个0元素的行(列)开始，给这个0元素加圈，记作◎。然后划去◎所在列(行)的其它0元素，记作Ø ;这表示这列所代表的任务已指派完，不必再考虑别人了。
(2)给只有一个0元素的列(行)中的0元素加圈，记作 ◎;然后划去◎ 所在行的0元素，记作Ø.
(3)反复进行(1)，(2)两步，直到尽可能多的0元素都 被圈出和划掉为止。
(4)若仍有没有划圈的0元素，且同行(列)的0元素至 少有两个，则从剩有0元素最少的行(列)开始，比较这 行各0元素所在列中0元素的数目，选择0元素少的那列 的这个0元素加圈(表示选择性多的要“礼让”选择性少 的)。然后划掉同行同列的其它0元素。可反复进行，直到所有0元素都已圈出和划掉为止。
(5)若◎ 元素的数目m 等于矩阵的阶数n，那么这指 派问题的最优解已得到。若m < n, 则转入下一步。

`第三步:作最少的直线覆盖所有0元素。 `
(1)对没有◎的行打√号; 
(2)对已打√号的行中所有含Ø元素的列打√号; 
(3)再对打有√号的列中含◎ 元素的行打√号;
(4)重复(2)，(3)直到得不出新的打√号的行、列为止;
(5)对没有打√号的行画横线，有打√号的列画纵线， 这就得到覆盖所有0元素的最少直线数 l 。l 应等于m， 若不相等，说明试指派过程有误，回到第二步(4)，另 行试指派;若 l=m < n，须再变换当前的系数矩阵， 以找到n个独立的0元素，为此转第四步。

`第四步:变换矩阵(bij)以增加0元素。`
在没有被直线覆盖的所有元素中找出最小元素，然后打√各行都减去这最小元素;打√各列都加上这最小元 素(以保证系数矩阵中不出现负元素)。新系数矩阵的最优解和原问题仍相同。转回第二步。
```