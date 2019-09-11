```
// time O(nv)	space O(nv)
int dp[51][v];
for (int i = 1; i <= n; ++i)
    for (int j = v; j >= 1; --j)
    	if (j - c[i] >= 0)
    	    dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - c[i]] + w[i]);
    	else
            dp[i][j] = dp[i - 1][j];

// time O(nv)	space O(v)
int dp[v];
for (int i = 1; i <= n; ++i)
    for (int j = v; j >= c[i]; --j)
    	dp[j] = max(dp[j], dp[j - c[i]] + w[i]);
```