class Solution {
public:
	double findMaxAverage(vector<int>& nums, int k) {
		int i = 0, j = 0;
		double max_sum = 0;
		for (;j < k;++j)
			max_sum += nums[j];

		double tmp_sum = max_sum;
		int n = nums.size();
		while (j < n)
		{
			tmp_sum += nums[j++] - nums[i++];
			if (tmp_sum > max_sum)
				max_sum = tmp_sum;
		}
		return max_sum / k;
	}
};
