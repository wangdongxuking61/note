class Solution {
public:
    double myPow(double x, int n) {
        int flag = false;
        long m = n;
        if (m < 0)
        {
            flag = true;
            m = -m;
        }
        cout<< m<<endl;

        double ans = 1, y = x;
        for (int i = 0; i < 32; i++)
        {
            if (m & (1 << i))
                ans *= y;
            y *= y;
        }
        return flag?(1.0 / ans):ans;
    }
};