#pragma once


class IntArray {
    public: 
        IntArray(unsigned int size);
        ~IntArray();
        IntArray(const IntArray& other);
        IntArray& operator=(const IntArray& other);

        int& operator[] (const unsigned int& index);
        unsigned int size() const ;

    private:
        int* m_pData;
        unsigned int m_usize;
};