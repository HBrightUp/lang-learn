#include<iostream>
#include<unordered_map>
#include<sstream>
#include<map>
#include"../include/example.h"
#include"../include/int_array.h"

  
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

/**********************exp -02  *************************/

std::vector<std::string> split(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::string word;

    while(iss >> word) {
        words.push_back(word);
    }

    return words;
}

std::map<std::string, int> count_words(const std::vector<std::string>& words) {
    std::map<std::string, int> map_words;
    for(const auto& word : words) {
        ++map_words[word];
    }

    return map_words;
}

void print_words_count(const std::map<std::string, int>& map_words) {
    for(const auto& pair : map_words) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void statistics_word_call() {
    
    const std::string text = "this is a sample text with some words this text is a test";
    std::vector<std::string> words = split(text);
    std::map<std::string, int> word_counts = count_words(words);
    print_words_count(word_counts);
}

/*************************  exp-03   *********************** */

void print_intarray( IntArray& arr) {
    for(unsigned int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " " ;
    }

    std::cout << std::endl;
    std::cout << "size of int array: " << arr.size() << std::endl;
}


void manage_array_call() {

    IntArray arr(5);
    print_intarray(arr);    // 0 0 0 0 0

    IntArray a2(3);
    a2[2] = 3;
    a2[1] = 2;
    a2[0] = 1;
    arr = a2;
    print_intarray(arr);

    a2[1] = 8;
    IntArray a3(a2);
    print_intarray(a3);

}
