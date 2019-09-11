class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int p0 = 0, p1 = 0;
        while(1)
        {
            while(p0 < nums.size() && nums[p0])  p0++;
            if(p0 == nums.size()) break;
            if(p1 <= p0)    p1 = p0;
            while(p1 < nums.size() && !nums[p1])  p1++;
            if(p1 == nums.size()) break;
            
            nums[p0] = nums[p1];
            nums[p1] = 0;
        }
    }
};