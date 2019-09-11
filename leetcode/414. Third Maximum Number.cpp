class Solution {
public:
    int thirdMax(vector<int>& nums) {
        long ans[3] = {LONG_MIN, LONG_MIN, LONG_MIN}; //ans[0] is the third maximum number
        for(int i = 0; i < nums.size(); i++)
        {
            if(nums[i] < ans[0]);
            else if(nums[i] > ans[0])
            {
                if(nums[i] < ans[1])
                   ans[0] = nums[i];
                else if(nums[i] > ans[1])
                {
                    if(nums[i] < ans[2])
                    {
                        ans[0] = ans[1];
                        ans[1] = nums[i];
                    }
                    else if(nums[i] > ans[2])
                    {
                        ans[0] = ans[1];
                        ans[1] = ans[2];
                        ans[2] = nums[i];
                    }
                }
            }
        }
        if(ans[0] != LONG_MIN)
            return ans[0];
        else
            return ans[2];
    }
};