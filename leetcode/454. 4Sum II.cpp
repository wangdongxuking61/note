class Solution {
public:
    int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D)
    {
        int N = A.size(), N2 = N * N, *sumAB, *sumCD;
        if (N == 0)
            return 0;

        sumAB = (int *)malloc(sizeof(int) * N2);
        sumCD = (int *)malloc(sizeof(int) * N2);
        for (int i = 0, cn = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
            {
                sumAB[cn] = A[i] + B[j];
                sumCD[cn++] = C[i] + D[j];
            }
        sort(sumAB, sumAB + N2);
        sort(sumCD, sumCD + N2);

        int ans = 0;
        int front = 0, back = N2 - 1;
        while (front < N2 && back >= 0)
        {
            if(sumAB[front] + sumCD[back] > 0)
                back--;
            else if(sumAB[front] + sumCD[back] < 0)
                front++;
            else
            {
                int cnAB = 1, cnCD = 1;
                while (++front < N2 && sumAB[front] == sumAB[front-1])  cnAB++;
                while (--back >= 0 && sumCD[back] == sumCD[back+1])     cnCD++;
                ans += cnAB * cnCD;
            }
        }
        return ans;
    }
};