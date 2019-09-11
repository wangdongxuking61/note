// 使用map，用空间换时间，复杂度O(n)
// map->first 是元素值
// map->second 是地址
class Solution
{
public:
    vector<int> twoSum(vector<int>& nums, int target)
    {
        vector<int> answer;
        unordered_map<int, int> m;
        for(int i = 0; i < nums.size(); i++)
        {
            auto it = m.find(target - nums[i]);
            if(it != m.end())
            {
                answer.push_back(it->second);
                answer.push_back(i);
                break;
            }
            else
            {
                m[nums[i]] = i;
            }
        }
        return answer;
    }
};