#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cmath>

using namespace std;

#define str(var)                        #var" = %2d   "
#define PRINT_1VAR(v0)                  printf(str(v0)"\n",v0)
#define PRINT_2VAR(v0, v1)              printf(str(v0)"\t" str(v1)"\n",v0,v1)
#define PRINT_3VAR(v0, v1, v2)          printf(str(v0)"\t" str(v1)"\t" str(v2)"\n",v0,v1,v2)
#define PRINT_4VAR(v0, v1, v2, v3)      printf(str(v0)"\t" str(v1)"\t" str(v2)"\t" str(v3)"\n",v0,v1,v2,v3)

#define umap unordered_map
#define uset unordered_set

// 3ms
class Solution
{
public:
    int n, min_cost = INT32_MAX;
    vector<int> _price;
    vector<vector<int>> _special;

    int shoppingOffers(vector<int> &price, vector<vector<int>> &special, vector<int> &needs)
    {
        if ((n = needs.size()) == 0) return 0;
        if(is_zeros(needs)) return 0;

        _price = price;
        _special = special;

        min_cost=0;
        for (int i = 0; i < n; ++i) min_cost+=price[i]*needs[i];

        dp(needs, 0);

        return min_cost;
    }

    void dp(vector<int> &needs, int cost)
    {
        //PRINT_VECTOR(needs);
        if (cost >= min_cost || is_zeros(needs))
        {
            min_cost = min(min_cost, cost);
            return;
        }
        bool gain = false;
        for (auto &offer: _special)
        {
            //PRINT_1VAR(cn++);
            if (can_gain(offer, needs))
            {
                for (int i = 0; i < n; ++i) needs[i] -= offer[i];
                gain = true;
                dp(needs, cost + offer.back());
                for (int i = 0; i < n; ++i) needs[i] += offer[i];
            }
        }
        // very important!!!
        if (!gain)
        {
            for (int i = 0; i < n; ++i)
                cost += _price[i] * needs[i];
            min_cost = min(min_cost, cost);
        }
    }

    bool can_gain(vector<int> &offer, vector<int> &needs)
    {
//        PRINT_VECTOR(offer);
//        PRINT_VECTOR(needs);
        for (int i = 0; i < n; ++i)
            if (offer[i] > needs[i])
                return false;
        return true;
    }

    bool is_zeros(vector<int> &needs)
    {
        for (int i : needs)
            if (i > 0)
                return false;
        return true;
    }
};


// 800ms
const int maxNum = 0x3f3f3f3f;
const int N = 7;

class Solution
{
public:
	int mod[7] = { 1 };

	int shoppingOffers(vector<int> &price, vector <vector<int>> &special, vector<int> &needs)
	{
		int n = needs.size();
		if (n == 0)	return 0;

		int max = *max_element(needs.begin(), needs.end());
		if (max == 0)	return 0;

		for (int i = 0; i < n; i++)
		{
			mod[i + 1] = mod[i] * N;
			vector<int> tmp(n + 1);
			tmp[i] = 1;
			*tmp.rbegin() = price[i];
			special.push_back(tmp);
		}

		int size = (int)pow(N, n) + 1;
		int *dp = (int *)malloc(size * sizeof(int));
		memset(dp, 0x3f, size * sizeof(int));
		dp[0] = 0;

		array<int, N> s0;
		for (int i = 0; i < size; i++)
		{
			if (dp[i] == maxNum)	continue;
			for (int j = 0; j < n; ++j) s0[j] = i % mod[j + 1] / mod[j];

			for (auto &sp : special)
			{
				bool overflow = false;
				for (int j = 0; j < n; ++j)
					if (s0[j] + sp[j] > needs[j])
					{
						overflow = true;
						break;
					}
				if (!overflow)
				{
					int ii = 0;
					for (int j = 0; j < n; ++j) ii += (s0[j] + sp[j]) * mod[j];
					dp[ii] = min(dp[ii], dp[i] + sp.back());
				}
			}
		}
		int need_state = get_state(needs);
		return dp[need_state];
	}

	int get_state(vector<int> &s)
	{
		int state = 0;
		for (int j = 0; j < s.size(); ++j)
			state += s[j] * mod[j];
		return state;
	}
};


int main()
{
	Solution s;
	vector<int> price({ 2,6,7,10,2,4 });
	vector <vector<int>> special({ { 0,3,1,4,4,3,12 },{ 3,3,3,3,4,5,25 },{ 4,6,0,2,1,1,5 },{ 1,3,0,4,4,2,6 },{ 0,3,1,0,5,0,2 },{ 0,3,1,3,4,4,24 },{ 6,0,1,3,4,2,19 },{ 5,1,4,5,3,4,22 } });
	vector<int> needs = { 0,4,5,0,0,6 };

	/*vector<int> price({ 2,5 });
	vector <vector<int>> special({ { 3,0,5 },{ 1,2,10 } });
	vector<int> needs = { 3,2 };
*/
	cout << s.shoppingOffers(price, special, needs);

	return 0;
}
