#pragma once


// bubble sort
void bubble_sort(int arr[], const unsigned len);
void cocktail_bubble_sort(int arr[], const unsigned len);

void selection_sort(int arr[], const unsigned len);

void insertion_sort(int arr[], const unsigned len);

void quick_sort(int arr[], const unsigned left, const unsigned right);

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

