//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HashMasmFixed_GUARD
#define HashMasmFixed_GUARD
#define ASMOPTFX

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iterator>
#include <x86intrin.h>
#include "FastList.h"
#include "hash/hashes.h"

template<typename T>
class HashMasmFixed {
    struct KeyOperations{
        __m256i key;

        bool operator==(const __m256i& other) {
            __m256i cmp = _mm256_cmp_ps(key, other, _CMP_NEQ_UQ);
            return _mm256_testz_si256(cmp, cmp) == 1;
        }
    };
    struct HashCell {
        KeyOperations key;
        T value;
        size_t hash;
    };


    constexpr static int packedNum    = 32;
    constexpr static int listInitSize = 8;
    constexpr static int minCapacity  = 32;
    FastList<HashCell> *storage;
    bool isRehash;
    size_t capacity;
    unsigned loadRate;
    size_t size;
    size_t threshold;

    size_t hashString(const char *key) {
        return CRC::hash32AsmFLen(reinterpret_cast<const unsigned char *>(key), packedNum);
    }

    size_t hashVector(const __m256i& keyPacked){
        char  symbols[packedNum] __attribute__((aligned (32))) = {};
        _mm256_store_si256((__m256i*)symbols, keyPacked);
        return hashString(symbols);
    }

    int rehash() {
        FastList<HashCell> *previousStorage = storage;
        const size_t newCapacity = capacity * 2;
        storage = (FastList<HashCell> *) calloc(newCapacity, sizeof(FastList<HashCell>));
        if (!storage)
            return EXIT_FAILURE;

        initStorage(storage, newCapacity);

        for (size_t i = 0; i < capacity; i++) {
            for (size_t iter = previousStorage[i].begin(); iter != 0; previousStorage[i].nextIterator(&iter)) {
                HashCell *value = nullptr;
                previousStorage[i].get(iter, &value);
                storage[value->hash % newCapacity].pushBack(*value);
            }
        }

        capacity = newCapacity;
        threshold = loadRate * capacity / 100;
        return EXIT_SUCCESS;
    }

    int tryRehash() {
        if (isRehash && size > threshold)
            return rehash();
        return EXIT_SUCCESS;
    }

    void freeStorage(FastList<HashCell> *storageTest) {
        for (int i = 0; i < capacity; i++) {
            storageTest[i].dest();
        }
        free(storageTest);
    }

    void initStorage(FastList<HashCell> *storageTest, size_t len) {
        for (size_t i = 0; i < len; i++)
            storageTest[i].init(listInitSize);
    }


    HashCell *findCell(const __m256i& key, size_t hashed, size_t *iter = nullptr) {
        HashCell *node = nullptr, *tmpNode = nullptr;
        #ifdef ASMOPTFX
        #pragma message "Asm optimization of findCell loop\n"
        volatile size_t i = storage[hashed].begin();
        findCellLoop:
        asm goto ("test %0, %0\n"
                  "je %l1"::"r"(i):"cc": findCellLoopEnd);
        {
            storage[hashed].get(i, &tmpNode);
            if (tmpNode->key == key) {
                node = tmpNode;
                asm goto("jmp %l0":::: findCellLoopEnd);
            }
            storage[hashed].nextIterator((size_t*)&i);
        }
        asm goto("jmp %l0":::: findCellLoop);
        findCellLoopEnd:
        #endif
        #ifndef ASMOPTFX
        size_t i = storage[hashed].begin();
        for (; i != storage[hashed].end();storage[hashed].nextIterator(&i)){
            storage[hashed].get(i, &tmpNode);
            if (tmpNode->key == key) {
                node = tmpNode;
            }
        }
        #endif
        if (iter)
            *iter = i;
        return node;
    }

public:
    int init(size_t newCapacity, bool rehash = true, unsigned newLoadrate = 75) {
        if (newCapacity < minCapacity)
            newCapacity = minCapacity;
        capacity = newCapacity * 2;
        loadRate = newLoadrate;
        isRehash = rehash;
        threshold = loadRate * capacity / 100;
        size = 0;
        storage = (FastList<HashCell> *) calloc(capacity, sizeof(FastList<HashCell>));
        if (!storage)
            return EXIT_FAILURE;
        initStorage(storage, capacity);
        return EXIT_SUCCESS;
    }

