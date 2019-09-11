class Solution {
public:
    int minMoves(vector<int>& nums) {
        int sum = 0, min_num = INT_MAX;
        for(x : nums)
        {
            sum += x;
            if(x  < min_num)
                min_num = x;
        }
        return sum - min_num * nums.size();
    }
};