// sort排序复杂度O(nlogn)
// 从头向后遍历nums（重复的数掠过），设当前位置变量是target，使用twoSum函数从当前位置到末尾，找两个数相加是-target（可能多组）
// 由于nums已经排序，twoSum使用两个指针前后夹逼，复杂度O(n)，如果没有排序可以使用map做
// 总的复杂度O(n^2)
class Solution {
public:
    void twoSum(vector<int>& nums, int start, int target, vector<vector<int>>& ans)
    {
        int i = start, j = (int)nums.size() - 1;
        int sum;
        while (i < j)
        {
            sum = nums[i] + nums[j];
            if(sum > target)
                j--;
            else if(sum < target)
                i++;
            else
            {
                if(ans.size() == 0 || !(ans[ans.size() - 1][0] == -target && ans[ans.size() - 1][1] == nums[i]))
                    ans.push_back({-target, nums[i], nums[j]});
                i++, j--;
            }
        }
    }

    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ans;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i)
        {
            if(i > 0 && nums[i] == nums[i-1])
                continue;
            twoSum(nums, i + 1, -nums[i], ans);
        }
        return ans;
    }
};