class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> ans;
        vector<string> str = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};

        if(digits.size() == 0)  return ans;

        int d = digits[0] - '0';
        for (int j = 0; j < str[d].size(); ++j)
            ans.push_back(string(1, str[d][j]));

        for (int i = 1; i < digits.size(); ++i)
        {
            int d = digits[i] - '0';
            int last_size = ans.size();
            for (int j = 1; j < str[d].size(); ++j)
                for (int k = 0; k < last_size; ++k)
                    ans.push_back(ans[k] + str[d][j]);
            for (int k = 0; k < last_size; ++k)
                ans[k] += str[d][0];
        }
        return ans;
    }
};