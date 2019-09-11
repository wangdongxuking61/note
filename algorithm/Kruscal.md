```CPP
#include"stdio.h"
#define N 9
int E[N][N]={{0,4,0,0,0,0,0,8,0},
             {4,0,8,0,0,0,0,11,0},
             {0,8,0,7,0,4,0,0,2},
             {0,0,7,0,9,14,0,0,0},
             {0,0,0,9,0,10,0,0,0},
             {0,0,4,14,10,0,2,0,0},
             {0,0,0,0,0,2,0,1,6},
             {0,0,0,0,0,0,1,0,7},
             {0,0,2,0,0,0,6,7,0}
};
void Qsort(int a[36][3], int low, int high)
{
    if(low >= high)
        return;
    int first = low;
    int last = high;
    int key = a[first][2];/*用字表的第一个记录作为枢轴*/
    int tmpi = a[first][0];
    int tmpj = a[first][1];
    while(first < last)
    {
        while(first < last && a[last][2] >= key)
            --last;
        a[first][2] = a[last][2];/*将比第一个小的移到低端*/
        a[first][0] = a[last][0];
        a[first][1] = a[last][1];

        while(first < last && a[first][2] <= key)
            ++first;
        a[last][2] = a[first][2];/*将比第一个大的移到高端*/
        a[last][0] = a[first][0];
        a[last][1] = a[first][1];
    }
    a[first][2] = key;/*枢轴记录到位*/
    a[first][0] = tmpi;
    a[first][1] = tmpj;
    Qsort(a, low, first-1);
    Qsort(a, first+1, high);
}

int main()
{
    //init A:最小生成树，source,dest,weight
    int A[N-1][3],Acnt=0;
    //init Set:相当于MAKE-SET点所处的不同集合
    int Set[N];
    for(int i=0;i<N;i++)
        Set[i] = i;
    //get weight：数据结构的转化，从邻接表到一维数组，方便排序
    int edges[N*(N-1)/2][3];
    int eNum = 0;
    for(int i=0;i<N;i++)
        for(int j=i+1;j<N;j++)
            if(E[i][j]!=0)
            {
                edges[eNum][0] = i;
                edges[eNum][1] = j;
                edges[eNum][2] = E[i][j];
                eNum++;
            }
    //sort:给edges排序，这里原文第三个参数要减1否则内存越界
    Qsort(edges, 0, eNum - 1);

    //loop:select edge to A
    for(int i=0;i<eNum;i++)
    {
        if(Set[edges[i][0]] != Set[edges[i][1]])
        {
            //add edge to A
            A[Acnt][0] = edges[i][0];
            A[Acnt][1] = edges[i][1];
            A[Acnt][2] = edges[i][2];
            printf("%c %c %2d\n",A[Acnt][0]+'a',A[Acnt][1]+'a',A[Acnt][2]);
            Acnt++;
            //union(u,v)
            int tmp = Set[edges[i][1]];
            for(int j=0;j<N;j++)
                if(Set[j] == tmp)
                    Set[j] = Set[edges[i][0]];
        }
    }
}
```