#include<iostream>
#include"../include/int_array.h"



IntArray::IntArray(unsigned int size) {
    m_usize = size;
    m_pData = new int[size];
}

IntArray::~IntArray() {
    delete[] m_pData;
    m_pData = nullptr;
    std::cout << "destruction 1" << std::endl;
}

IntArray& IntArray::operator=(const IntArray& other) {
    if (this != &other) {
        delete[] m_pData;
        std::cout << "destruction 2" << std::endl;
        m_usize = other.m_usize;

        m_pData = new int[m_usize];
        std::copy(other.m_pData, other.m_pData + other.m_usize, m_pData);
    }

    return *this;
}

IntArray::IntArray(const IntArray& other) {

    m_usize = other.m_usize;
    m_pData = new int[other.m_usize];

    std::copy(other.m_pData, other.m_pData + other.m_usize, m_pData);
}

int& IntArray::operator[](const unsigned int& index) {
    return m_pData[index];
}

unsigned int IntArray::size() const {
    return m_usize;
}