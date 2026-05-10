
#ifndef STORAGE_H
#define STORAGE_H
#include <iostream>

template <typename T>
class Storage {
private:
    T data[100];  
    int itemCount;  

public:
    Storage() : itemCount(0) {}

    void add(const T& item) {
        if (itemCount < 100) {
            data[itemCount++] = item;  
        }
    }
    T* getAll() {
        return data; 
    }

    
    void removeById(int id) {
        for (int i = 0; i < itemCount; ++i) {
            if (data[i].getId() == id) {
                
                for (int j = i; j < itemCount - 1; ++j) {
                    data[j] = data[j + 1];
                }
                --itemCount;
                break;
            }
        }
    }

    const T* findById(int id) const {
        for (int i = 0; i < itemCount; ++i) {
            if (data[i].getId() == id) {
                return &data[i]; 
            }
        }
        return nullptr; 
    }

    const T* getAll() const {
        return data; 
    }
    void clear()
    {
        itemCount = 0;
    }
    int size() const {
        return itemCount;
    }
    T* findById(int id) {
        for (int i = 0; i < itemCount; ++i) {
            if (data[i].getId() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }
};



#endif 