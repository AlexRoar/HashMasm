//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HASHMASM_TESTTOOLS_H
#define HASHMASM_TESTTOOLS_H
#include <HashMasm.h>

struct StringMap {
    char* key;
    size_t val;
};

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

void setGetTest (int testNum, int strSize, int startSize, bool variableStrSize=false, bool rehash = true) {
    auto testStrings = (StringMap*)(calloc(testNum + 1, sizeof (StringMap)));
    for (int i = 0; i < testNum; i++) {
        testStrings[i].key = getRandomString(variableStrSize ? i + 1 : strSize);
        testStrings[i].val = i;
    }

    HashMasm<int> htable = {};
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

#endif //HASHMASM_TESTTOOLS_H
