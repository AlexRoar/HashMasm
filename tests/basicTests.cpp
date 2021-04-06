//
// Created by Александр Дремов on 31.03.2021.
//

#include "gtest/gtest.h"
#include "testTools.h"
#include <cstdlib>
#include <HashMasm.h>

TEST(HashMasm, init) {
    HashMasm<int> htable = {};
    ASSERT_EQ(htable.init(), EXIT_SUCCESS);
    htable.dest();

    ASSERT_EQ(htable.init(100), EXIT_SUCCESS);
    ASSERT_TRUE(htable.getCapacity() > 100);
    htable.dest();

    ASSERT_EQ(htable.init(), EXIT_SUCCESS);
    ASSERT_TRUE(htable.getCapacity() > HashMasm<int>::getMinCapacity());
    htable.dest();
}

TEST(HashMasm, iteratorTests) {
    HashMasm<unsigned char> htable = {};
    htable.init();
    char a[] = "a";
    for(unsigned char i = 0; i < 25; i++){
        a[0] = 'a' + i;
        htable[a] = i;
    }

    size_t i = 0;
    for(const auto & elem : htable){
        ASSERT_EQ(elem.key[0] - 'a', elem.value);
        i++;
    }
    ASSERT_EQ(i, htable.getSize());
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

TEST(HashMasm, setGetRandomNoRehashBig) {
    const int testSize = 100000;
    setGetTest(testSize, 50, testSize * 2, false);
}

TEST(HashMasm, setGetRandomRehash) {
    const int testSize = 100000;
    setGetTest(testSize, 50, 0, false);
}

TEST(HashMasm, setGetRandomNoRehashSmall) {
    const int testSize = 100000;
    setGetTest(testSize, 50, 100, false, false);
}

TEST(HashMasm, setGetRemoveRandomNoRehashSmall) {
    const int testSize = 100000;
    setGetRemoveTest(testSize, 50, 10000, false, false);
}

TEST(HashMasm, setGetRemoveRandomRehashSmall) {
    const int testSize = 100000;
    setGetRemoveTest(testSize, 50, 0, false, true);
}

TEST(HashMasm, subscriptorTests) {
    HashMasm<char> htable = {};
    ASSERT_EQ(htable.init(), EXIT_SUCCESS);
    htable["one"] = 1;
    ASSERT_EQ(htable.getSize(), 1);
    ASSERT_EQ(htable["one"], 1);
    ASSERT_EQ(htable["one"], htable["one"]);

    htable["two"] = 2;

    ASSERT_EQ(htable.getSize(), 2);
    ASSERT_EQ(htable["two"], 2);
    ASSERT_EQ(htable["two"], htable["two"]);

    htable["three"];
    ASSERT_EQ(htable.getSize(), 3);
    htable["two"]--;
    ASSERT_EQ(htable["two"], htable["one"]);
}

