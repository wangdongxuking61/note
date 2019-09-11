class Solution
{
public:
    int myAtoi(string str)
    {
        long flag = 1, num = 0;
        while (!str.empty())
        {
            if (str[0] == ' ')
                str.erase(str.begin());
            else
                break;
        }
        while (!str.empty())
        {
            if (str.back() == ' ')
                str.pop_back();
            else
                break;
        }

        if (str[0] == '+')
            flag = 1;
        else if (str[0] == '-')
            flag = -1;
        else if (isdigit(str[0]))
            num = str[0] - '0';
        else
            return 0;

        for (int i = 1; i < str.size(); ++i)
        {
            if (isdigit(str[i]))
            {
                long tmp = num * 10 + str[i] - '0';
                if (flag==1 && tmp > INT32_MAX)
                    tmp = INT32_MAX;
                else if (flag==-1 && tmp > 2147483648)
                    tmp = 2147483648;
                num = tmp;
            }
            else
                break;
        }
        num *= flag;
        return num;
    }
};
