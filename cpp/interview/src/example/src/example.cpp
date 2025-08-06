#include<iostream>
#include<unordered_map>
#include<sstream>
#include<map>
#include<set>
#include<algorithm>
#include<cmath>
#include<string.h>
#include"../include/example.h"
#include"../include/int_array.h"
#include"../../algorithm/include/list.hpp"

  
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

void call_exp1() {

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

void call_exp2() {
    
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


void call_exp3() {

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

/**********************  exp -04 ****************************/
void call_exp4() {
    std::vector<int> nums = {4,6,3,8,1,9,2};
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a < b;
    });

    MyList<int> list;
    for(const auto& v : nums) {
        list.push_back(v);
    }

    list.print();
}
/**********************  exp -05 ****************************/
template<typename T>
void print_vector(const std::vector<T>& v) {
    for(const auto& d : v) {
        std::cout << d << " ";
    }

    std::cout << std::endl;
}

template<typename T>
void print_multiset(const std::multiset<T>& v) {
    for(const auto& d : v) {
        std::cout << d << " ";
    }

    std::cout << std::endl;
}

const unsigned int find_min_element_position(const std::vector<int>& top_numbs) {

    int min = *top_numbs.begin();
    unsigned int pos = 0;
    unsigned int len = 0;

    for(auto& data : top_numbs) {
        if (min > data) {
            min = data;
            pos = len;
        }
         ++len;
    }

    return pos;
}

std::vector<int> find_top_numbers_m1(std::vector<int>& nums, const unsigned int& top) {
    std::vector<int> top_numbs;

    auto iter_end = nums.begin() + top;
    std::copy(nums.begin(), iter_end, std::back_inserter(top_numbs));
    unsigned int min_position = find_min_element_position(top_numbs);

    for(std::vector<int>::iterator iter = nums.begin() + top; iter != nums.end(); ++iter) {
        if (top_numbs[min_position] > *iter) {
            continue;
        } 
  
        top_numbs[min_position] = *iter;
     
        min_position = find_min_element_position(top_numbs);      
        
    }
    
    std::sort(top_numbs.begin(), top_numbs.end(), [](int a, int b) {
        return a > b;
    });

    return top_numbs;
}

std::vector<int> find_top_numbers_m2(std::vector<int>& nums, const unsigned int& top) {
    std::multiset<int> s;
    std::vector<int> top_nums;
    
    for(const auto& num : nums) {
        s.insert(num);
    }

    print_multiset(s);

    unsigned int len = 0;
    for(std::multiset<int>::reverse_iterator iter = s.rbegin(); iter != s.rend(); ++iter) {
        top_nums.push_back(*iter);

        if(++len == top) {
            break;
        }
    }
    

    return top_nums;
}

void call_exp5() {

    std::vector<int> nums = {4,6,3,8,1,9,2, 7, 10, 3, 5,8};
    const unsigned int& top = 4;

    // methon-1
    std::vector<int> top_nums = find_top_numbers_m1(nums, top);
    print_vector(top_nums);

    // methon-2
    top_nums.clear();
    top_nums = find_top_numbers_m2(nums, top);
    print_vector(top_nums);

}

/***************************************exp-06 *************************** */
template<typename T> 
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}


void RightLoopMove(char *pstr, unsigned short steps) {

    unsigned int len = strlen(pstr);
    char* p1 = pstr;
    char* p2 = pstr + len - steps;
    char temp = 0;

    for(unsigned int i = 0; i < steps; ++i) {
        temp = *p1;
        *p1++ = *p2;
        *p2++ = temp;
    }
}

void call_exp6() {
    char str[] = "There is a small river in front of my house.";
    const unsigned int steps = 6;
    RightLoopMove(str, steps);
    std::cout << str << std::endl;
}

/***************************************exp-07 *************************** */

bool  IsRevStr( char *str) {
    bool is_rev = true;
    unsigned int len = strlen(str);

    for(unsigned int i = 0; i < len/2; ++i) {
        if (*(str + i) != *(str + len -i - 1)) {
            is_rev = false;
            break;
        }
    }

    return is_rev;
}

void call_exp7() {

    char str1[] = "98789";
    std::cout << std::boolalpha;
    std::cout << str1 << " is a Palindrome: " << IsRevStr(str1) << std::endl;

    char str2[] = "987789";
    std::cout << std::boolalpha;
    std::cout << str2 << " is a Palindrome: " << IsRevStr(str2) << std::endl;

    char str3[] = "987891";
    std::cout << std::boolalpha;
    std::cout << str3 << " is a Palindrome: " << IsRevStr(str3) << std::endl;

}

/***************************************exp-08 *************************** */
int str2int (const char *str) {
  
    unsigned int len = strlen(str);
    unsigned int pos = 0;
    if(*str == '-' || *str == '+') {
        len = len - 1;
        pos = 1;
    }

    const unsigned int base = 10;
    unsigned int sum = 0;

    for(int i = 0; i < len; ++i) {
        sum += (*(str + pos + i) - '0') * std::pow( base , (len - i-1));    
    }

    if(*str == '-') {
        sum = -sum;
    }

    return sum;
}

void call_exp8() {
    char str1[] = "-3435";
    std::cout << str2int(str1) << std::endl;

    // char str2[] = "+5142";
    // std::cout << str2int(str2) << std::endl;

    // char str3[] = "223435";
    // std::cout << str2int(str3) << std::endl;
}

/***************************************exp-09 *************************** */

void int2str(const int& data, char *str) {
    if (str == nullptr) {
        return ;
    }

    bool is_postive = true;
    int a = data;
    if (a < 0) {
        is_postive = false;
        a = -a;
    }

    const unsigned int base = 10;
    unsigned int len = 0;
    
    if (a < base) {
        len = 1;
    } else {
        while(a) {
            ++len;
            a = a/base;
        }
    }

    a = data < 0? -data : data;
    char *p = nullptr;

    if (is_postive) {
        p = str + len ;
    } else   {
        p = str + len + 1;
    }
     
    *p-- = '\0';

    for(unsigned int i = 0; i < len; ++i) {
        *p-- = a % base + '0';
        a = a / base;
    }

    if(!is_postive) {
        *p = '-';
    }

}

void call_exp9() {
    int a1 = -1234;
    char s1[100];
    int2str(a1, s1);
    std::cout << s1 << std::endl;

    int a2 = 26745553;
    char s2[100];
    memset(s2, sizeof(s2)/sizeof(char), 0);
    int2str(a2, s2);
    std::cout << s2 << std::endl;

}

/***************************************exp-10 *************************** */
void strsort(char *str) {

    char *p1 = str;
    char *p2 = nullptr;
    char temp;

    while( *p1 != '\0') {
        p2 = p1 + 1;
        while(*p2 != '\0') {
            if (*p1 > *p2) {
                temp = *p1;
                *p1 = *p2;
                *p2 = temp;
            }

            ++p2;
        }
        ++p1;
    }
}

void call_exp10() {
    char str[] = "thisthrowclassgetjsi";
    strsort(str);
    std::cout << str << std::endl;
}