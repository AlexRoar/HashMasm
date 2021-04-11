//
// Created by Александр Дремов on 06.04.2021.
//

#ifndef HASHMASM_LEOTESTS1_H
#define HASHMASM_LEOTESTS1_H

#include <cstdlib>
#include <HashMasm.h>
#include <x86intrin.h>
#include <string_x86.h>

char *getRandomString(unsigned size);

size_t countFileWhitespace(const char *content);

char *flattenString(char *token);

char *readFile(const char *const *argv);

void hashStdTest(size_t allWhitespaces, char *content);

void hashMasmTest(size_t allWhitespaces, char *content);

void hashMasmTestFixed(size_t allWhitespaces, char *content);

char *getRandomString(unsigned size, char *tmp_s);

size_t countFileLines(const char *content);

__m256i makePacked(const char* value);

template<typename T>
T min(const T a, const T b) {
    return a > b ? b : a;
}

#endif //HASHMASM_LEOTESTS1_H