    int init() {
        return init(minCapacity);
    }

    void dest() {
        freeStorage(storage);
    }

    void set(const __m256i& keyPacked, const T &value) {
        tryRehash();
        size_t hashedInitial = hashVector(keyPacked);
        size_t hashed = hashedInitial % capacity;

        HashCell *node = findCell(keyPacked, hashed);
        if (node) {
            node->value = value;
        } else {
            HashCell newCell = {{keyPacked}, value, hashedInitial};
            storage[hashed].pushBack(newCell);
            size++;
        }
    }

    T *get(const __m256i& keyPacked) {
        size_t hashed = hashVector(keyPacked) % capacity;
        HashCell *node = findCell(keyPacked, hashed);
        return node ? &(node->value) : nullptr;
    }

    void remove(const __m256i& keyPacked) {
        size_t hashed = hashVector(keyPacked) % capacity;
        size_t id = 0;
        findCell(keyPacked, hashed, &id);
        if (id) {
            storage[hashed].remove(id);
            size--;
        }
    }

    T &operator[](const __m256i& keyPacked) {
        size_t hashedInitial = hashVector(keyPacked);
        size_t hashed = hashedInitial % capacity, id = 0;
        findCell(keyPacked, hashed, &id);
        if (!id) {
            size++;
            tryRehash();
            HashCell newCell = {};
            newCell.hash = hashedInitial;
            newCell.key.key = keyPacked;
            storage[hashed].pushBack(newCell, &id);
        }
        HashCell *value = nullptr;
        storage[hashed].get(id, &value);
        return value->value;
    }

    static HashMasmFixed *New() {
        auto thou = static_cast<HashMasmFixed *>(calloc(1, sizeof(HashMasmFixed)));
        thou->init();
        return thou;
    }

    void Delete() {
        dest();
        free(this);
    }

    [[nodiscard]] bool getIsRehash() const {
        return isRehash;
    }

    [[nodiscard]] size_t getCapacity() const {
        return capacity;
    }

    [[nodiscard]] unsigned int getLoadRate() const {
        return loadRate;
    }

    [[nodiscard]] size_t getSize() const {
        return size;
    }

    [[nodiscard]] size_t getThreshold() const {
        return threshold;
    }

    static int getListInitSize() {
        return listInitSize;
    }

    static int getMinCapacity() {
        return minCapacity;
    }

    void printBucketsSizes() {
        for(size_t i = 0; i < getCapacity(); i++){
            printf("%zu, ", storage[i].getSize());
        }
        printf("\n");
    }

private:
    struct HashIter {
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        HashMasmFixed *object;
        size_t bucket;
        size_t pos;
        bool end;

        HashIter &operator++() {
            object->storage[bucket].nextIterator(&pos);
            while (pos == object->storage[bucket].end()) {
                bucket++;
                if (bucket >= object->getCapacity()) {
                    end = true;
                    break;
                }
                pos = object->storage[bucket].begin();
            }
            return *this;
        }

        HashIter &operator--() {
            object->storage[bucket].prevIterator(&pos);
            while (pos == object->storage[bucket].end()) {
                if (bucket == 0) {
                    end = true;
                    break;
                }
                bucket--;
                pos = object->storage[bucket].last();
            }
            return *this;
        }

        HashCell& operator*() {
            HashCell *value = nullptr;
            object->storage[bucket].get(pos, &value);
            return *value;
        }

        HashIter operator++(int) {
            const HashIter now = *this;
            ++(*this);
            return now;
        }

        bool operator==(const HashIter &other) {
            if (end == other.end && end)
                return true;
            return end == other.end && pos == other.pos && bucket == other.bucket && object == other.object;
        }

        bool operator!=(const HashIter &other) {
            return !(*this == other);
        }
    };
public:
    HashIter begin() {
        HashIter it = {this, 0, 0, false};
        return ++it;
    }

    HashIter end() {
        return {this, 0, 0, true};
    }
};

#endif //HashMasm_GUARD
