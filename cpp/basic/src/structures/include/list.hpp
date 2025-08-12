#pragma once
#include<iostream>
#include<vector>

template <class T>
concept integral = std::is_integral_v<T>;

template<class T > requires integral<T>
struct Node{
    T data;
    Node<T>* next;
    Node(const T& d):data(d),next(nullptr) {}
};

template<class T >  
class MyList {
    public:
        MyList();
        ~MyList();

        void create_list(const T& data);
        void push_back(const T& data);
        void insert(const unsigned int& pos, const T& data);
        void erase(const T& data);
        void update(const T& old_data, const T& new_data);
        Node<T>* reverse();
        std::vector<T> find_mid_data();
        void print();

    private:
        Node<T> *head;

        void clear();
        Node<T>* find(const T& data);
};


template<class T>
MyList<T>::MyList() {
    head = nullptr;
}

template<class T>
MyList<T>::~MyList() {

}

template<class T>
void MyList<T>::create_list(const T& data) {
    head = new Node(data);
}

template<class T>
void MyList<T>::push_back(const T& data) {
    Node<T>* p = new Node<T>(data);

    Node<T>* temp = head;
    if(temp == nullptr) {
        head = p;
        return ;
    }

    while(temp->next != nullptr) {
        temp = temp->next;
    }

    temp->next = p;

    return ;
}

template<class T>
void MyList<T>::insert(const unsigned int& pos, const T& data) {
    Node<T>* p = new Node(data);

    Node<T>* temp = head;
    unsigned int index = pos;

    if (index == 0) {
        head = p;
        p->next = temp;
        return ;
    }

    
    Node<T>* prev = nullptr;
    while(index--) {
        prev = temp;
        temp = temp->next;

        if (temp == nullptr) {
            break;
        }
    }

    prev->next = p;
    p->next = temp;

    return ;
}

template<class T>
void MyList<T>::erase(const T& data) {

    Node<T> *temp = nullptr;

    if (head->data == data) {
        temp = head->next;
        delete head;
        head = temp;

        return ;
    }

    Node<T> *p = head->next;
    temp = head;

    while( p != nullptr) {
        if (p->data == data) {
            temp->next = p->next;
            delete p;
            break;
        }

        temp = p;
        p = p->next;
    }

    return ;
}

template<class T>
 void MyList<T>::update(const T& old_data, const T& new_data) {

    Node<T>* temp = head;

    while(temp != nullptr) {
        if (temp->data == old_data) {
            temp->data = new_data;
            break;
        }

        temp = temp->next;
    }

    return ;
 }

 template<class T>
 Node<T>* MyList<T>::reverse() {

    Node<T> *newHead = nullptr;
	Node<T> *p = nullptr;

	while (head != NULL) {
		p = head;
		head = head->next;
		p->next = newHead;
		newHead = p;
	}

    head = newHead;

	return head;

 }

template<class T>
Node<T>* MyList<T>::find(const T& data) {
    Node<T>* p = head;

    while(p != nullptr) {
        if (p->data == data) {
            return p;
        }
    }

    return nullptr;
}

template<class T>
void MyList<T>::print() {
    Node<T>* p = head;

    while(p != nullptr) {
        std::cout << p->data << " ";
        p = p->next;
    }

    std::cout << std::endl;

    return ;
}

template<class T>
void MyList<T>::clear() {

    if (head == nullptr) {
        return ;
    }

    if (head->next == nullptr) {
        delete head;
        head = nullptr;
    }

    Node<T>* p = head;
    Node<T>* temp = p;

    while(temp != nullptr) {
        delete temp;
        temp = p->next;
    }

    return ;
}

template<class T>
std::vector<T> MyList<T>::find_mid_data() {

    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return {};
    }

    Node<T> *p1 = head->next->next;
    Node<T> *p2 = head;
    std::vector<T> v;

    while(p1 != nullptr) {
        p2 = p2->next;

        if (p1->next == nullptr) {
            v.push_back(p2->data);
            break;
        }

        if (p1->next->next == nullptr) {
            v.push_back(p2->data);
            v.push_back(p2->next->data);
            break;
        }

        p1 = p1->next->next;
    }

    return v;
}