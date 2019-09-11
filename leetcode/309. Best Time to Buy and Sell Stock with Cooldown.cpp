// 动态规划简单题，每天有三个状态{no, have, cool}
// 转移方程如下：
// maxProfit[i][no] = max(maxProfit[i - 1][no], maxProfit[i - 1][cool]);
// maxProfit[i][have] = max(maxProfit[i - 1][have], maxProfit[i - 1][no] - prices[i]);
// maxProfit[i][cool] = maxProfit[i - 1][have] + prices[i];

class Solution {
public:
	int maxProfit(vector<int>& prices)
	{
		if (prices.empty())
			return 0;

		const int N = 10000;
		enum { no, have, cool };
		int maxProfit[N][3];

		maxProfit[0][no] = 0;
		maxProfit[0][cool] = 0;
		maxProfit[0][have] = -prices[0];
		for (int i = 1; i < prices.size(); i++)
		{
			maxProfit[i][no] = max(maxProfit[i - 1][no], maxProfit[i - 1][cool]);
			maxProfit[i][have] = max(maxProfit[i - 1][have], maxProfit[i - 1][no] - prices[i]);
			maxProfit[i][cool] = maxProfit[i - 1][have] + prices[i];
		}
		int n = prices.size();
		return max(maxProfit[n - 1][cool], max(maxProfit[n - 1][no], maxProfit[n - 1][have]));
	}
};
