class Solution {
public:
    static bool cmp(const pair<int, int> &a, const pair<int, int> &b)
    {
        return a.second > b.second ? true : false;
    }
    
    string frequencySort(string s) {
        vector<pair<int, int>> vec;
        for(int i = 0; i < 128; i++)
            vec.push_back({i, 0});
        for(char ch : s)
            vec[ch].second++;
        sort(vec.begin(), vec.end(), cmp);
        string ans(s);
        int cn = 0;
        for(int i = 0; i < 128; i++)
            for(int j = 0; j < vec[i].second; j++)
                ans[cn++] = (char)vec[i].first;
        return ans;
    }
};