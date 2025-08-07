#pragma once

// 冒泡排序
class CBubbleSort {

    public:
        CBubbleSort() = default;
        ~CBubbleSort() = default;
    
    public:
        void sort(int ar[], int n);    // 常规排序
        void sort_with_flag();

};