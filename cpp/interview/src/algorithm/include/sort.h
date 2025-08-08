#pragma once


// bubble sort
void bubble_sort(int arr[], const unsigned len);
void cocktail_bubble_sort(int arr[], const unsigned len);

void selection_sort(int arr[], const unsigned len);

void insertion_sort(int arr[], const unsigned len);

void quick_sort_non_recursive(std::vector<int>& arr);

template <typename T>
void shell_sort(T arr[], unsigned len) {
 
    for (int gap = len / 2; gap > 0; gap /= 2) { 
        for (int i = gap; i < len; ++i) {
            T temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

void heap_sort(int arr[], unsigned len);

void merge_sort_non_recursive(int* a, int n);

void bucket_sort(int arr[], unsigned len);


void radix_sort(std::vector<int>& arr);

