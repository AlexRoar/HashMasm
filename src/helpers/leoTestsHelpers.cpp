//
// Created by Александр Дремов on 06.04.2021.
//

#include "leoTestsHelpers.h"
#include "HashMasm.h"
#include <unordered_map>
#include <cstdio>
#include <cstdlib>

#define SIZE_MODIF /10;

void hashMasmTest(size_t allWhitespaces, char *content) {
    HashMasm<unsigned> htable = {};
    htable.init(971, false);

    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        token = flattenString(token);
        auto *valuePtr = htable.get(token);
        if (valuePtr == nullptr)
            htable.set(token, 1, false);
        else
            htable.set(token, 1 + *valuePtr, false);
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%% (%zu words)\n", i);
    const int testSize = htable.getSize() SIZE_MODIF;
    const int stringLen = 100;
    char* random = getRandomString(stringLen);
    for(int iter = 0; iter < testSize; iter++) {
        random = getRandomString(stringLen, random);
        auto get = htable.get(random);
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
        printf("\b\r%d%%", int(float(i * 100) / float(allWhitespaces)));
        fflush(stdout);
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
    while (token != nullptr && token > ret && *token == ' ') {
        *token = 0;
        token--;
    }
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
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    int i = 0;
    for (; i < size; ++i)
        tmp_s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
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