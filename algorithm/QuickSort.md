```
//arr是数组首指针，left是第一个元素位置，right是最后一个元素位置(size()-1)
void QuickSort(int* arr, int left, int right)
{
    if(left >= right)
        return;
    int i = left, j = right, pivot = arr[left];
    while(i < j)
    {
        while(i<j && arr[j]>=pivot) j--;
        arr[i] = arr[j];
        while(i<j && arr[i]<=pivot) i++;
        arr[j] = arr[i];
    }
    arr[i] = pivot;
    QuickSort(arr, left, i-1);
    QuickSort(arr, i+1, right);
}

int main()
{
    int arr[] = {3, 2, 1};
    QuickSort(arr, 0, 3 - 1);
    return 0;
}
```