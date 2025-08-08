#include <iostream>
#include <stack>
#include<vector>
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


