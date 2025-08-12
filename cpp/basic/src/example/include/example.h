#pragma once

#include<iostream>
#include<vector>


// exp-01: 给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那两个整数，并返回他们的数组下标.
void call_exp1();

// exp-02: 给定一段文本，我们需要统计其中每个单词出现的频率。
void call_exp2();

// exp-03: 实现一个简单的类，该类内部使用动态内存分配来存储一个整数数组。我们需要展示如何正确地管理这块内存，防止内存泄漏，并实现拷贝构造函数和拷贝赋值运算符
void call_exp3();

// exp-04: 利用无序数组元素构建一个有序单链表。
void call_exp4();

// exp-05: 写一个函数找出一个整数数组中，找出前面三个最大的数。 
void call_exp5();

// exp-06: 字符串的环形移动
//参数1：char *pstr 表示需要移动的字符串
//参数2：unsigned short steps 表示从第几个字符开始向右移动
//void RightLoopMove(char *pstr, unsigned short steps);
void call_exp6();

// exp-07:如何判断一个字符串是否是回文串？
//"回文数"是一种数字。如：98789, 这个数字正读是98789,倒读也是98789,正读倒读一样，所以这个数字就是回文数, 类似的 987789 也是.
//int  IsRevStr(char *str);
void call_exp7();

// exp-08: 如何把数字字符串转换为整型数据？
//int str2int (const char *str);
void call_exp8();

// exp-09: 如何把整型数据转换为字符串？
// void  int2str(int data,char *str);
void call_exp9();

// exp-10: 如何对字符串进行排序？
// void strsort(char *str);
void call_exp10();

// exp-11: 如何把字符串中某个指定的字符删除？
//char * DeleChar(char *str,char c);
void call_exp11();

//exp-12: 如何找出01字符串中0与1出现的最大次数？
// void  Calculate(const char *str,int *max0,int *max1);
void call_exp12();

//exp-13: 如何从字符串的某一个位置删除指定个数的字符？\
// char  *deleteChar(char *str,int pos,int len);
void call_exp13();

// exp-14: 写一个函数把字符串反转
// char* strRev(const char *str);
void call_exp14();

// exp-15: 写一个函数查找两个字符串中的第一个公共字符串、最长字串，所有公共字串
// char* find_first_common_str( char *str1,  char *str2, char* dest); // 找出第一个公共字串;
// char* find_max_common_str( char *str1,  char *str2, char* dest); // 查找最长的公共字符串
// std::vector<std::string> find_all_common_str( char *str1,  char *str2); // 查找出所有相同的字符串
void call_exp15();

// exp-16: 写一个函数在字符串N中查找第一次出现子串M的位置
// int StrStr(const char *src, const char *sub);
void call_exp16();

// exp-17: 写一个函数把字符串A中的B字符子串用字符串C进行替换
//char* replace(const char *str, const char *sub1, const char *sub2, char *output);
void call_exp17();

// exp-18: 判断机器的大小端字节
void call_exp18();

// exp-19: 用户输入两个整数，求最大公约数和最大公倍数
void call_exp19();

// exp-20: 查找链表的中间节点
void call_exp20();




void call_example();