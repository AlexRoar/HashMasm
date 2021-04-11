//
// Created by Александр Дремов on 06.04.2021.
//

#include "leoTestsHelpers.h"
#include "HashMasm.h"
#include "HashMasmFixed.h"
#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>

#define SIZE_MODIF * 300;

void hashMasmTest(size_t allWhitespaces, char *content) {
    HashMasm<unsigned> htable = {};
    htable.init();

    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        token = flattenString(token);
        volatile __m256i tPacked = makePacked(token);
        auto *valuePtr = htable.get(token);
        if (valuePtr == nullptr)
            htable.set(token, 1, false);
        else
            htable.set(token, 1 + *valuePtr, false);
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%%\n");
    const int testSize = htable.getSize() SIZE_MODIF;
    const int stringLen = 40;
    char* random = getRandomString(stringLen);
    for(int iter = 0; iter < testSize; iter++) {
        random = getRandomString(stringLen, random);
        __m256i tPacked = makePacked(random);
        volatile auto get = htable.get(random);
    }
    free(random);

    printf("\r100%%\n");

    htable.dest();
}

void hashMasmTestFixed(size_t allWhitespaces, char *content) {
    HashMasmFixed<unsigned> htable = {};
    htable.init();

    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        token = flattenString(token);
        __m256i tPacked = makePacked(token);
        auto *valuePtr = htable.get(tPacked);
        if (valuePtr == nullptr)
            htable.set(tPacked, 1);
        else
            htable.set(tPacked, 1 + *valuePtr);
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%%\n");
    const int testSize = htable.getSize() SIZE_MODIF;
    const int stringLen = 32;
    char* random = getRandomString(stringLen);
    for(int iter = 0; iter < testSize; iter++) {
        random = getRandomString(stringLen, random);
        __m256i tPacked = makePacked(random);
        volatile auto get = htable.get(tPacked);
    }
    free(random);
    printf("\r100%%\n");
    htable.dest();
}

void hashStdTest(size_t allWhitespaces, char *content) {
    std::unordered_map<const char *, unsigned> htable;
    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        token = flattenString(token);
        auto valuePtr = htable.find(token);
        if (valuePtr == htable.end()) {
            htable[token] = 1;
        } else {
            htable[token] = 1 + valuePtr->second;
        }
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%%\n");
    const int testSize = htable.size() SIZE_MODIF;
    const int stringLen = 40;
    char* random = getRandomString(stringLen);
    for(int iter = 0; iter < testSize; iter++) {
        random = getRandomString(stringLen, random);
        volatile auto get = htable.find(random);
    }
    free(random);

    printf("\r100%%\n");
}


char *readFile(const char *const *argv) {
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("File (%s) is not found\n", argv[1]);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    auto string = (char *) calloc(fsize + 2, 1);
    fread(string, 1, fsize, file);
    fclose(file);
    string[fsize] = 0;
    return string;
}



char *flattenString(char *token) {
    char *ret = token;
    while (*token == ' ') {
        token++;
        ret = token;
    }
    char *tokenN = strchr(token, '\n');
    char *tokenR = strchr(token, '\r');
    if (!tokenN)
        token = tokenR;
    else
        token = tokenN;
    if (tokenR < token && tokenR)
        token = tokenR;
    if (token) {
        *token = 0;
        token--;
    }
    for (;token != nullptr && token > ret && *token == ' ';token--)
        *token = 0;
    return ret;
}

size_t countFileWhitespace(const char *content) {
    size_t res = 0;
    while (*content != 0) {
        if (*content == ' ')
            res++;
        content++;
    }
    return res;
}

size_t countFileLines(const char *content) {
    size_t res = 0;
    while (*content != 0) {
        if (*content == '\n')
            res++;
        content++;
    }
    return res;
}

char* getRandomString(unsigned size){
    auto tmp_s = (char *) calloc((size + 2), sizeof(char));
    getRandomString(size, tmp_s);
    return tmp_s;
}

char* getRandomString(unsigned size, char* tmp_s){
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    int i = 0;
    for (; i < size; ++i)
        tmp_s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

__m256i makePacked(const char* value) {
    bool ended = false;
    #define GET_SYMB (ended = (ended ? ended : *value == '\0'), ended ? '\0': *(value++))
    char  symbols[] __attribute__((aligned (32))) = {GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB,
                                                     GET_SYMB, GET_SYMB, GET_SYMB, GET_SYMB};
    return _mm256_load_si256(reinterpret_cast<const __m256i *>(symbols));
}