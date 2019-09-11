class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int ans = 0;
        int m = grid.size();
        int n = grid[0].size();
        bool findland = false;
        for(int i = 0; i < m; i++)
        {
            bool lastline = true;
            for(int j = 0; j < n; j++)
            {
                if(grid[i][j] == 1)
                {
                    findland = true;
                    lastline = false;
                    if(i == 0 || grid[i-1][j] == 0)     ans++;
                    if(i == m-1 || grid[i+1][j] == 0)   ans++;
                    if(j == 0 || grid[i][j-1] == 0)     ans++;
                    if(j == n-1 || grid[i][j+1] == 0)   {ans++; break;};
                }
            }
        }
        return ans;
    }
};