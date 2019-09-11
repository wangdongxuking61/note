```
int dir[4][2] = {1, 0, -1, 0, 0, 1, 0, -1}; // 方向向量   右，左，上，下

const int N = 51;
int r, c, label[N][N] = {{0}}, grid[N][N];

int bfs_2dim(int i, int j, int lab)
{
    int num = 0;
    deque<pair<int, int> > q;
    q.PB(MP(i, j));

    while (!q.empty())
    {
        auto p = q.front();
        q.pop_front();
        label[p.first][p.second] = lab;
        num++;

        for (int k = 0, x, y; k < 4; ++k)
        {
            x = p.first + dir[k][0];
            y = p.second + dir[k][1];
            if (x < 0 || x >= r || y < 0 || y >= c)
                continue;
            if (grid[x][y] == 0 && label[x][y] == 0)// grid合格并且没有被标志
                q.PB(MP(x, y));
        }
    }
    return num;
}


int min_trees()
{
    int lab = 1;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (gridd[i][j] == 0 && label[i][j] == 0)
                int num = bfs_2dim(i, j, lab++);

    return sum;
}
```