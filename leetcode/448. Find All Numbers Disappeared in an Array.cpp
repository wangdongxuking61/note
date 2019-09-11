class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int i = 0;
        while(i < nums.size())
        {
            if(nums[i] == i+1 || nums[i] == nums[nums[i]-1])
            {
                i++;
                continue;
            }
            else
            {
                int tmp = nums[nums[i]-1];
                nums[nums[i]-1] = nums[i];
                nums[i] = tmp;
            }
        }
        vector<int> ans;
        for(i = 0; i < nums.size(); i++)
            if(nums[i] != i+1)
                ans.push_back(i+1);
        return ans;
    }
};