//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HASHMASM_CRC_H
#define HASHMASM_CRC_H
#include "CRCLookup.h"
#include <cstdlib>

namespace CRC {
    size_t hash64(size_t crc , const unsigned char *p) {
            for (size_t i = 0; p[i] != 0; i++) {
                size_t t = ((crc >> 56) ^ (*p++)) & 0xFF;
                crc = crc64Lookup[t] ^ (crc << 8);
            }

            return crc;
    }

    size_t hash32(size_t crc , const unsigned char *p) {
        for (size_t i = 0; p[i] != 0; i++) {
            size_t t = ((crc >> 56) ^ (*p++)) & 0xFF;
            crc = crc32Lookup[t] ^ (crc << 8);
        }

        return crc;
    }
}

#endif //HASHMASM_CRC_H
