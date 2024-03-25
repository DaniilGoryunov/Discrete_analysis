#include <iostream>
#include <cstring>


template <typename T>
class MyVector {

private:
    T* data;
    size_t size;
    size_t capacity;

public:
    MyVector() : data(nullptr), size(0), capacity(0) {}

    MyVector(size_t count, T value)
    {
        data = new T[count];
        size = count;
        capacity = count;
        for (size_t i = 0; i < count; i++)
        {
            data[i] = value;
        }
    }

    MyVector(size_t count)
    {
        data = new T[count];
        size = 0;
        capacity = count;
    }

    ~MyVector() {
        delete[] data;
    }

    void PushBack(const T& value) {
        if (size >= capacity) {

            if (capacity == 0){
                capacity = 1;
            }
            else{
                capacity *= 2;
            }

            T* new_data = new T[capacity];
            std::memcpy(new_data, data, size * sizeof(T));
            delete[] data;
            data = new_data;
        }

        data[size++] = value;
    }

    size_t get_size() const {
        return size;
    }

    T& operator[](size_t index) {
        return data[index];
    }
};


class MyString {

private:
    size_t size;
    size_t capacity;

public: 
    char* data;

    MyString(char* str) {
        capacity = 64;
        data = new char[capacity];
        strcpy(data, str);
        
        size = std::strlen(str);

        for (size_t i = size; i < 64; i++)
        {
            data[i] = '-';
        }
    }

    void PrintStr()
    {
        int i = 0;

        while (data[i] != '-' && i < capacity) 
        {std::cout << data[i];
        i++;
        }
    }

    ~MyString() {
        delete[] data;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) { 
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new char[capacity];
            memcpy(data, other.data, size + 1);
        }
        return *this;
    }

};

struct Pair{
    uint64_t key;
    MyString* value;
};

void CountingSort(MyVector<Pair>& arr, uint64_t exp) {
    MyVector<Pair> output(arr.get_size());
    MyVector<int> count(10, 0);

    for (int i = 0; i < arr.get_size(); i++) {
        const Pair& pair = arr[i];
        count[(pair.key / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = arr.get_size() - 1; i >= 0; i--) {
        output[count[(arr[i].key / exp) % 10] - 1].key = arr[i].key;
        output[count[(arr[i].key / exp) % 10] - 1].value = arr[i].value;
        count[(arr[i].key / exp) % 10]--;
    }

    for (size_t i = 0; i < arr.get_size(); i++) {
        arr[i] = output[i];
    }
}

void RadixSort(MyVector<Pair>& arr) {
    uint64_t max = arr[0].key;

    for (int i = 0; i < arr.get_size(); i++) {
        const Pair& pair = arr[i];
        if (pair.key > max) {
            max = pair.key;
        }
    }

    uint64_t exp = 1;

    while (max / exp > 0) {
        CountingSort(arr, exp);
        exp *= 10;
        if (exp == 0 ) break;
    }
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    MyVector<Pair> myVector;
    uint64_t key = 0;
    char* value = new char[65];
    Pair p;
    bool f = false;

    if (std::cin >> key >> value){
        f = true;
    };

    if (key != 0 or f == true){
        do{
            p.key = key;
            MyString* str = new MyString(value);
            p.value = str;
            myVector.PushBack(p);
        } while (std::cin >> key >> value);
    }
    else return 0;
    
    delete[] value;

    RadixSort(myVector);

    for(size_t i = 0; i < myVector.get_size(); i++){
        std::cout << myVector[i].key << '\t';
        myVector[i].value->PrintStr();
        std::cout << '\n';
    }

    return 0;
}