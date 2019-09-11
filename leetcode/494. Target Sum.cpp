const int maxn = 20;
const int maxm = 4002;
#define ofs 2000
int d[maxn][maxm];

class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int S) {
        if(S > 1000 || S < -1000)   return 0;
        memset(d, 0x00, sizeof(d));
        d[0][nums[0] + ofs] = d[0][-nums[0] + ofs] = 1;
        for (int i = 1; i < nums.size(); i++) {
            for (int j = -1000; j <= 1000; j++) {
                d[i][j + ofs] = d[i - 1][j - nums[i] + ofs] + d[i - 1][j + nums[i] + ofs];    
            }
        }
        if(nums[0] == 0)
            return d[nums.size() - 1][S + ofs] * 2;
        else
            return d[nums.size() - 1][S + ofs];
    }
};