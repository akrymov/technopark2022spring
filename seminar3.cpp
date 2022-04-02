#include <iostream>
#include <vector>
#include <algorithm>

int partition(int* arr, int l, int r) {
    int pivot_idx = l + (r - l) / 2;
    int pivot = arr[pivot_idx];
    
    int i = l;
    int j = r - 1;
    std::swap(arr[pivot_idx], arr[r - 1]);
    while (true) {
        for (; arr[i] < pivot; i++);
        while ( !(arr[--j] < pivot) && j >= i );
        
        if (i >= j) break;
        std::swap(arr[i], arr[j]);
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}

void kth_element(int *arr, int size, int k) {
    int l = 0;
    int r = size;
    
    while (true) {
        int m = partition(arr, l, r);
        if (m == k) {
            return;
        } else if (m < k) {
            l = m + 1;
        } else {
            r = m;
        }
    }
}




void count_sort(int* arr, int size, int R) {
    int counters_size = R + 1;
    
    int* counters = new int[R];
    std::fill_n(counters, size, 0);
    
    for (int i = 0; i < size; ++i) {
        counters[ arr[i] ]++;
    }
    
    for (int i = 0; i < counters_size - 1; i++) {
        counters[i + 1] += counters[i];
    }
    
    int* aux = new int[size];
    
    for (int arr_idx = size - 1; arr_idx >= 0; arr_idx--) {
        //aux[ --counters[ arr[arr_idx] ] ] = arr[arr_idx];
        int aux_idx = counters[ arr[arr_idx] ] - 1;
        counters[ arr[arr_idx] ]--;
        aux[ aux_idx ] = arr[arr_idx];
    }
    
    for (int i = 0; i < size; ++i) {
        arr[i] = aux[i];
    }
    
    delete [] aux;
    delete [] counters;
    
    
}


int main(int argc, const char * argv[]) {
    /*int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<int> arr;
    for (int i = 0; i < n; ++i) {
        int val = 0;
        std::cin >> val;
        arr.push_back(val);
    }
    kth_element(&arr[0], arr.size(), k);
    std::cout << arr[k] << std::endl;
     */
    std::vector<int> arr = {1,3,5,7,2,4,6,1,2,3};
    count_sort(&arr[0], arr.size(), 10);
    
    for (int val : arr) {
        std::cout  << val << " ";
    }

    return 0;
}
