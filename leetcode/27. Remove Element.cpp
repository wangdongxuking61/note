class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if(nums.size()==0)  return 0;
        int front = 0, back = (int)nums.size() - 1, tmp;
        while(front < back)
        {
            if(nums[front] != val)
                front++;
            else
            {
                while(front < back && nums[back] == val)    back--;
                if(front < back)
                {
                    nums[front] = nums[back];
                    front++, back--;
                }
            }
        }
        return nums[back]==val ? back : back+1;
    }
};