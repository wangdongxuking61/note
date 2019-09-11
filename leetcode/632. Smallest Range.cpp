// use priority_queue, merge n sorted array
class Solution {
public:
	struct mycompare {
		bool operator () (pair<int, int>& a, pair<int, int>& b) { return a.first > b.first; }
	};
	vector<int> smallestRange(vector<vector<int>>& nums) {
		int n = nums.size(), large = INT_MIN, maxlen = INT_MAX;
		priority_queue<pair<int, int>, vector<pair<int, int> >, mycompare> pq;
		for (int i = 0; i < nums.size(); i++) {
			large = max(large, nums[i][0]);
			pq.push({ nums[i][0], i });
		}
		vector<int> ans(2, 0), idx(n, 0);
		while (pq.size() == n) {
			int cur = pq.top().first, row = pq.top().second;
			pq.pop();
			if (large - cur < maxlen) {
				maxlen = large - cur;
				ans[0] = cur;
				ans[1] = large;
			}
			if (++idx[row] < nums[row].size()) {
				pq.push({ nums[row][idx[row]], row });
				large = max(large, nums[row][idx[row]]);
			}
		}
		return ans;
	}
};

// Runtime: 1176 ms
class Solution {
public:
	vector<int> smallestRange(vector<vector<int>>& nums)
	{
		int n = nums.size() ;

		pair<int,int> a_limit(INT32_MAX, 0);
		vector<pair<int, int> > vec;
		for (int i = 0; i < n; i++)
		{
			if (a_limit.first > nums[i].back())
			{
				a_limit.first = nums[i].back();
				a_limit.second = i;
			}
			for (int j = 0; j < nums[i].size(); j++)
				vec.push_back(make_pair(nums[i][j], i));
		}
		sort(vec.begin(), vec.end());

		int pos_a = find(vec.begin(), vec.end(), a_limit) - vec.begin();
		int nn = vec.size() , d = INT32_MAX, ret_a, ret_b ,sum;
		bool *is_vis = new bool[n];
		for (int i = 0; i <= pos_a; i++)
		{
			sum = 0;
			for (int i = 0; i < n; i++)	is_vis[i] = 0;

			for (int j = i; j < nn; j++)
			{
				if (is_vis[vec[j].second])	continue;
				if (vec[j].first - vec[i].first >= d)	break;

				is_vis[vec[j].second] = true;
				sum++;
				if (sum == n)
				{
					if (vec[j].first - vec[i].first < d)
					{
						d = vec[j].first - vec[i].first;
						ret_a = vec[i].first;
						ret_b = vec[j].first;
					}
					break;
				}
			}
		}

		return { ret_a, ret_b };
	}
};
