```
//poj 2195
#include "cstring"
#include "iostream"
#include "stdlib.h"
using namespace std;
#define maxn 100
#define INF 0x7fff

int mp[maxn][maxn], match[maxn], ly[maxn], lx[maxn], slack[maxn];
int n;
bool visx[maxn], visy[maxn];

bool dfs(int x)
{
    visx[x] = true;
    for(int y = 0; y < n; y++)
    {
        if(!visy[y])
        {
            int t = lx[x] + ly[y] - mp[x][y];
            if(t == 0)
            {
                visy[y] = true;
                int tmp = match[y];
                match[y] = x;
                if(tmp == -1 || dfs(tmp)) return true;
                match[y] = tmp;
            }
            else if(t < slack[y])
                slack[y] = t;
        }
    }
    return false;
}

int km()
{
    memset(match, 0xff, sizeof(match));
    memset(ly, 0, sizeof(ly));
    memset(lx, 0, sizeof(lx));
    for(int i = 0;i < n;i ++)
        for(int j = 0;j < n;j ++)
            if(mp[i][j] > lx[i])
                lx[i] = mp[i][j];
    for(int i = 0;i < n;i ++)
    {
        for(int j = 0;j < n;j ++)
            slack[j] = INF;
        while(1)
        {
            int d = INF;
            memset(visx,false,sizeof(visx));
            memset(visy,false,sizeof(visy));
            if(dfs(i)) break;
            for(int j = 0;j < n;j ++)
                if(!visy[j] && slack[j] < d)
                    d = slack[j];
            for(int j = 0 ;j < n;j ++)
                if(visx[j])
                    lx[j] -=d;
            for(int j = 0;j < n;j ++)
            {
                if(visy[j]) ly[j] +=d;
                else slack[j] -= d;
            }
        }
    }
    int ret = 0;
    for(int i = 0;i < n;i ++)
        ret += mp[match[i]][i];
    return ret;
}

int N, M;
int hcn, mcn, hx[maxn], hy[maxn], mx[maxn], my[maxn];
char ch;

int main()
{
    while (cin >> N >> M)
    {
        if (N == 0 && M == 0) break;
        hcn = mcn = 0;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < M; ++j)
            {
                cin >> ch;
                if (ch == '.') continue;
                else if (ch == 'm')
                {
                    mx[mcn] = i;
                    my[mcn] = j;
                    mcn++;
                }
                else if (ch == 'H')
                {
                    hx[hcn] = i;
                    hy[hcn] = j;
                    hcn++;
                }
            }
        }
        n = hcn;

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                mp[i][j] = -abs(mx[i] - hx[j]) - abs(my[i] - hy[j]);

        cout << - km() << endl;
    }
}
```