class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int combine_size = nums1.size() + nums2.size();
        int* combine = (int *)malloc(sizeof(int) * combine_size);
        int count = 0;
        vector<int>::const_iterator t1 = nums1.begin();
        vector<int>::const_iterator t2 = nums2.begin();
        while (t1 != nums1.end() && t2 != nums2.end())
        {
            if(*t1 < *t2)
                *(combine + count++) = *t1++;
            else
                *(combine + count++) = *t2++;
        }
        while (t1 != nums1.end())
            *(combine + count++) = *t1++;
        while (t2 != nums2.end())
            *(combine + count++) = *t2++;
//        for (int i = 0; i < combine.size(); ++i)
//        {
//            cout << *(combine.begin() + i) << " ";
//        }

        if(combine_size & 0x01)
            return combine[combine_size/2];
        else
            return (combine[combine_size/2-1] + combine[combine_size/2]) / 2.0;
    }
};