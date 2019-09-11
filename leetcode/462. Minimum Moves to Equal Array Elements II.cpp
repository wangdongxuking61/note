class Solution {
public:
    int minMoves2(vector<int>& nums) {
        long sum = 0, front_sum = 0;
        sort(nums.begin(), nums.end());
        for(int x : nums)
            sum += x;
        int median = nums.size()>>1u;
        for(int i = 0; i < median; i++)
            front_sum += nums[i];
        return median*nums[median]-front_sum + sum-front_sum-(nums.size()-median)*nums[median];
    }
};