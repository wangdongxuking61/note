http://www.cnblogs.com/chenying99/p/3932877.html
##### 在动态规划算法中，处于首要位置、且也是核心理念之一的就是状态的定义。在这里，把d[k][i][j]定义成：

```
只能使用第1号到第k号点作为中间媒介时，点i到点j之间的最短路径长度
d[k][i][j] = min(d[k-1][i][j], d[k-1][i][k]+d[k-1][k][j])（k,i,j∈[1,n]）
```