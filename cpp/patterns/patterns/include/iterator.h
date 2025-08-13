#pragma once

#include <iostream>
#include <vector>
#include <memory> // 包含 unique_ptr

class List {
public:
    void add(int value) {
        data_.push_back(value);
    }

    size_t size() const {
        return data_.size();
    }

    int get(size_t index) const {
        return data_[index];
    }

    class Iterator {
    public:
        Iterator(const List* list, size_t index) : list_(list), index_(index) {}

        int next() {
            if (index_ < list_->size()) {
                return list_->get(index_++);
            }
            return -1; // Or throw an exception
        }

        bool is_done() const {
            return index_ >= list_->size();
        }

    private:
        const List* list_;
        size_t index_ = 0;
    };

    std::unique_ptr<Iterator> create_iterator() const {
        return std::make_unique<Iterator>(this, 0);
    }

private:
    std::vector<int> data_;
};

