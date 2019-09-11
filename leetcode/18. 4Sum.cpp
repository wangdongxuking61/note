// TLE...
// class Solution {
// public:
//     vector<vector<int>> fourSum(vector<int>& nums, int target) {
//         set<multiset<int>> ans;
//         unordered_map<int, vector<pair<int, int>>> m;

//         for (int i = 0; i < nums.size(); ++i)
//             for (int j = i + 1; j < nums.size(); ++j)
//             {
//                 auto it = m.find(nums[i] + nums[j]);
//                 if (it != m.end())
//                     (it->second).push_back({i, j});
//                 else
//                     m.insert({nums[i] + nums[j], {{i, j}}});
//             }

//         for (auto A : m)
//         {
//             auto itB = m.find(target - A.first);
//             if (itB != m.end())
//                 for (auto a : A.second)
//                     for (auto b : itB->second)
//                         if (!(a.first == b.first || a.first == b.second || a.second == b.first || a.second == b.second))
//                             ans.insert({nums[a.first], nums[a.second], nums[b.first], nums[b.second]});
//         }

//         vector<vector<int>> ret;
//         for (auto per_ans : ans)
//             ret.push_back(vector<int> (per_ans.begin(), per_ans.end()));
//         return ret;
//     }
// };

// O(n^3)，两边两个数前后夹逼O(n^2),中间两个数前后夹逼O(n),同twoSum
// http://bbs.csdn.net/topics/390853322
#include "vector"
#include "iostream"
using namespace std;

class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target)
    {
        vector<vector<int>> ans;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size() && nums[i] <= target/4.0; ++i)
        {
            if(i > 0 && nums[i] == nums[i-1])   continue;
            for (int j = (int)nums.size() - 1; j > i + 2 && nums[j] >= target/4.0; --j)
            {
                if(j < (int)nums.size() - 1 && nums[j] == nums[j+1])    continue;
                int front = i + 1, back = j - 1, newtarget = target - nums[i] - nums[j];
                while (front < back)
                {
                    if (nums[front] + nums[back] > newtarget)
                        do back--; while (front < back && nums[back] == nums[back + 1]);
                    else if (nums[front] + nums[back] < newtarget)
                        do front++; while (front < back && nums[front] == nums[front - 1]);
                    else
                    {
                        ans.push_back({nums[i], nums[front], nums[back], nums[j]});
                        cout<<i<<" "<< front<<" "<<back<<" "<< j<<endl;
                        cout<<nums[i]<<" "<< nums[front]<<" "<<nums[back]<<" "<< nums[j]<<endl;
                        do back--;  while (front < back && nums[back] == nums[back + 1]);
                        do front++; while (front < back && nums[front] == nums[front - 1]);
                    }
                }
            }
        }
        return ans;
    }
};

int main()
{
    vector<int> nums = {1,-2,-5,-4,-3,3,3,5};
    Solution s;
    vector<vector<int>> ans = s.fourSum(nums, -11);
    return 0;
}