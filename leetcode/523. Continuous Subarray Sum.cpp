// brute
class Solution {
public:
	bool checkSubarraySum(vector<int>& nums, int k) {
		int n = nums.size();
		if (n < 2) return false;

		int *sum = new int[n+1];
		sum[0] = 0;
		for (int i = 0; i < n; i++)
			sum[i + 1] = sum[i] + nums[i];
		
		if (k == 0)
		{
			for (int i = 0; i < n; i++)
				for (int j = i + 2; j <= n; j++)
					if ((sum[j] - sum[i]) == 0)
						return true;
		}
		else
		{
			for (int i = 0; i < n; i++)
				for (int j = i + 2; j <= n; j++)
					if ((sum[j] - sum[i]) % k == 0)
						return true;
		}

		return false;
	}
};

// put sum[i]%k into set
// t = sum[i+1]%k, find there is a same number equal to t in set
// if exist, minus result is the two number is 0.
// time: use set O(nlog(n)), use unordered_set O(n)
class Solution {
public:
	bool checkSubarraySum(vector<int>& nums, int k) {
		int n = nums.size(), sum = 0, pre = 0;
		unordered_set<int> modk;
		for (int i = 0; i < n; ++i) {
			sum += nums[i];
			int mod = k == 0 ? sum : sum % k;
			if (modk.count(mod)) return true;
			modk.insert(pre);
			pre = mod;
		}
		return false;
	}
};
