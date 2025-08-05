#pragma once

#include<iostream>
#include<vector>

// exp-01: 给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那两个整数，并返回他们的数组下标.
void get_indexs_call();
// method-1
std::vector<int> getInddexs_methon1( const std::vector<int>& nums, const int& target);
//methon-2
std::vector<int> getInddexs_methon2( const std::vector<int>& nums, const int& target);