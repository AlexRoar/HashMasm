//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HashMasm_GUARD
#define HashMasm_GUARD
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "FastList.h"
#include "CRC.h"

template<typename T>
class HashMasm {
    struct HashCell {
        char *key;
        T value;
        size_t hash;

        void dest(){
            free(key);
        }
    };
    FastList<HashCell>* storage;
    bool         isRehash;
    size_t       capacity;
    unsigned     loadRate;
    size_t       size;
    size_t       threshold;

    size_t hashString(const char* key) {
        return CRC::hash64(-1, reinterpret_cast<const unsigned char *>(key));
    }

    constexpr static int listInitSize = 8;
    constexpr static int minCapacity = 32;

    int rehash() {
        FastList<HashCell>* previousStorage = storage;
        size_t newCapacity = capacity * 2;
        storage = (FastList<HashCell>*)calloc(newCapacity, sizeof(FastList<HashCell>));
        if (!storage)
            return EXIT_FAILURE;

        initStorage(storage, newCapacity);

        for (size_t i = 0; i < capacity; i++) {
            for (size_t iter = previousStorage[i].begin(); iter != 0; previousStorage[i].nextIterator(&iter)) {
                HashCell* value = nullptr;
                previousStorage[i].get(iter, &value);
                storage[value->hash % newCapacity].pushBack(*value);
            }
        }

        capacity = newCapacity;
        threshold = loadRate * capacity / 100;
        return EXIT_SUCCESS;
    }

    int tryRehash() {
        if (size > threshold)
            rehash();
    }

    void freeStorage(FastList<HashCell>* storageTest) {
        HashCell* tmp = nullptr;
        for (int i = 0; i < capacity; i++) {
            for (auto it = storageTest[i].begin(); it != 0; storageTest[i].nextIterator(&it)){
                storageTest[i].get(it, &tmp);
                tmp->dest();
            }
        }
        free(storageTest);
    }

    void initStorage(FastList<HashCell>* storageTest, size_t len){
        for (size_t i = 0; i < len; i++) {
            storageTest[i].init(listInitSize);
        }
    }

public:

    int init(size_t newCapacity, bool rehash=true, unsigned newLoadrate = 75) {
        if (newCapacity < minCapacity)
            newCapacity = minCapacity;
        capacity = newCapacity;
        loadRate = newLoadrate;
        isRehash = rehash;
        threshold = loadRate * capacity / 100;
        size = 0;
        storage = (FastList<HashCell>*)calloc(capacity, sizeof(FastList<HashCell>));
        if (!storage) {
            return EXIT_FAILURE;
        }
        initStorage(storage, capacity);
        return EXIT_SUCCESS;
    }

    int init() {
        return init(minCapacity);
    }

    void dest() {
        freeStorage(storage);
    }

    void set(const char* key, const T& value) {
        tryRehash();
        size_t hashedInitial = hashString(key);
        size_t hashed = hashedInitial % capacity;
        char* keyDub = strdup(key);

        HashCell* node = nullptr;
        for (size_t i = storage[hashed].begin(); i != 0; storage[hashed].nextIterator(&i)) {
            HashCell* tmpNode = nullptr;
            storage[hashed].get(i, &tmpNode);
            if (strcmp(tmpNode->key, key) == 0){
                node = tmpNode;
                break;
            }
        }
        if (node) {
            node->value = value;
        } else {
            HashCell newCell = {keyDub, value, hashedInitial};
            storage[hashed].pushBack(newCell);
            size++;
        }
    }

    T* get(const char* key) {
        size_t hashed = hashString(key) % capacity;

        HashCell *node = nullptr;
        for (size_t i = storage[hashed].begin(); i != 0; storage[hashed].nextIterator(&i)) {
            HashCell *tmpNode = nullptr;
            storage[hashed].get(i, &tmpNode);
            if (strcmp(tmpNode->key, key) == 0) {
                node = tmpNode;
                break;
            }
        }
        if (node)
            return &(node->value);
        else
            return nullptr;
    }

    static HashMasm *New() {
        HashMasm *thou = static_cast<HashMasm *>(calloc(1, sizeof(HashMasm)));
        thou->init();
        return thou;
    }

    void Delete() {
        dest();
        free(this);
    }

    bool getIsRehash() const {
        return isRehash;
    }

    size_t getCapacity() const {
        return capacity;
    }

    unsigned int getLoadRate() const {
        return loadRate;
    }

    size_t getSize() const {
        return size;
    }

    size_t getThreshold() const {
        return threshold;
    }

    static const int getListInitSize() {
        return listInitSize;
    }

    static const int getMinCapacity() {
        return minCapacity;
    }
};

#endif //HashMasm_GUARD
