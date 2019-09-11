class Solution {
public:
    int findMin(vector<int>& nums) {
        if(nums[0] <= nums[(int)nums.size() - 1])
            return nums[0];
        int low = 0, high = (int)nums.size() - 1, pivot = 0;
        while(low < high - 1)
        {
            pivot = (low + high) >> 1u;
            if (nums[pivot] >= nums[low])
                low = pivot;
            else
                high = pivot;
        }
        return nums[high];
    }
};