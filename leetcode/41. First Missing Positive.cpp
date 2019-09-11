// 要求时间复杂度是O(n)，空间复杂度是O(1)
// 所以要在原来的数组上进行修改，最多swap次数是nums.size()
// 几个特殊情况：0个元素，1个元素，多个元素找完了看nums[0]
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        for(int i = 0; i < nums.size();)
        {
            if(nums[i] >= 0 && nums[i] < nums.size() && nums[i] != nums[nums[i]])
                swap(nums[i], nums[nums[i]]);
            else
                i++;
        }
        for(int i = 1; i < nums.size(); i++)
            if(i != nums[i])
                return i;
        if(nums.size() == 0)
            return 1;
        else if(nums.size() == 1)
            return nums[0] == 1 ? 2 : 1;
        else
            return nums.size()==nums[0] ? nums.size() + 1 : nums.size();
    }
};