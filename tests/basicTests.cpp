//
// Created by Александр Дремов on 31.03.2021.
//

#include "gtest/gtest.h"
#include <cstdlib>
#include "testTools.h"
#include <HashMasm.h>

TEST(HashMasm, init) {
    HashMasm<int> htable = {};
    ASSERT_EQ(htable.init(), EXIT_SUCCESS);
    htable.dest();

    ASSERT_EQ(htable.init(100), EXIT_SUCCESS);
    ASSERT_EQ(htable.getCapacity(), 100);
    htable.dest();

    ASSERT_EQ(htable.init(), EXIT_SUCCESS);
    ASSERT_EQ(htable.getCapacity(), HashMasm<int>::getMinCapacity());
    htable.dest();
}

TEST(HashMasm, setGetRandom) {
    const int testSize = 10000;
    setGetTest(testSize, 1, testSize * 2, true);
}

TEST(HashMasm, setGetRandomFixedSize) {
    const int testSize = 10000;
    setGetTest(testSize, 100, 32, false);
}

TEST(HashMasm, setGetRandomOverflowed) {
    const int testSize = 10000;
    setGetTest(testSize, 50, 32, false);
}

TEST(HashMasm, setGetRandomNoRehash) {
    const int testSize = 100000;
    setGetTest(testSize, 50, testSize * 2, false);
}

TEST(HashMasm, setGetRandomRehash) {
    const int testSize = 100000;
    setGetTest(testSize, 50, 0, false);
}