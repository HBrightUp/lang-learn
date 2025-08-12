#include <iostream>
#include <stack>
#include<vector>
#include<algorithm>
#include<cstring>
#include"../../common/include/template.hpp"



void bubble_sort(int arr[], const unsigned len)
{
    if (arr == nullptr || len == 0)
    {
        return;
    }

    bool is_swaped;

    for (int i = 0; i < len - 1; ++i) {
        is_swaped = false; 
        for (int j = 0; j < len - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                is_swaped = true;
            }
        }
        if (!is_swaped) {
            break; 
        }
    }

}

void cocktail_bubble_sort(int arr[], const unsigned len) {
    if (arr == nullptr || len == 0) {
        return ;
    }
    
    bool swapped = true;
    int start = 0;
    int end = len - 1;

    while (swapped) {
        swapped = false;
        for (int i = start; i < end; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

        for (int i = end - 1; i >= start; --i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        ++start;
    }

}
/******************************* selection sort ******************************* */
void selection_sort(int arr[], const unsigned len) {
    if (arr == nullptr ||  len == 0) {
        return ;
    }
    
    int min_index;
    for (int i = 0; i < len - 1; ++i) {
        min_index = i;
        for (int j = i + 1; j < len; ++j) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
       
        std::swap(arr[i], arr[min_index]);
    }
}

/************************************** insert sort *************************** */
void insertion_sort(int arr[], const unsigned len) {
    
    int key,j;
    for (int i = 1; i < len; ++i) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/************************************** quick  sort ************************ */
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; 
    int i = low - 1; 

    for (int j = low; j <= high - 1; j++) {

        if (arr[j] <= pivot) {
            i++; 
            //std::cout << "i: " << i << ", j:" << j << std::endl;
            if (i !=j) {
                swap(arr[i], arr[j]);
            }
            
        }
    }
    swap(arr[i + 1], arr[high]); 
    return i + 1; 
}

void quick_sort_non_recursive(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) {
        return; 
    }

    std::stack<std::pair<int, int>> st; 
    st.push({0, n - 1});

    while (!st.empty()) {

        std::pair<int, int> currentRange = st.top();
        st.pop();
        int low = currentRange.first;
        int high = currentRange.second;

        if (low < high) { 
            int pivotIndex = partition(arr, low, high); 

            st.push({low, pivotIndex - 1});
            st.push({pivotIndex + 1, high});
        }
    }
}

/*************************************** heap sort ************************** */
void maxHeapify(int arr[], int start, int end) {
    int dad = start;
    int son = 2 * dad + 1; // 左子节点
    while (son <= end) {
        // 如果右子节点存在，并且比左子节点大
        if (son + 1 <= end && arr[son] < arr[son + 1]) {
            son++; // 选右子节点
        }
        // 如果父节点小于任何一个子节点，则交换
        if (arr[dad] < arr[son]) {
            swap(arr[dad], arr[son]);
            dad = son;
            son = 2 * dad + 1;
        } else {
            return; // 父节点已经满足最大堆性质，无需再调整
        }
    }
}

void heap_sort(int arr[], unsigned len) {
  
    // 构建最大堆
    for (int i = len / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, i, len - 1);
    }

    // 从最后一个元素开始，逐个将最大元素（堆顶）放到数组末尾
    for (int i = len - 1; i > 0; i--) {
        swap(arr[0], arr[i]); // 交换堆顶和当前未排序部分的最后一个元素
        maxHeapify(arr, 0, i - 1); // 重新调整堆，保持最大堆性质
    }
}

/***************************** merge sort ************************ */
void merge_sort_non_recursive(int* a, int n)
{
	int* p = (int*)malloc(sizeof(int) * n);
    if (p == nullptr) {
        return ;
    }

	int gap = 1;  
	while (gap < n)
	{
		for (int i = 0; i < n ; i += 2*gap)
		{
			int left1 = i; int right1 = i + gap - 1;
			int left2 = i+gap; int right2 = i + 2*gap - 1;
			
			if (left2 > n)
			{
				break;
			}
			if (right2 > n)
			{
				right2 = n - 1;
			}

			int index = i;
			while (left1 <= right1 && left2 <= right2)
			{
				if (a[left1] < a[left2])
				{
					p[index] = a[left1];
					index++;
					left1++;
				}
				else
				{
					p[index] = a[left2];
					index++;
					left2++;
				}
			}
			while (left1 <= right1)
			{
				p[index] = a[left1];
				index++;
				left1++;
			}
			while (left2 <= right2)
			{
				p[index] = a[left2];
				index++;
				left2++;
			}
			std::memcpy(a + i, p + i, sizeof(int) * (right2 - i + 1));
		}
		gap *= 2;
	}
}

/********************************* bucket sort ********************* */
void bucket_sort(int arr[], unsigned len) {
    
    if (arr == nullptr ||  len <= 1) {
        return; 
    }

    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < len; ++i) {
        minVal = std::min(minVal, arr[i]);
        maxVal = std::max(maxVal, arr[i]);
    }

    int bucketCount = 10; 
    std::vector<std::vector<double>> buckets(bucketCount);

    for (int i = 0; i < len; ++i) {
        int bucketIndex = (int)((arr[i] - minVal) * (bucketCount - 1) / (maxVal - minVal) );
        buckets[bucketIndex].push_back(arr[i]);
    }

    for (int i = 0; i < bucketCount; ++i) {
        sort(buckets[i].begin(), buckets[i].end());
    }

    int index = 0;
    for (int i = 0; i < bucketCount; ++i) {
        for (double num : buckets[i]) {
            arr[index++] = num;
        }
    }
}

/**********************  radix sort  **************************/

static const unsigned bucket_nums = 6;

int getDigit(int number, int digit) {
    int divisor = 1;
    for (int i = 0; i < digit; ++i) {
        divisor *= bucket_nums;
    }
    return (number / divisor) % bucket_nums;
}

// 
int maxDigit(const std::vector<int>& arr) {
    int max_val = *std::max_element(arr.begin(), arr.end());
 
    int digits = 0;
    while (max_val > 0) {
        max_val /= bucket_nums;
        digits++;
    }

    return digits;
}

// non_r
void radix_sort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) {
        return; 
    }

    int max_digits = maxDigit(arr);

    for (int digit = 0; digit < max_digits; ++digit) {

        std::vector<std::vector<int>> buckets(bucket_nums);

        for (int i = 0; i < n; ++i) {
            int d = getDigit(arr[i], digit);
            buckets[d].push_back(arr[i]);

        }

        // 将桶中的元素放回原数组
        int index = 0;
        for (int i = 0; i < bucket_nums; ++i) {
      
            for (int j = 0; j < buckets[i].size(); ++j) {
                arr[index++] = buckets[i][j];
            }
        }
     
    }
}