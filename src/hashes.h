//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HASHMASM_HASHES_H
#define HASHMASM_HASHES_H

#include "hashesLookup.h"
#include <cstdlib>

namespace CRC {
    size_t hash64(size_t crc, const unsigned char *p) {
        for (size_t i = 0; p[i] != 0; i++) {
            size_t t = ((crc >> 56) ^ (*p++)) & 0xFF;
            crc = crc64Lookup[t] ^ (crc << 8);
        }

        return crc;
    }

    size_t hash32(size_t crc, const unsigned char *p) {
        for (size_t i = 0; p[i] != 0; i++) {
            size_t t = ((crc >> 56) ^ (*p++)) & 0xFF;
            crc = crc32Lookup[t] ^ (crc << 8);
        }

        return crc;
    }
}

namespace FNV {
    size_t fnv64(const char *p, size_t hash = 0) {
        const size_t fnv_prime = 1099511628211;
        for (size_t i = 0; p[i] != 0; i++) {
            hash *= fnv_prime;
            hash ^= (p[i]);
        }
        return hash;
    }
}

#endif //HASHMASM_HASHES_H
