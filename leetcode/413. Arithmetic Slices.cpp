// very easy
// find each arithmetic sequence
//  ans += (n-2)*(n-1)/2    note: n is size of  sequence

class Solution {
public:
	int numberOfArithmeticSlices(vector<int>& A)
	{
		int n = A.size();
		if (n < 3)
			return 0;

		int i = 0, ans = 0;
		while (i + 1 < n)
		{
			int d = A[i + 1] - A[i], i_first = i;
			i += 2;

			while (i < n && A[i] == A[i-1] + d)	++i;
			int num = i - i_first;
			if (num >= 3)
				ans += (num - 2)*(num - 1) / 2;
			if (i < A.size())
				--i;
		}
		return ans;
	}
};
