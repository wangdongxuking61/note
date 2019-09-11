class Solution {
public:
    int reverse(int x) {
        long ans = 0, y = x;
        if(y < 0)   y = -y;
        while(y)
        {
            ans = ans * 10 + y%10;
            y /= 10;
        }
        if(x >= 0 && ans < 0x80000000) return int(ans);
        else if(x < 0 && ans <= 0x80000000)    return int(-ans);
        else return 0;
    }
};