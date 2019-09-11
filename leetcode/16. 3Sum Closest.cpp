// sort排序复杂度O(nlogn)
// 从头向后遍历nums（重复的数掠过），设当前位置变量是target，使用twoSum函数从当前位置到末尾，找两个数相加最接近-target
// 由于nums已经排序，twoSum使用两个指针前后夹逼，复杂度O(n)
// 总的复杂度O(n^2)
class Solution {
public:
    void twoSum(vector<int>& nums, int start, int target, int& dis)
    {
        int i = start, j = (int)nums.size() - 1;
        int sum;
        while (i < j)
        {
            sum = nums[i] + nums[j];
            if(abs(dis) > abs(target - sum))
                dis = target - sum;
            if(sum > target)
                j--;
            else if(sum < target)
                i++;
            else
            {
                dis = 0;
                return ;
            }
        }
    }

    int threeSumClosest(vector<int>& nums, int target) {
        int dis = INT_MAX;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size() - 2; ++i)
        {
            if(i > 0 && nums[i] == nums[i-1])
                continue;
            twoSum(nums, i + 1, target - nums[i], dis);
            if(dis == 0)
                break;
        }
        return target - dis;
    }
};