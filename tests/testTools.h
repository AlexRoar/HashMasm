//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HASHMASM_TESTTOOLS_H
#define HASHMASM_TESTTOOLS_H
#include <HashMasm.h>
#include "helpers/leoTestsHelpers.h"

struct StringMap {
    char* key;
    size_t val;
    __m256i keyPacked;
};

template <typename T>
void setGetTest (int testNum, int strSize, int startSize, bool variableStrSize=false, bool rehash = true) {
    auto testStrings = (StringMap*)(calloc(testNum + 1, sizeof (StringMap)));
    for (int i = 0; i < testNum; i++) {
        testStrings[i].key = getRandomString(variableStrSize ? i + 1 : strSize);
        testStrings[i].val = i;
    }

    T htable = {};
    ASSERT_EQ(htable.init(startSize, rehash), EXIT_SUCCESS);

    for (int i = 0; i < testNum; i++) {
        htable.set(testStrings[i].key, testStrings[i].val);
        ASSERT_EQ(htable.getSize(), i + 1);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].key);
        ASSERT_NE(found, nullptr);
        ASSERT_EQ(*found, testStrings[i].val);
    }

    for (int i = 0; i < testNum;i++){
        free(testStrings[i].key);
    }
    free(testStrings);
    htable.dest();
}

template <typename T>
void setGetTestFixed (int testNum, int strSize, int startSize, bool variableStrSize=false, bool rehash = true) {
    auto testStrings = (StringMap*)(calloc(testNum + 1, sizeof (StringMap)));
    for (int i = 0; i < testNum; i++) {
        testStrings[i].key = getRandomString(variableStrSize ? i + 1 : strSize);
        testStrings[i].keyPacked = makePacked(testStrings[i].key);
        testStrings[i].val = i;
    }

    T htable = {};
    ASSERT_EQ(htable.init(startSize, rehash), EXIT_SUCCESS);

    for (int i = 0; i < testNum; i++) {
        htable.set(testStrings[i].keyPacked, testStrings[i].val);
        ASSERT_EQ(htable.getSize(), i + 1);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].keyPacked);
        ASSERT_NE(found, nullptr);
        ASSERT_EQ(*found, testStrings[i].val);
    }

    for (int i = 0; i < testNum;i++){
        free(testStrings[i].key);
    }
    free(testStrings);
    htable.dest();
}

template <typename T>
void setGetRemoveTest (int testNum, int strSize, int startSize, bool variableStrSize=false, bool rehash = true) {
    auto testStrings = (StringMap*)(calloc(testNum + 1, sizeof (StringMap)));
    for (int i = 0; i < testNum; i++) {
        testStrings[i].key = getRandomString(variableStrSize ? i + 1 : strSize);
        testStrings[i].val = i;
    }

    T htable = {};
    ASSERT_EQ(htable.init(startSize, rehash), EXIT_SUCCESS);

    for (int i = 0; i < testNum; i++) {
        htable.set(testStrings[i].key, testStrings[i].val);
        ASSERT_EQ(htable.getSize(), i + 1);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].key);
        ASSERT_NE(found, nullptr);
        ASSERT_EQ(*found, testStrings[i].val);
    }

    for (int i = 0; i < testNum; i++) {
        htable.remove(testStrings[i].key);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].key);
        ASSERT_EQ(found, nullptr);
    }

    ASSERT_EQ(htable.getSize(), 0);

    for (int i = 0; i < testNum;i++){
        free(testStrings[i].key);
    }

    free(testStrings);
    htable.dest();
}

template <typename T>
void setGetRemoveTestFixed (int testNum, int strSize, int startSize, bool variableStrSize=false, bool rehash = true) {
    auto testStrings = (StringMap*)(calloc(testNum + 1, sizeof (StringMap)));
    for (int i = 0; i < testNum; i++) {
        testStrings[i].key = getRandomString(variableStrSize ? i + 1 : strSize);
        testStrings[i].val = i;
        testStrings[i].keyPacked = makePacked(testStrings[i].key);
    }

    T htable = {};
    ASSERT_EQ(htable.init(startSize, rehash), EXIT_SUCCESS);

    for (int i = 0; i < testNum; i++) {
        htable.set(testStrings[i].keyPacked, testStrings[i].val);
        ASSERT_EQ(htable.getSize(), i + 1);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].keyPacked);
        ASSERT_NE(found, nullptr);
        ASSERT_EQ(*found, testStrings[i].val);
    }

    for (int i = 0; i < testNum; i++) {
        htable.remove(testStrings[i].keyPacked);
    }

    for (int i = 0; i < testNum; i++) {
        int* found = htable.get(testStrings[i].keyPacked);
        ASSERT_EQ(found, nullptr);
    }

    ASSERT_EQ(htable.getSize(), 0);

    for (int i = 0; i < testNum;i++){
        free(testStrings[i].key);
    }

    free(testStrings);
    htable.dest();
}

#endif //HASHMASM_TESTTOOLS_H
