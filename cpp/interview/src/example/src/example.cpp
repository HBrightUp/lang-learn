#include<iostream>
#include<unordered_map>
#include"../include/example.h"

  
  std::vector<int> getInddexs_methon1( const std::vector<int>& nums, const int& target) {

    int isize = nums.size();
    int temp = 0;

    for(int i = 0; i < isize; ++i) {

        temp = target - nums[i];

        for(int j = i + 1; j < isize; ++j) {
            if (temp != nums[j]) {
                continue;
            } else {
                return {i, j};
            }
        }
    }

    return {};
}

std::vector<int> getInddexs_methon2( const std::vector<int>& nums, const int& target) {
    std::unordered_map<int, int>  map;
    int isize = nums.size();
    int complement = 0;

    for(int i = 0; i < isize; ++i) {
        complement = target - nums[i];
        if (map.find(complement) != map.end()) {
            return {i, map[complement]};
        }

        map[nums[i]] = i;
    }

    return {};
}

void get_indexs_call() {

    const std::vector<int> nums = { 2, 7, 11, 15, 3};
    const int target = 13;

    //std::vector<int> v = getInddexs_methon1(nums, target);
    std::vector<int> v = getInddexs_methon2(nums, target);
    if (!v.empty()) {
        for(auto& index : v) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No index found." << std::endl;
    }

}